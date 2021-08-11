#ifndef RUNABLE_H
#define RUNABLE_H
#include <memory>
#include <mutex>
#include <exception>
#include <iostream>
#include <stdio.h>
struct RunableException : public std::exception
{
    const char* what() const throw ()
    {
        return "pointer parameter runable can't be nullptr";
    }
};
class Runable
{
public:
    Runable()=default;
    Runable(Runable* runable):_sp(runable){}
    virtual ~Runable(){}
    virtual void run()=0;

    inline std::shared_ptr<Runable>& sp(){
        return _sp;
    }
private:
    std::shared_ptr<Runable> _sp;
    std::mutex _mtx;
};

#endif
