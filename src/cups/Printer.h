#pragma once

#include <string>
#include "state/IPrinterState.h"
#include "Spooler.h"

#include "../sync/process.h"

namespace cups
{
    class Spooler;
    class PrinterState;
    
    class Printer : public IPrinterState, public syncsys::consumer<Job*>
    {
        public:
            Printer() noexcept;
            virtual ~Printer() noexcept;
            
            void setSpooler(Spooler *sp);
            Spooler* getSpooler() const;

            void setName(const std::string &n);
            std::string getName();

            void setState(PrinterState *s);
            PrinterState* getState() const;

            virtual bool start() const;
            virtual bool stop() const;

            virtual bool print(Job *job) const;
            virtual bool printingCompleted() const;
            
            virtual bool suspend() const;
            virtual bool resume() const;
            virtual bool cancel() const;
            
            Job* getCurrentJob() const;
            void setCurrentJob(Job *job);
            
            bool isAvailable() const;
            
            virtual void run();
        protected:
            std::string name;
            Job *currentJob;
            Spooler *spooler;
            PrinterState *state;
    };
}
