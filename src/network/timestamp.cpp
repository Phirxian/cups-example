#include <ctime>

#include "timestamp.h"
#include "util.h"

namespace netsys
{
    static const uint64_t MILLION = 1000000;
    static const uint64_t BILLION = 1000 * MILLION;

    static timespec current_time()
    {
        timespec ret;
        SystemCall("clock_gettime", clock_gettime(CLOCK_REALTIME, &ret));
        return ret;
    }

    static uint64_t timestamp_ms_raw(const timespec &ts)
    {
        const uint64_t nanos = ts.tv_sec * BILLION + ts.tv_nsec;
        return nanos / MILLION;
    }

    uint64_t timestamp_ms()
    {
        return timestamp_ms(current_time());
    }

    uint64_t timestamp_ms(const timespec &ts)
    {
        const static uint64_t EPOCH = timestamp_ms_raw(current_time());
        return timestamp_ms_raw(ts) - EPOCH;
    }
}
