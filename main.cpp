
#include <list>
#include <mutex>
#include <memory>


#include "Test.h"
#include "Runable.h"
#include "ThreadGroup.h"
#include "Thread.h"

using namespace std;


__attribute((constructor)) void before_main()  
{  
    printf("\n[ main:%s ]\n",__FUNCTION__);  
}  

__attribute__((destructor)) void after_main()  
{  
     printf("\n[ main:%s ]\n",__FUNCTION__);  
      this_thread::sleep_for(std::chrono::seconds(1));
}

//Thread thd1;
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
}

int main()
{
    Task* task=new Task;
    Thread thd1(task,"thread_A");
    Thread thd2(task,"thread_B");
    Thread thd3(task,"thread_C");
    Thread thd4(task,"thread_D");
    Thread thd5(task,"thread_E");
    Thread thd6(task,"thread_F");
    Thread thd7(task,"thread_G");
    Thread thd8(task,"thread_H");

    thd1.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd2.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd3.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd4.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd5.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd6.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd7.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd8.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
   //thd.start(new Task);

    this_thread::sleep_for(std::chrono::seconds(6));
    thd1.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd2.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd3.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd4.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd5.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd6.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd7.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    thd8.start();
    this_thread::sleep_for(std::chrono::milliseconds(100));
    return 0;
}