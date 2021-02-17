#ifndef FILE_DESCRIPTOR_HH
#define FILE_DESCRIPTOR_HH

#include <string>

namespace netsys
{
    class FileDescriptor
    {
        public:
            FileDescriptor(const int fd);
            FileDescriptor(FileDescriptor &&other);
            FileDescriptor(const FileDescriptor &other) = delete;
            const FileDescriptor &operator = (const FileDescriptor & other) = delete;
            
            virtual ~FileDescriptor();

            inline const int & fd_num() const { return fd_; }
            inline const bool & eof() const { return eof_; }
            inline unsigned int read_count() const { return read_count_; }
            inline unsigned int write_count() const { return write_count_; }

            std::string read(const size_t limit = BUFFER_SIZE);
            std::string::const_iterator write(const std::string &buffer, const bool write_all = true);
        protected:
            inline void register_read() { read_count_++; }
            inline void register_write() { write_count_++; }
            inline void set_eof() { eof_ = true; }
        private:
            int fd_;
            bool eof_;

            unsigned int read_count_, write_count_;

            std::string::const_iterator write(
                const std::string::const_iterator &begin,
                const std::string::const_iterator &end
            );

            const static size_t BUFFER_SIZE = 1024 * 1024;
    };
}

#endif /* FILE_DESCRIPTOR_HH */
