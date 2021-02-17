#include "Printer.h"

#include "state/OffState.h"
#include <unistd.h>

namespace cups
{
    Printer::Printer() noexcept
        : syncsys::consumer<Job*>(spooler),
          currentJob(nullptr)
    {
        state = new OffState(this);
    }
    
    Printer::~Printer() noexcept
    {
        delete state;
    }
    
    void Printer::setSpooler(Spooler *sp)
    {
        if(sp)
        {
            spooler = sp;
            pthread_create(&thread, nullptr, syncsys::process_run, (void*)this);
        }
        else
        {
            pthread_cancel(thread);
        }
    }
    
    Spooler* Printer::getSpooler() const
    {
        return spooler;
    }

    std::string Printer::getName()
    {
        return name;
    }

    void Printer::setName(const std::string &n)
    {
        name = n;
    }

    void Printer::setState(PrinterState *s)
    {
        delete state;
        state = s;
    }
    
    PrinterState* Printer::getState() const
    {
        return state;
    }

    bool Printer::start() const
    {
        return getState()->start();
    }

    bool Printer::stop() const
    {
        return getState()->stop();
    }

    bool Printer::print(Job *job) const
    {
        // l'imprimante doit être allumée
        // et ne doit pas être en train d'imprimer un document
        return getState()->print(job);
    }

    bool Printer::printingCompleted() const
    {
        // l'imprimante doit être allumée
        // et en train d'imprimer un document
        return getState()->printingCompleted();
    }

    bool Printer::suspend() const
    {
        // L'imprimante doit être en cours d'impression
        // et pas encore suspendue.
        return getState()->suspend();
    }

    bool Printer::resume() const
    {
        // L'imprimante doit être active
        // et avoir une impression suspendue
        return getState()->resume();
    }

    bool Printer::cancel() const
    {
        return getState()->cancel();
    }

    bool Printer::isAvailable() const
    {
        PrinterState *tmp = const_cast<PrinterState*>(getState());
        return tmp->isAvailable();
    }

    Job* Printer::getCurrentJob() const
    {
        return currentJob;
    }

    void Printer::setCurrentJob(Job *job)
    {
        currentJob = job;
    }
    
    void Printer::run()
    {
        while(true)
        {
            if(state->isAvailable())
            {
                sem_wait(&sem);
                
                Job *nextJob = nullptr;
                *spooler >> nextJob;
                state->print(job);
                
                //! emulation of the process work time
                sleep(1);
                
                state->printingCompleted();                        
                s->notifyCons();
            }
        }
    }
}
