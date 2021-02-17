#pragma once

#include "IPrinterState.h"

namespace cups
{
    class Printer;
    
    class PrinterState : public IPrinterState
    {
        public:
            PrinterState(Printer *printer) noexcept
                : printer(printer)
            {
            }
            
            virtual bool start() const
            {
                return error();
            }

            virtual bool stop() const
            {
                return error();
            }

            virtual bool print(Job *job) const
            {
                return error();
            }

            virtual bool printingCompleted() const
            {
                return error();
            }

            virtual bool suspend() const
            {
                return error();
            }

            virtual bool resume() const
            {
                return error();
            }

            virtual bool cancel() const
            {
                return error();
            }
        protected:
            static bool error()
            {
                throw "Erreur";
                return false;
            }
        protected:
            Printer *printer;
    };
}
