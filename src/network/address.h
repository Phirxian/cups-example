#ifndef ADDRESS_HH
#define ADDRESS_HH

#include <string>
#include <utility>

#include <netinet/in.h>
#include <netdb.h>

namespace netsys
{
    class Address
    {
        public:
          typedef union {
              sockaddr as_sockaddr;
              sockaddr_storage as_sockaddr_storage;
          } raw;
        public:
            Address();
            Address(const raw & addr, const size_t size);
            Address(const sockaddr & addr, const size_t size);

            Address(const std::string &hostname, const std::string &service);
            Address(const std::string &ip, const uint16_t port);
            
            inline std::string ip() const { return ip_port().first; }
            inline uint16_t port() const { return ip_port().second; }

            std::pair<std::string, uint16_t> ip_port() const;
            std::string to_string() const;

            socklen_t size() const { return size_; }
            const sockaddr& to_sockaddr() const;

            bool operator == (const Address & other) const;
        private:
            Address(const std::string & node, const std::string & service, const addrinfo * hints);
            socklen_t size_;
            raw addr_;
    };
}

#endif /* ADDRESS_HH */
