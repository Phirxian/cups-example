#pragma once

#include "PrinterState.h"

namespace cups
{
    class IsPrintingState : public PrinterState
    {
        public:
            IsPrintingState(Printer *printer) noexcept;
            virtual bool printingCompleted() const;
            virtual bool suspend() const;
            bool isAvailable() const;
        protected:
    };
}