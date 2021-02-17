#pragma once

#include "../sync/process.h"
#include "../network/socket.h"

namespace cups
{
    class Job;
    class Spooler;
    class User : public syncsys::producer<Job*>
    {
        public:
            User(Spooler *spooler, netsys::TCPSocket &&) noexcept;
            virtual ~User() noexcept;
            
            int getQuota() const;
            
            void increaseQuota(int nbPages);
            void decreaseQuota(int nbPages);
            
            void run();
        protected:
            netsys::TCPSocket client;
            std::vector<Job*> jobs;
            Spooler *spooler;
            int quota = 0;
    };
}
