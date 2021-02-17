#include "SuspendState.h"

#include "IsPrintingState.h"
#include "OnState.h"

namespace cups
{
    SuspendState::SuspendState(Printer *printer) noexcept
        : PrinterState(printer)
    {
    }
    
    bool SuspendState::resume() const
    {
        job->setState(EJS_PRINTING);
        printer->setState(new IsPrintingState(printer));
        return true;
    }

    bool SuspendState::cancel() const
    {
        printer->getCurrentJob()->setState(EJS_CANCELED);
        printer->getCurrentJob()->setPrinter(nullptr);
        printer->setState(new OnState(printer));
        return true;
    }
    
    bool SuspendState::isAvailable() const
    {
        return false;
    }
}