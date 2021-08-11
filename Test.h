#ifndef __TEST_H
#define __TEST_H
#include <string>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
class Test
{
public:
    Test(){
        cout << "default construct" << endl;
    }
    ~Test(){
        cout << "deconstruct" << endl;
    }
    void test(){
        cout << "a:" << Test::a << endl;
    }
    void run(){
        int i=10;
        while(--i){
             cout << "i:" << i << "\n" <<endl;
               //  std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
  std::string _s;
    int _id;
//private:
    static int a;
};

#endif