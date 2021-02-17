#pragma once

#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>

namespace syncsys
{
    void* process_run(void*);
    
    template<typename T>
    class consumer;
    template<typename T>
    class producer;

    template<typename T>
    class stream
    {
        public:
            using type = T;
        public:
            stream(int s) noexcept
                : data(new T[s]), used(0), size(s),
                  pcurrent(0), ccurrent(0), ri(0), wi(0),
                  mprod(PTHREAD_MUTEX_INITIALIZER),
                  mcons(PTHREAD_MUTEX_INITIALIZER)
            {
                memset(data, 0, s);
                pthread_mutex_init(&mprod, nullptr);
                pthread_mutex_init(&mcons, nullptr);
                sem_init(&event, 0, 0);
                sem_init(&rwlock, 0, 1);
                sem_init(&step, 0, 0);
                sem_init(&step2, 0, 1);
                sem_init(&sync, 0, 0);
            }
            
            ~stream() noexcept
            {
                for(auto &it : prod)
                    delete it;
                for(auto &it : cons)
                    delete it;
                sem_destroy(&event);
                sem_destroy(&rwlock);
                sem_destroy(&step);
                sem_destroy(&step2);
                sem_destroy(&sync);
                pthread_mutex_destroy(&mprod);
                pthread_mutex_destroy(&mcons);
            }
            
            void attach(producer<T> *p) noexcept
            {
                pthread_mutex_lock(&mprod);
                prod.push_back(p);
                pthread_mutex_unlock(&mprod);
            }
            
            void remove(producer<T> *p) noexcept
            {
                pthread_mutex_lock(&mprod);
                if(pcurrent == prod.size()-1)
                    pcurrent = prod.size()-2;
                for(auto it = prod.begin(); it != prod.end(); ++it)
                    if(p == *it)
                        prod.erase(it);
                pthread_mutex_unlock(&mprod);
            }
            
            void attach(consumer<T> *p) noexcept
            {
                pthread_mutex_lock(&mcons);
                cons.push_back(p);
                pthread_mutex_unlock(&mcons);
            }
            
            void remove(consumer<T> *p) noexcept
            {
                pthread_mutex_lock(&mcons);
                if(ccurrent == cons.size()-1)
                    ccurrent = cons.size()-2;
                for(auto it = cons.begin(); it != cons.end(); ++it)
                    if(p == *it)
                        cons.erase(it);
                pthread_mutex_unlock(&mcons);
            }
            
            void notifyProd() noexcept
            {
                sem_post(&step);
                sem_post(&sync);
            }
            
            void notifyCons() noexcept
            {
                sem_post(&step2);
                sem_post(&sync);
            }
            
            void operator << (const T &d) noexcept
            {
                sem_wait(&rwlock);
                wi = (wi+1)%size;
                used++;
                data[wi] = d;
                sem_post(&rwlock);
                sem_post(&event);
            }
            
            void operator >> (T &i) noexcept
            {
                sem_wait(&event);
                sem_wait(&rwlock);
                ri = (ri+1)%size;
                i = data[ri];
                used--;
                sem_post(&rwlock);
                
                nextProd();
            }
            
            void run() noexcept
            {
                prod[0]->notify();
                cons[0]->notify();
                
                for(;;)
                {
                    sem_wait(&sync);
                    nextCons();
                }
            }
        protected:
            // used inside when rwlock is aquired
            void nextProd() noexcept
            {
                // lorsque @step est locker alors le thread courrant a fini sont segment
                // donc si lock et si @data est vide => thread suivant
                if(sem_trywait(&step) && used == 0)
                {
                    pthread_mutex_lock(&mprod);
                    pcurrent = (pcurrent+1)%prod.size();
                    prod[pcurrent]->notify();
                    pthread_mutex_unlock(&mprod);
                }
            }
            // used inside when rwlock is aquired
            void nextCons() noexcept
            {
                // lorsque @step2 est locker alors le thread courrant a fini sont segment
                // donc si lock et si @data est vide => thread suivant
                if(sem_trywait(&step2) && used == cons[(ccurrent+1)%cons.size()]->count);
                {
                    pthread_mutex_lock(&mcons);
                    ccurrent = (ccurrent+1)%cons.size();
                    cons[ccurrent]->notify();
                    pthread_mutex_unlock(&mcons);
                }
            }
        protected:
            std::vector<consumer<T>*> cons;
            std::vector<producer<T>*> prod;
            //! mutex to protect prod & cons access -> better performance in this case
            pthread_mutex_t mprod, mcons;
            //! rwlock = mutex pour @used et @data
            //! event = pour notifier l'arrive d'un item a consomer (pas d'attente active)
            //! step = pour notifier la fin d'un segment au stream
            sem_t event, rwlock, step, step2, sync;
            //! pcurrent = thread courrent ayant la main sur le stream
            //! ri/wi index ecriture/lecture et used le nombre d'élément présent
            int ri, wi, pcurrent, ccurrent, used, size;
            T *data;
    };
}