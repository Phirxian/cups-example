#ifndef POLLER_HH
#define POLLER_HH

#include <functional>
#include <vector>
#include <poll.h>

#include "file_descriptor.h"

namespace netsys
{
    class Poller
    {
        public:
            struct Action
            {
                enum PollDirection : short
                {
                    In = POLLIN,
                    Out = POLLOUT
                };
                
                struct Result
                {
                    enum class Type
                    {
                        Continue,
                        Exit,
                        Cancel
                    };
                    
                    Result(const Type &s_result = Type::Continue, const unsigned int &s_status = EXIT_SUCCESS)
                        : result(s_result), exit_status(s_status)
                    {
                    }
                    
                    Type result;
                    unsigned int exit_status;
                };

                typedef std::function<Result()> CallbackType;

                Action(FileDescriptor &s_fd, const PollDirection &s_direction, const CallbackType &s_callback, const std::function<bool(void)> &s_when_interested = []{ return true; })
                    : fd(s_fd), direction(s_direction), callback(s_callback), when_interested(s_when_interested), active(true)
                {
                }

                unsigned int service_count() const;

                FileDescriptor &fd;
                PollDirection direction;
                CallbackType callback;
                
                std::function<bool(void)> when_interested;
                bool active;
            };
        public:
            struct Result
            {
                enum class Type
                {
                    Success,
                    Timeout,
                    Exit
                };
                
                Result(const Type &s_result, const unsigned int & s_status = EXIT_SUCCESS)
                    : result(s_result), exit_status(s_status)
                {
                }
                
                Type result;
                unsigned int exit_status;
            };
        public:
            Poller() : actions_(), pollfds_()
            {
            }
            
            void add_action(Action action);
            Result poll(const int &timeout_ms);
        private:
            std::vector<Action> actions_;
            std::vector<pollfd> pollfds_;
    };

    namespace PollerShortNames
    {
        typedef Poller::Action::Result Result;
        typedef Poller::Action::Result::Type ResultType;
        typedef Poller::Action::PollDirection Direction;
        typedef Poller::Action Action;
        typedef Poller::Result::Type PollResult;
    }
}
#endif /* POLLER_HH */
