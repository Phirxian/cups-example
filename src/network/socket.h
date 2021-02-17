#ifndef SOCKET_HH
#define SOCKET_HH

#include <functional>

#include "address.h"
#include "file_descriptor.h"

namespace netsys
{
    class Socket : public FileDescriptor
    {
        public:
            void bind(const Address &address);
            void connect(const Address &address);

            Address local_address() const;
            Address peer_address() const;

            void set_reuseaddr();
        protected:
            Socket(const int domain, const int type);
            Socket(FileDescriptor &&s_fd, const int domain, const int type);

            template <typename option_type>
            void setsockopt(const int level, const int option, const option_type &option_value);
        private:
            Address get_address(
                const std::string &name_of_function,
                const std::function<int(int, sockaddr*, socklen_t*)> &function
            ) const;
    };

    class UDPSocket : public Socket
    {
        public:
            UDPSocket() : Socket(AF_INET6, SOCK_DGRAM)
            {
            }

            struct received_datagram {
                Address source_address;
                uint64_t timestamp;
                std::string payload;
            };

            received_datagram recv();
            
            void sendto(const Address &peer, const std::string &payload);
            void send(const std::string &payload);

            void set_timestamps();
    };

    class TCPSocket : public Socket
    {
        public:
            TCPSocket() : Socket(AF_INET6, SOCK_STREAM)
            {
            }

            void listen(const int backlog = 16);
            TCPSocket accept();
        private:
            TCPSocket(FileDescriptor &&fd) :
                Socket(std::move(fd), AF_INET6, SOCK_STREAM)
            {
            }
    };
}

#endif /* SOCKET_HH */
