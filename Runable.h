#ifndef RUNABLE_H
#define RUNABLE_H

#include <memory>
#include <exception>
#include <iostream>
#include <stdio.h>

class Runable
{
public:
    Runable()=default;
    Runable(Runable* runable):_sp(runable)
    {
       
    }
    virtual ~Runable(){
        std::cout << "\n--------------------------------[ ~Runable ]:" << _sp.use_count() << "\n"<<std::endl;
    }
    virtual void run()=0;
    std::shared_ptr<Runable> _sp;
};

#endif
