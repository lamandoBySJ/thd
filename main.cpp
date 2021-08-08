#include <string>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>
#include "Test.h"
#include "Runable.h"

using namespace std;
__attribute((constructor)) void before_main()  
{  
    printf("main:%s/n",__FUNCTION__);  
}  
  
__attribute((destructor)) void after_main()  
{  
    printf("main:%s/n",__FUNCTION__);  
}
class Task:public Runable
{
public:
    Task(const char* name="def"):_name(name)
    {

    }
~Task(){
    cout<<_name << "\n[ ~Task ]\n" << endl;
    this_thread::sleep_for(std::chrono::seconds(1));
}
    void run() override
    {
        cout << ">>>>>>>>>>>>> "<< _name <<" Thread::run 1\n" << endl;
        this_thread::sleep_for(std::chrono::seconds(1));
        cout << ">>>>>>>>>>>>> "<< _name <<" Thread::run 2\n" << endl;
        this_thread::sleep_for(std::chrono::seconds(1));
        cout << ">>>>>>>>>>>>> "<< _name <<" Thread::run 3\n" << endl;
        this_thread::sleep_for(std::chrono::seconds(1));
    }
private:
    std::string _name;
};
enum class State : char{
        NEW,
        RUNNABLE,
        BLOCKED,
        WAITING,
        TIMED_WAITING,
        TERMINATED
};
class Thread:public Runable
{
public:
    
public:
    Thread():_thread(&Thread::run,this),_finished(false){
        _state=State::NEW;
    }
    Thread(Runable* runable):_thread(&Thread::run,this),_finished(false){
        _state=State::NEW;
    }
    ~Thread(){
        stop();
        
        for(auto& runable : _targets){
                if(runable!=nullptr){
                    delete runable;
                    runable=nullptr;
                    cout << "\n--xxxxxxxxxxxxxxxxxxxxx--\n" << endl; 
                }
        }
    }
    void run() override
    {
        cout << ">>>>>>>>>>>>>Thread::run crested\nwaiting startup...\n" << endl;
        while(!_finished){
            std::unique_lock<mutex> lck(_mtx);
            while(_targets.empty()){
                _cv.wait(lck);
            }
           Runable* pTask = _targets.front();
           
           _targets.pop_front();
            if(pTask==nullptr){
                continue;
            }
            cout << ">>>>>>>>>>>>>Thread::run start\n" << endl;
            pTask->run();
            cout << ">>>>>>>>>>>>>Thread::run finish\n" << endl;
            delete pTask;
            pTask=nullptr;
        }  
    }
    void start(Runable* target=nullptr){
            if(_finished){
                if(target!=nullptr){
                    delete target;
                    target=nullptr;
                }
                return;
            }
            cout << "\n--try...........--\n" << endl; 
            {
                std::lock_guard<std::mutex> lck(_mtx); 
                _targets.push_back(target);
            }
            _cv.notify_one(); 
            cout << "\n--sssssssssssssssssssssssssssssssssss--\n" << endl;            
       
    }
    void stop(){
        if(_finished){
            return;
        }else{
            _finished=true; 
        } 
        {
                std::lock_guard<std::mutex> lck(_mtx); 
                _targets.push_front(nullptr);
        } 
        if(_thread.joinable()){          
            _state=State::TERMINATED;
            _cv.notify_one();
            _thread.join();
        }
    }
private:
    std::thread _thread;
    std::list<Runable*> _targets;
   // std::shared_ptr<Runable> _sp;
    //std::atomic<bool> _blocked;
    std::atomic<bool> _finished;
    std::mutex _mtx;
    std::condition_variable _cv;
    std::atomic<State> _state;
    //State _state;
};
Test t;
Thread thd1;
void run(){
    /*Runable* r=new Task;
    Thread thd1(r);
    Thread thd2(r);
    Thread thd3(r);
    //thd.start(new Task);
    thd1.start();
    thd2.start();
    thd2.start();
    */
Runable* r=new Task;
Runable* r1=new Task("Aaaaaaaaaa");
Runable* r2=new Task("BBbBBBbbbB");

thd1.start(r);
this_thread::sleep_for(std::chrono::seconds(1));
thd1.start(r1);
//thd1.start(r2);
thd1.stop();
this_thread::sleep_for(std::chrono::seconds(2));
thd1.start(r2);
//this_thread::sleep_for(std::chrono::seconds(2));
}

int main()
{

    run();
  
   //thd.start(new Task);
this_thread::sleep_for(std::chrono::seconds(10));
   



    return 0;
}