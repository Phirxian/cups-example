
#pragma once

#include "../Job.h"

namespace cups
{
    class IPrinterState
    {
        public:
            virtual bool start() const = 0;
            virtual bool stop() const = 0;
            virtual bool print(Job *job) const = 0;
            virtual bool printingCompleted() const = 0;
            virtual bool suspend() const = 0;
            virtual bool resume() const = 0;
            virtual bool cancel() const = 0;
            virtual bool isAvailable() const = 0;
        protected:
            Job *job;
    };
}
