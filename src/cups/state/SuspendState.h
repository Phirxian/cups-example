#pragma once

#include "../Printer.h"
#include "PrinterState.h"

namespace cups
{
    class SuspendState : public PrinterState
    {
        public:
            SuspendState(Printer *printer) noexcept;
            
            virtual bool resume() const;
            virtual bool cancel() const;
            
            bool isAvailable() const;
    };
}
