#include "Job.h"
#include "Printer.h"
#include "Document.h"
#include "User.h"

#include "../sync/stream.h"
#include <iostream> 

namespace cups
{
    Spooler::Spooler(netsys::Address &&addr)
        : syncsys::stream<Job*>(1000)
    {
        socket.set_reuseaddr();
        socket.bind(addr);
        socket.listen();
        
        std::cerr << "Listening on local address: "
                  << socket.local_address().to_string()
                  << std::endl;
    }

    void Spooler::join(Printer *aPrinter)
    {
        printers.push_back(aPrinter);
        aPrinter->setSpooler(this);
        aPrinter->start();
    }

    void Spooler::quit(Printer *aPrinter)
    {
        for(auto it = printers.begin(); it != printers.end(); ++it)
            if(*it == aPrinter)
                printers.erase(it);
        aPrinter->setSpooler(nullptr);
        aPrinter->stop();
    }

    Job* Spooler::print(Document *aDocument, User *aUser)
    {
        Job *aJob = new Job(aUser, aDocument);
        aJob->setState(EJS_WAITING);
        authorizedToPrint(aUser, aJob);
        return aJob;
    }
    
    void Spooler::findAvailablePrinter(User *aUser, Job *aJob)
    {
        for(Printer *aPrinter : printers)
        {
            if(aPrinter->isAvailable())
            {
                schedule(aUser, aJob, aPrinter);
                return;
            }
        }
    }
    
    void Spooler::authorizedToPrint(User *aUser, Job *aJob)
    {
        if(aUser->getQuota() > aJob->getDocument()->getTotalPages())
            findAvailablePrinter(aUser, aJob);
    }
    
    void Spooler::schedule(User *aUser, Job *aJob, Printer *aPrinter)
    {
        aPrinter->print(aJob);
        // attendre que la tâche soit terminée
        if (aJob->getState() == EJS_COMPLETED)
            bill(aJob, aUser);
    }
    
    void Spooler::bill(Job *aJob, User *aUser)
    {
        aUser->decreaseQuota(aJob->getDocument()->getTotalPages());
    }
    
    void Spooler::run()
    {
        while(!socket.eof())
        {
            cups::User *client = new cups::User(
                this, std::move(socket.accept())
            );
            client->start();
        }
    }
}
