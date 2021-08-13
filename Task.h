#pragma once
#include "Thread.h"
#include "Runable.h"

class Task:public Runable
{
public:
    Task(const char* name="def"):Runable(this),_name(name)
    {
        _now = std::chrono::system_clock::now();
       // auto now = std::chrono::system_clock::now();
       // std::string tt=TimeTool::to_str_ex(TimeTool::now_to_ms());
        //std::string tt2=TimeTool::to_str_ex(TimeTool::to_ms(now));
       // std::string tt3=TimeTool::to_str(std::chrono::system_clock::to_time_t(now));
        //std::string tt4=TimeTool::to_str(now);
        //printf("%s\n", tt.c_str());
        //std::this_thread::sleep_for(std::chrono::seconds(3));
        
    
        //std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // std::time_t last = std::chrono::system_clock::to_time_t(tp_epoch);
    // time_t next= system_clock::to_time_t(now - std::chrono::hours(24));
    //std::cout << "One day ago, the time was "<< std::put_time(std::localtime(&last), "%F %T") << '\n';
    //　std::cout << "Next day, the time was "<< std::put_time(std::localtime(&next), "%F %T") << '\n';
    }
~Task(){
    cout<< "\n[ ~Task ]: destroy " << _name << "...\n"<<endl;
    }
    void run() override
    { /*
        _mtx.lock();
        _now = std::chrono::system_clock::now();
        _time=TimeTool::to_str_ex(TimeTool::now_to_ms());
        _mtx.unlock();
        cout << "Task: Begin Time:" << _time  << "\n"<< endl;
        */
        while(i>0){
            _mtx.lock();
            --i;
            cout << "---" << "Run Task: "<< _name << " ,i: " << i << "---\n"<<endl;
            _mtx.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }
         _mtx.lock();
        auto sec=TimeTool::elapse_seconds(_now);
        _time=TimeTool::to_str_ex(TimeTool::now_to_ms());
        _mtx.unlock();
        cout <<"Task: End Time:"<< _time <<" completed within "<< sec << " s\n"<<endl;

    }

private:
    std::string _name;
    std::mutex _mtx;
    std::atomic<int> i{10};
    std::chrono::_V2::system_clock::time_point _now;
    std::string _time;
   
};