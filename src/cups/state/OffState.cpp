#include "OffState.h"

#include "OnState.h"
#include "../Printer.h"

namespace cups
{
    OffState::OffState(Printer *printer) noexcept
        : PrinterState(printer)
    {
    }

    bool OffState::start() const
    {
        printer->setState(new OnState(printer));
        return true;
    }

    bool OffState::isAvailable() const
    {
        return false;
    }
}
