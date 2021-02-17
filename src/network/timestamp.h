#ifndef TIMESTAMP_HH
#define TIMESTAMP_HH

#include <ctime>
#include <cstdint>

namespace netsys
{
    uint64_t timestamp_ms();
    uint64_t timestamp_ms(const timespec &ts);
}

#endif /* TIMESTAMP_HH */
