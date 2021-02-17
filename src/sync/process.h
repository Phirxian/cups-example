#pragma once
#include "stream.h"

namespace syncsys
{
    void* process_run(void*);
    
    class process
    {
        public:
            process()
            {
                sem_init(&sem, 0, 0);
            }
            virtual ~process()
            { 
                sem_destroy(&sem);
                cancel();
            }
        
            void start() { pthread_create(&thread, nullptr, syncsys::process_run, (void*)this); }
            void cancel(){ pthread_cancel(thread); }
            void join()  { pthread_join(thread, nullptr); }
            void notify(){ sem_post(&sem); }
            
            virtual void run() = 0;
        protected:
            pthread_t thread;
            sem_t sem;
    };

    template<typename T>
    class producer : public process
    {
        public:
            producer(stream<T> *_s) : process(), s(_s) {}
            virtual void run() = 0;
        protected:
            stream<T> *s;
    };

    template<typename T>
    class consumer : public process
    {
        public:
            consumer(stream<T> *_s) : process(), s(_s) {}
            virtual void run() = 0;
        protected:
            stream<T> *s;
    };
}