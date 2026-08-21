#pragma once

#include "cpplib.hpp"
#include "stdint.h"
extern "C"
{
#include "klib.h"
#include "lib.h"
}

#define keyboard_type 1

namespace ker_driver
{

using DriverCall = void(*)(uint32_t,kList *);

class driver_interface
{
public:
    virtual uint32_t get_type() = 0;
};

class int_driver_interface : driver_interface
{
public :
    virtual uint32_t add_call_func(DriverCall func) = 0;
    virtual _OS_API void delete_call_func(uint32_t node) = 0;
};

class driver_keyboard : int_driver_interface
{
public :
    uint32_t add_call_func(DriverCall func);
    _OS_API void delete_call_func(uint32_t node);
    static driver_keyboard *getDriver();
    void irq_handler();
    uint32_t get_type();
private:
    driver_keyboard();
    ker_lib::qdeque<uint8_t> *key_buff;
};

}


