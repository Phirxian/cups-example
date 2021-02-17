#pragma once

namespace cups
{
    enum E_JOB_STATE
    {
        EJS_WAITING,
        EJS_PRINTING,
        EJS_COMPLETED,
        EJS_SUSPEND,
        EJS_CANCELED,
    };
    enum E_ENABLED_SIDES
    {
        EES_RECTO,
        EES_VFERSO,
        EES_COUNT
    };
    
    class User;
    class Document;
    class Printer;
    
    class Job
    {
        public:
            Job(User *aUser, Document *aDocument, E_ENABLED_SIDES si = EES_RECTO) noexcept
                : user(aUser), printer(nullptr), document(aDocument), count(1), side(si), state(EJS_WAITING)
            {
            }
            
            ~Job()
            {
            }

            int getState() const noexcept
            {
                return state;
            }

            void setState(int s) noexcept
            {
                state = s;
            }
            
            Printer* getPrinter() noexcept
            {
                return printer;
            }

            void setPrinter(Printer *aPrinter) noexcept
            {
                printer = aPrinter;
            }

            Document* getDocument() noexcept
            {
                return document;
            }

            void setDocument(Document *document) noexcept
            {
                document = document;
            }
        protected:
            User *user;
            Printer *printer;
            Document *document;
            E_ENABLED_SIDES side;
            int count, state;
        private:
    };
}
