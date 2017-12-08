#ifndef GML_INC_TIMER_IMPL_HPP_
#define GML_INC_TIMER_IMPL_HPP_

#include "gpio.hpp"

template <TIMS Timer_id>
timer<Timer_id>::timer(uint32_t freq/*=1000*/)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    /* Compute the prescaler value */
    uint32_t full_period = SystemCoreClock / freq;

    //auto PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
    uint16_t prescaler = full_period / 0xFFFF;
    period = full_period / (prescaler + 1);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(Timer, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(Timer, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(Timer, ENABLE);
}

template <TIMS Timer_id>
void timer<Timer_id>::set_frequency(uint32_t freq)
{
    uint32_t full_period = SystemCoreClock / freq;
    uint16_t prescaler = full_period / 0xFFFF;
    period = full_period / (prescaler + 1);
    //FIXME: Critical section
    TIM_PrescalerConfig(Timer, prescaler, TIM_PSCReloadMode_Immediate);
    TIM_SetCounter(Timer, period);
}

template <TIMS Timer_id>
template <typename ch, typename N>
timer<Timer_id>::oc<ch, N>::oc(ratio duty_cycle)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure {TIM_OCMode_PWM1,
                                            TIM_OutputState_Enable,
                                            0,
                                            uint16_t(((uint32_t)period)*duty_cycle.num/duty_cycle.denom),
                                            TIM_OCPolarity_High };
    if(ch::get() == ch1)
    {
        TIM_OC1Init(get_timer_address(Timer_id), &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(get_timer_address(Timer_id), TIM_OCPreload_Enable);
    }
    else if (ch::get() == ch2)
    {
        TIM_OC2Init(get_timer_address(Timer_id), &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(get_timer_address(Timer_id), TIM_OCPreload_Enable);
    }
    else if (ch::get() == ch3)
    {
        TIM_OC3Init(get_timer_address(Timer_id), &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(get_timer_address(Timer_id), TIM_OCPreload_Enable);
    }
    else if (ch::get() == ch4)
    {
        TIM_OC4Init(get_timer_address(Timer_id), &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(get_timer_address(Timer_id), TIM_OCPreload_Enable);
    }
    GPIO_InitTypeDef GPIO_InitStructure {GPIO_to_device{get_timer_address(Timer_id), ch::get()},
                                         GPIO_Speed_50MHz, GPIO_Mode_AF_PP};
    GPIO_Init(GPIO_to_device{get_timer_address(Timer_id), ch::get()}, &GPIO_InitStructure);
}

template <TIMS Timer_id>
template <typename ch, typename N>
void timer<Timer_id>::oc<ch, N>::set_duty(ratio duty_cycle)
{
    TIM_TypeDef* Timer = get_timer_address(Timer_id);
    auto val = uint16_t(((uint32_t)period)*duty_cycle.num/duty_cycle.denom);
    if(ch::get() == ch1)
        TIM_SetCompare1(Timer, val);
    else if (ch::get() == ch2)
        TIM_SetCompare2(Timer, val);
    else if (ch::get() == ch3)
        TIM_SetCompare3(Timer, val);
    else if (ch::get() == ch4)
        TIM_SetCompare4(Timer, val);
}

template <TIMS Timer_id>
template <typename ch, typename N>
timer<Timer_id>::ic<ch, N>::ic()
{
    TIM_OCInitTypeDef  TIM_OCInitStructure {TIM_OCMode_PWM1,
                                            TIM_OutputState_Enable,
                                            0,
                                            0,
                                            TIM_OCPolarity_High };
    if(ch::get() == ch1)
    {
        //TIM_OC1Init(Timer, &TIM_OCInitStructure);
        //TIM_OC1PreloadConfig(Timer, TIM_OCPreload_Enable);
    }
    else if (ch::get() == ch2)
    {
        //TIM_OC2Init(Timer, &TIM_OCInitStructure);
        //TIM_OC2PreloadConfig(Timer, TIM_OCPreload_Enable);
    }
    else if (ch::get() == ch3)
    {
        //TIM_OC3Init(Timer, &TIM_OCInitStructure);
        //TIM_OC3PreloadConfig(Timer, TIM_OCPreload_Enable);
    }
    else if (ch::get() == ch4)
    {
        //TIM_OC4Init(Timer, &TIM_OCInitStructure);
        //TIM_OC4PreloadConfig(Timer, TIM_OCPreload_Enable);
    }
    //GPIO_InitTypeDef GPIO_InitStructure{GPIO_to_device{Timer, ch}, GPIO_Speed_50MHz, GPIO_Mode_AF_PP};
    //GPIO_Init(GPIO_to_device{Timer, ch}, &GPIO_InitStructure);
}

#endif //GML_INC_TIMER_IMPL_HPP_
