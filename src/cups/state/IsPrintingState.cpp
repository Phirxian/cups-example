#include "IsPrintingState.h"

#include "../Job.h"
#include "../Printer.h"
#include "SuspendState.h"
#include "OnState.h"

namespace cups
{
    IsPrintingState::IsPrintingState(Printer *printer) noexcept
        : PrinterState(printer)
    {
    }

    bool IsPrintingState::printingCompleted() const
    {
        job->setPrinter(nullptr);
        job->setState(EJS_COMPLETED);
        printer->setCurrentJob(nullptr);
        printer->setState(new OnState(printer));
        return true;
    }

    bool IsPrintingState::suspend() const
    {
        printer->getCurrentJob()->setState(EJS_SUSPEND);
        printer->setState(new SuspendState(printer));
        return true;
    }
    
    bool IsPrintingState::isAvailable() const
    {
        return false;
    }
}