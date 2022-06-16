//
// Created by shamil on 15.06.22.
//

#ifndef TESTAPP_SOCKET_TCPSOCKET_HPP_
#define TESTAPP_SOCKET_TCPSOCKET_HPP_

// std
#include <string_view>
#include <cstring>

// linux
#include <unistd.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netdb.h>

// project
#include "utility.hpp"

namespace mysocket {
class TcpServerSocket;

class TcpSocket {
 private:
  explicit TcpSocket(int socketfd) noexcept;

 public:
  TcpSocket(const char *host, unsigned short port);
  ~TcpSocket() noexcept;

  TcpSocket(const TcpSocket &) noexcept = delete;
  TcpSocket &operator=(const TcpSocket &) noexcept = delete;

  TcpSocket(TcpSocket &&rhs) noexcept;
  TcpSocket &operator=(TcpSocket &&rhs) noexcept;

  // Reads symbols from socket (count + 1 for '\0').  Produces null-terminated string.
  ssize_t Read(char *result_buffer, ssize_t count) const;
  // Reads from socket until delimiter (maximum 'count' symbols + 1 for '\0'). Produces null-terminated string.
  ssize_t ReadUntil(char *result_buffer, ssize_t count, char delimiter) const;
  // Writes symbols to socket
  void Write(std::string_view view) const;

  // Closes sending
  void ShutDown();

 private:
  friend TcpServerSocket;

 private:
  int socketfd_;
  bool is_shutdown_;
};
}

#endif //TESTAPP_SOCKET_TCPSOCKET_HPP_
