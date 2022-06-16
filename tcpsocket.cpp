//
// Created by shamil on 15.06.22.
//

#include "tcpsocket.hpp"

namespace mysocket {
TcpSocket::TcpSocket(int socketfd) noexcept: socketfd_{socketfd}, is_shutdown_{} {}

TcpSocket::TcpSocket(const char *host, unsigned short port) : socketfd_{socket(AF_INET, SOCK_STREAM, 0)},
                                                                    is_shutdown_{} {
  if (socketfd_ < 0) {
    throw SocketCreationError{};
  }

  addrinfo hints{};
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo *result;
  auto addr_info_res{getaddrinfo(host, std::to_string(port).c_str(), &hints, &result)};
  if (addr_info_res != 0) {
    throw SocketError{};
  }

  auto address{result};
  for (; address != nullptr; address = address->ai_next) {
    if (connect(socketfd_, address->ai_addr, address->ai_addrlen) != -1) {
      break;
    }
  }

  freeaddrinfo(result);

  if (address == nullptr) {
    close(socketfd_);
    throw SocketError{"Cannot determine the address"};
  }
}

TcpSocket::~TcpSocket() noexcept {
  if (socketfd_ != -1) {
    close(socketfd_);
  }
}

TcpSocket::TcpSocket(TcpSocket &&rhs) noexcept: socketfd_{rhs.socketfd_}, is_shutdown_{rhs.is_shutdown_} {
  rhs.socketfd_ = -1;
  rhs.is_shutdown_ = true;
}

TcpSocket &TcpSocket::operator=(TcpSocket &&rhs) noexcept {
  new(this) TcpSocket{std::move(rhs)};
  return *this;
}

ssize_t TcpSocket::Read(char *result_buffer, ssize_t count) const {
  long long read_count{};
  while (read_count < count) {
    auto read_num{read(socketfd_, result_buffer + read_count, count - read_count)};
    if (read_num == 0) {
      result_buffer[read_count + read_num] = '\0';
      return read_count;
    }

    if (read_num < 0) {
      throw SocketReadError{};
    }

    read_count += read_num;
  }

  result_buffer[count] = '\0';
  return read_count;
}

ssize_t TcpSocket::ReadUntil(char *result_buffer, ssize_t count, char delimiter) const {
  long long read_count{};
  while (read_count < count) {
    auto read_num{read(socketfd_, result_buffer + read_count, 1)};
    if (read_num < 0) {
      throw SocketReadError{};
    }

    if (result_buffer[read_count] == delimiter) {
      result_buffer[read_count] = '\0';
      return read_count;
    }

    if (read_num == 0) {
      result_buffer[read_count + 1] = '\0';
      return read_count;
    }

    ++read_count;
  }

  result_buffer[count] = '\0';
  return read_count;
}

void TcpSocket::Write(std::string_view view) const {
  if (is_shutdown_) {
    throw SocketClosedError{};
  }

  const auto kLength{view.size()};

  const char *data{view.data()};
  long long sent{};
  while (sent < kLength) {
    auto sent_num{write(socketfd_, data + sent, kLength - sent)};
    if (sent_num < 0) {
      throw SocketWriteError();
    }
    sent += sent_num;
  }
}

void TcpSocket::ShutDown() {
  if (!is_shutdown_) {
    if (shutdown(socketfd_, SHUT_WR) < 0) {
      throw SocketShutdownError{};
    }
    is_shutdown_ = true;
  }
}
}