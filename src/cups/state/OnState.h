#pragma once

#include "PrinterState.h"
#include "IsPrintingState.h"
#include "OffState.h"

#include "../Printer.h"
#include "../Job.h"

namespace cups
{
    class OnState : public PrinterState
    {
        public:
            OnState(Printer *printer) noexcept;
            
            virtual bool stop() const;
            bool print(Job *job) const;
            
            bool isAvailable() const;
        protected:
            Job *job;
    };
}
