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
#include "ThreadGroup.h"
#include <ctime>
#include <chrono>
#include <time.h>
#include<iomanip>
#include "TimeTool.h"

using namespace  std;
using namespace std::chrono;


class Thread : public ThreadGroup
{
public:
    Thread()=delete;
    Thread(Runable* target=nullptr,const char* name="def_thread"):ThreadGroup(name), _name(name),
    _sp(target->sp())
    {
        cout << "[ Thread: ]:  create "<< _name << "..." << endl;
    }
    ~Thread(){
        terminate();
        cout << "[ ~Thread: ]: destroy "<< _name << "..." <<" [ sp ]: " << _sp.use_count() << "\n"<< endl;
        if(_sp.use_count()==2){
            _sp.~shared_ptr();
        }
        //this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    void run() override
    {
        _sp->run();
    }

    void start(){
        std::lock_guard<std::mutex> lck(_mtx);
        add();
    }
    const char* get_name() const{
        return _name;
    }

private:
    
    const char* _name;
    std::shared_ptr<Runable> _sp;
    std::condition_variable _cv;
    std::mutex _mtx;
};

#endif