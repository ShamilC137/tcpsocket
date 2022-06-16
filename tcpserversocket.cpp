//
// Created by shamil on 15.06.22.
//

#include "tcpserversocket.hpp"

namespace mysocket {
TcpServerSocket::TcpServerSocket(const unsigned short port) : socketfd_{socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)} {
  if (socketfd_ < 0) {
    throw SocketCreationError{};
  }

  addrinfo hints{};
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_family = AF_INET;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;

  addrinfo *addrinfo{};
  auto addr_info_res{getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &addrinfo)};
  if (addr_info_res != 0) {
    throw SocketError{};
  }

  auto address{addrinfo};
  for (; address != nullptr; address = address->ai_next) {
    if (bind(socketfd_, address->ai_addr, address->ai_addrlen) == 0) {
      break;
    }
  }

  freeaddrinfo(addrinfo);

  if (address == nullptr) {
    close(socketfd_);
    throw SocketCreationError{};
  }

  if (listen(socketfd_, SOMAXCONN) == -1) {
    close(socketfd_);
    throw SocketCreationError{};
  }
}

TcpServerSocket::~TcpServerSocket() noexcept {
  if (socketfd_ != -1) {
    close(socketfd_);
  }
}

TcpServerSocket::TcpServerSocket(TcpServerSocket &&rhs) noexcept: socketfd_{rhs.socketfd_} {
  rhs.socketfd_ = -1;
}

TcpServerSocket &TcpServerSocket::operator=(TcpServerSocket &&rhs) noexcept {
  socketfd_ = rhs.socketfd_;
  rhs.socketfd_ = -1;
  return *this;
}

TcpSocket TcpServerSocket::Accept() const {
  auto fd{accept(socketfd_, nullptr, nullptr)};
  if (fd < 0) {
    throw SocketError{};
  }

  return TcpSocket{fd};
}

} // mysocket