#pragma once

#include "../sync/stream.h"
#include "../network/socket.h"
#include <vector>

namespace cups
{
    class User;
    class Job;
    class Document;
    class Printer;
    
    class Spooler : public syncsys::stream<Job*>
    {
        public:
            Spooler(netsys::Address &&);
            
            void join(Printer *aPrinter);
            void quit(Printer *aPrinter);
            
            Job* print(Document *aDocument, User *aUser);
            
            void run();
        protected:
            void findAvailablePrinter(User *aUser, Job *aJob);
        private:
            void authorizedToPrint(User *aUser, Job *aJob);
            void schedule(User *aUser, Job *aJob, Printer *aPrinter);
            void bill(Job *aJob, User *aUser);
        protected:
            netsys::TCPSocket socket;
            std::vector<Printer*> printers;
    };
}
