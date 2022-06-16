//
// Created by shamil on 15.06.22.
//

#ifndef TESTAPP_SOCKET_UTILITY_HPP_
#define TESTAPP_SOCKET_UTILITY_HPP_

// std
#include <stdexcept>

#define GENERATE_ERROR_CLASS(name, base, default_message)  class name : public base {                   \
                                                             public:                                    \
                                                               using Base = base;                       \
                                                               using Base::Base;                        \
                                                               inline name() : Base(default_message) {} \
                                                               using Base::operator=;                   \
                                                               using Base::what;                        \
                                                           };
namespace mysocket {
GENERATE_ERROR_CLASS(SocketError, std::runtime_error, "Something went wrong")
GENERATE_ERROR_CLASS(SocketCreationError, SocketError, "Something went wrong during creation")
GENERATE_ERROR_CLASS(SocketConnectionError, SocketError, "Something went wrong during connection")
GENERATE_ERROR_CLASS(SocketShutdownError, SocketError, "Something went wrong during shutdown")
GENERATE_ERROR_CLASS(SocketClosedError, SocketError, "Socket is already closed")
GENERATE_ERROR_CLASS(SocketWriteError, SocketError, "Something went wrong during while writing")
GENERATE_ERROR_CLASS(SocketReadError, SocketError, "Something went wrong during while reading")
}
#undef GENERATE_ERROR_CLASS
#endif //TESTAPP_SOCKET_UTILITY_HPP_
