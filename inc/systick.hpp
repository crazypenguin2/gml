#ifndef __SYSTICK_HPP_
#define __SYSTICK_HPP_

#include "stm32f10x.h"
#include "isr.hpp"
#include <functional>

class systick
{
public:
    systick(std::function<void(void)> f)
        : callback(f)
    {
        Interrupts::Manager::registerISR(Interrupts::SysTick_Handler,
                std::bind(&systick::SysTick_Handler,this,std::placeholders::_1));
        SysTick_Config(SystemCoreClock / 1000);
    }

private:
    std::function<void(void)> callback;

    void SysTick_Handler(int)
    {
        ADC_Cmd(ADC1, ENABLE);
        callback();
    }
};

#endif //__SYSTICK_HPP_
