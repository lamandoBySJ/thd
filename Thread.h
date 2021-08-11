#ifndef THREAD_H
#define THREAD_H
#include <iostream>
#include <stdio.h>
#include "Runable.h"
#include <thread>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <atomic>
#include <condition_variable>

using namespace  std;

enum class State : char{
        NEW,
        RUNNABLE,
        BLOCKED,
        WAITING,
        TIMED_WAITING,
        TERMINATED
};
class Task:public Runable
{
public:
    Task(const char* name="def"):Runable(this),_name(name)
    {

    }
~Task(){
    cout<< "\n[ ~Task ]: " << _name << endl;
    }
    void run() override
    {
        while(i>0){
           _mtx.lock();
            --i;
            cout << "\n---" << "Run Task: "<< _name << " ,i: " << --i << "---\n"<<endl;
            _mtx.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        cout << "\n---" << "Task: DONE---\n"<<endl;
    }

private:
    std::string _name;
    std::mutex _mtx;
    std::atomic<int> i{30};
};
class Thread
{
public:
    
public:
    /** @brief Test for non-letter.
    * Test the character @p ch in the global locale.
    * @param ch the character to test
    * @return true if @p ch is not a letter
    */
    Thread()=delete;
    Thread(Runable* runable=nullptr,const char* name="def_thread"):
        _sp(runable->sp()),
        _name(name),
        _thread(&Thread::run,this),
        _finished(false),
        _state(State::WAITING)
        {
            
        }
    ~Thread(){
        cout << "\n[---]~Thread:" << _sp.use_count() << "\n"<<endl;
        if(_thread.joinable()){
            _finished=true;
            {
                std::unique_lock<std::mutex> lck(_mtx);
                _state=State::TERMINATED;
            }
             _cv.notify_one();
            _thread.join();
        }
        if( _sp.use_count() == 2){
              _sp.~shared_ptr();
        }   
    }
    
    void run() 
    {
        std::unique_lock<std::mutex> lck(_mtx);
        while(!_finished){
            while(_state==State::WAITING){
                _cv.wait(lck);
            }
            if(_state==State::TERMINATED){
                continue;
            }else{
                _state=State::WAITING;
            }
            _sp->run();
        }
    }

    void start(){
            if(_finished){
                return;
            }
            {
                std::lock_guard<std::mutex> lck(_mtx); 
                _state=State::RUNNABLE;
            } 
            _cv.notify_one(); 
    }
 
private:
    std::shared_ptr<Runable> _sp;
    const char* _name;
    std::thread _thread;
    //std::shared_ptr<Runable> _sp;
    //Runable* _runable;
    std::atomic<bool> _finished;
    std::atomic<State> _state;
    std::mutex _mtx;
    std::condition_variable _cv;
};

#endif