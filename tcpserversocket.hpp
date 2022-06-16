//
// Created by shamil on 15.06.22.
//

#ifndef TESTAPP_SOCKET_TCPSERVERSOCKET_HPP_
#define TESTAPP_SOCKET_TCPSERVERSOCKET_HPP_

// project
#include "tcpsocket.hpp"

namespace mysocket {
class TcpServerSocket {
 public:
  explicit TcpServerSocket(const unsigned short port);
  ~TcpServerSocket() noexcept;

  TcpServerSocket(const TcpServerSocket &rhs) noexcept = delete;
  TcpServerSocket &operator=(const TcpServerSocket &rhs) noexcept = delete;

  TcpServerSocket(TcpServerSocket &&rhs) noexcept;
  TcpServerSocket& operator=(TcpServerSocket &&rhs) noexcept;

  [[nodiscard]] TcpSocket Accept() const;

 private:
  int socketfd_;
};

} // mysocket

#endif //TESTAPP_SOCKET_TCPSERVERSOCKET_HPP_
