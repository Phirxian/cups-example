#include "OnState.h"

#include "IsPrintingState.h"
#include "OffState.h"

#include "../Printer.h"
#include "../Job.h"

namespace cups
{
    OnState::OnState(Printer *printer) noexcept
        : PrinterState(printer)
    {
    }

    bool OnState::stop() const
    {
        printer->setState(new OffState(printer));
        return true;
    }
    
    bool OnState::print(Job *job) const
    {
        printer->setCurrentJob(job);
        job->setPrinter(printer);
        job->setState(EJS_COMPLETED);
        printer->setState(new IsPrintingState(printer));
        return true;
    }

    bool OnState::isAvailable() const
    {
        return true;
    }
}
