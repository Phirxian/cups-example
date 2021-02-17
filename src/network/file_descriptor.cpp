#include "file_descriptor.h"
#include "util.h"

#include <unistd.h>

using namespace std;

namespace netsys
{
    const size_t FileDescriptor::BUFFER_SIZE;

    FileDescriptor::FileDescriptor(const int fd)
      : fd_(fd), eof_(false), read_count_(0), write_count_(0)
    {
    }

    FileDescriptor::FileDescriptor(FileDescriptor &&other)
      : fd_(other.fd_), eof_(other.eof_),
        read_count_(other.read_count_),
        write_count_(other.write_count_)
    {
        other.fd_ = -1;
    }

    FileDescriptor::~FileDescriptor()
    {
        if(fd_ < 0) return;

        try
        {
            SystemCall( "close", close( fd_ ) );
        }
        catch(const exception &e)
        {
            print_exception(e);
        }
    }

    string::const_iterator FileDescriptor::write(const string::const_iterator &begin, const string::const_iterator &end)
    {
        if(begin >= end)
            throw runtime_error("nothing to write");

        ssize_t bytes_written = SystemCall("write", ::write(fd_, &*begin, end-begin));
        
        if(bytes_written == 0)
            throw runtime_error("write returned 0");
        register_write();

        return begin + bytes_written;
    }

    string FileDescriptor::read(const size_t limit)
    {
        char buffer[BUFFER_SIZE];

        ssize_t bytes_read = SystemCall("read", ::read(fd_, buffer, min(BUFFER_SIZE, limit)));
        
        if(bytes_read == 0)
            set_eof();
        register_read();

        return string(buffer, bytes_read);
    }

    string::const_iterator FileDescriptor::write(const std::string &buffer, const bool write_all)
    {
        auto it = buffer.begin();
        
        do {
            it = write(it, buffer.end());
        }
        while(write_all and (it != buffer.end()));

        return it;
    }
}
