#ifndef UTIL_HH
#define UTIL_HH

#include <system_error>
#include <iostream>
#include <string>
#include <cstring>

namespace netsys
{
    class tagged_error : public std::system_error
    {
        public:
            tagged_error(const std::error_category &category,  const std::string &s_attempt, const int error_code)
              : system_error(error_code, category), attempt_and_error_(s_attempt + ": " + std::system_error::what())
            {
            }

            const char* what() const noexcept override
            {
                return attempt_and_error_.c_str();
            }
        private:
          std::string attempt_and_error_;
    };

    class unix_error : public tagged_error
    {
        public:
            unix_error(const std::string &s_attempt, const int s_errno = errno)
              : tagged_error(std::system_category(), s_attempt, s_errno)
            {
            }
    };

    inline void print_exception(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    inline int SystemCall(const char *s_attempt, const int return_value)
    {
        if(return_value >= 0)
            return return_value;
        throw unix_error(s_attempt);
    }

    inline int SystemCall(const std::string &s_attempt, const int return_value)
    {
        return SystemCall(s_attempt.c_str(), return_value);
    }

    template <typename T> void zero(T &x) { memset(&x, 0, sizeof(x)); }
}

#endif/* UTIL_HH */
