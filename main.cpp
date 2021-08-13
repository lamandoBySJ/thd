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
}