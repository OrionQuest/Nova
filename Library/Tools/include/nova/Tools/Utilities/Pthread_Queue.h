//!#####################################################################
//! \file Pthread_Queue.h
//!#####################################################################
// Class Pthread_Queue
//######################################################################
#ifndef __Pthread_Queue__
#define __Pthread_Queue__

#include <nova/Tools/Utilities/Pointer_Queue.h>
#include <pthread.h>
#include <iostream>

class Pthread_Queue
{
    const int number_of_threads;
    pthread_t* const threads;
    pthread_attr_t attr;
    pthread_mutex_t queue_lock;
    pthread_cond_t done_condition,todo_condition;
    int active_threads,inactive_threads;

  public:
    struct Task
    {
        virtual ~Task(){};
        virtual void Run()=0;
    };

    struct Exiter: public Task
    {
        void Run()
        {pthread_exit(0);}
    };
    
  private:
    Pointer_Queue queue;

  public:
    Pthread_Queue(const int thread_count)
        :number_of_threads(thread_count),threads(new pthread_t[thread_count]),active_threads(thread_count),inactive_threads(0),queue(65535)
    {
        pthread_attr_init(&attr);
        pthread_cond_init(&done_condition,0);
        pthread_cond_init(&todo_condition,0);
        pthread_mutex_init(&queue_lock,0);

        for(int i=0;i<number_of_threads;i++) pthread_create(&threads[i],0,Thread_Routine,this);
    }

    ~Pthread_Queue()
    {
        for(int i=0;i<number_of_threads;i++) Queue(new Exiter());
        pthread_cond_destroy(&done_condition);
        pthread_cond_destroy(&todo_condition);
        pthread_mutex_destroy(&queue_lock);
        pthread_attr_destroy(&attr);
        delete[] threads;
    }

    void Queue(Task* task)
    {
        pthread_mutex_lock(&queue_lock);
        if(inactive_threads) pthread_cond_signal(&todo_condition);
        queue.Enqueue(task);
        pthread_mutex_unlock(&queue_lock);
    }

    static void* Thread_Routine(void* data)
    {
        Pthread_Queue& queue=*(Pthread_Queue*)data;
        while(1){
            pthread_mutex_lock(&queue.queue_lock);
            while(queue.queue.Empty()){
                queue.active_threads--;
                if(queue.active_threads==0) pthread_cond_signal(&queue.done_condition);
                queue.inactive_threads++;
                pthread_cond_wait(&queue.todo_condition,&queue.queue_lock);
                queue.active_threads++;queue.inactive_threads--;}
            Task* work=(Task*)queue.queue.Dequeue();
            pthread_mutex_unlock(&queue.queue_lock);
            work->Run();
            delete work;}
        return 0;
    }

    void Wait()
    {
        pthread_mutex_lock(&queue_lock);
        while(!queue.Empty() || active_threads!=0) pthread_cond_wait(&done_condition,&queue_lock);
        pthread_mutex_unlock(&queue_lock);
    }
};
#endif
