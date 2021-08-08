#pragma once
struct RunableException : public exception
{
  const char * what () const throw ()
  {
    return "pointer parameter runable can't be nullptr";
  }
};
class Runable
{
public:
    virtual ~Runable(){};
    virtual void run()=0;
};