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
    //this_thread::sleep_for(std::chrono::seconds(1));
    }
    void run() override
    {
        _mtx.lock();
        static std::atomic<int> i(30);
        cout << "\n---" << "Run Task: "<< _name << " ,i: " << --i << "---\n"<<endl;
        _mtx.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

private:
    std::string _name;
    std::mutex _mtx;
};
class Thread//:public Runable
{
public:
    
public:

    Thread(Runable* runable, const char* name="def_thread"):
        _sp(runable->_sp),
        _name(name),
        _thread(&Thread::run,this),
        _finished(false)
    {
        _state=State::NEW;
        
       // if(_runable->_sp.use_count()>0){
            //_sp.reset();
       // }
       // std::cout<<"[2]Thread::_sp.use_count():" <<_sp.use_count() << "\n" << endl;
    }
    ~Thread(){
        cout << "\n[---]~Thread:" << _sp.use_count() << "\n"<<endl;
        if(_thread.joinable()){
            _thread.join();
        }

        if( _sp.use_count()==2){
            Runable* _runable=_sp.get();
            _sp.reset();
            if(_runable){
                delete _runable;
                _runable=nullptr;
            }
        }   
    }
    void run() //override
    {
        
        //auto p= Runable::_sp.lock();
    
            //std::cout<<"\n[>>>]Thread::run:" <<_sp.lock()->_sp.use_count() << "\n" << endl;
          // _sp.lock()->run();
        _sp->run();
        
     
         //  _runable->run();
           // _sp.lock()->run();
           // std::cout<<"[T]Thread::run NO:" <<_runable->_sp.use_count() << "\n" << endl;
    
       // _runable->run();
        /*while(!_finished){
            std::unique_lock<std::mutex> lck(_mtx);
            //while(_targets.empty()){
            //    _cv.wait(lck);
           // }
          // Runable* pTask = _targets.front();
           
           _targets.pop_front();
           
            cout << ">>>>>>>>>>>>>Thread::run start\n" << endl;
      
            cout << ">>>>>>>>>>>>>Thread::run finish\n" << endl;
    
   
        }  */
    }
    void start(){
            if(_finished){
                return;
            }
            //_threadGroup.add(this);
            cout << "\n--try...........--\n" << endl; 
            {
               // std::lock_guard<std::mutex> lck(_mtx); 
               // _targets.push_back(target);
            }
           // _cv.notify_one(); 
            cout << "\n--sssssssssssssssssssssssssssssssssss--\n" << endl;            
       
    }
    void stop(){
        
        {
                //std::lock_guard<std::mutex> lck(_mtx); 
                //_targets.push_front(nullptr);
        } 

    }
private:
    std::thread _thread;
    const char* _name;
   // std::shared_ptr<Runable> _sp;
    //std::atomic<bool> _blocked;
    std::mutex _mtx;
    //std::condition_variable _cv;
    std::atomic<State> _state;
    //ThreadGroup _threadGroup;
    std::shared_ptr<Runable> _sp;
  
    std::atomic<bool> _finished;
};

#endif