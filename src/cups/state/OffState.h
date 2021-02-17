#pragma once

#include "PrinterState.h"

namespace cups
{
    class OffState : public PrinterState
    {
        public:
            OffState(Printer *printer) noexcept;
            
            virtual bool start() const;
            
            bool isAvailable() const;
        protected:
            Job *job;
    };
}
