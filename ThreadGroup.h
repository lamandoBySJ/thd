#pragma once

#include "Thread.h"
#include <mutex>
#include <queue>
enum class State : int{
        NEW,
        RUNNABLE,
        BLOCKED,
        WAITING,
        TIMED_WAITING,
        TERMINATED
};
class ThreadGroup
{
public:
    ThreadGroup(const char* name="def_thread_group"):_name(name)
    {
        for(int i=0;i<4;i++){
            _threads.emplace_back(std::shared_ptr<std::thread>(new std::thread(&ThreadGroup::run_thread_group,this)));
            ++_nthreads;
        }
    }
    virtual  ~ThreadGroup(){
       
    }
    void run_thread_group(){
       
        while(!_finished){
            {
                std::unique_lock<std::mutex> lck{_mtx};
                _cv.wait(lck,[this]{return _finished || !_tasks.empty();});
                if( _finished || _tasks.empty()){
                    return;
                }
                _tasks.pop();
            }
                
            this->run();
            
            std::unique_lock<std::mutex> lck{_mtx};
            --_nthreads;
            
        }
    }
    virtual void run()=0;
    void add(){
        {
            std::lock_guard<std::mutex> lck(_mtx_thread);
            if(_nthreads==_threads.size()){
                _threads.emplace_back(std::shared_ptr<std::thread>(new std::thread(&ThreadGroup::run_thread_group,this)));
                ++_nthreads;
            }
            
        }
        {
            std::lock_guard<std::mutex> lck(_mtx);
            _tasks.emplace(1);
        }
        _cv.notify_one(); 
    }
    void terminate()
    {
         _finished=true;
         std::lock_guard<std::mutex> lck(_mtx_thread);
         for(auto& thd:_threads){
             if(thd->joinable()){
                _cv.notify_all();
                thd->join(); 
             }
         }
    }
    //Runable* _target;
    const char* _name;
    //std::atomic<State> _state{State::WAITING};
    std::atomic<int> _nthreads{0};
    std::atomic<int> _nUnstartedThreads{0};
    std::list<std::shared_ptr<std::thread>> _threads;
    std::condition_variable _cv;
private:
    std::mutex _mtx;
     std::mutex _mtx_thread;
    std::atomic<bool> _finished{false};
    //State _state{State::WAITING};
    std::queue<int> _tasks; 
};