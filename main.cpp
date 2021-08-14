#include "Thread.h"
#include "Task.h"

int main(){
    Task* task=new Task();
    Thread thd(task);
    thd.start();
    thd.start();
    thd.start();
    thd.start();
    thd.start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
}