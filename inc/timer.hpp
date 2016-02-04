/*
 * timer.hpp
 *
 *  Created on: Jan 8, 2016
 *      Author: penguin
 */

/*
 * Before edit, please read:
 * http://stackoverflow.com/questions/16575534/non-type-template-parameters-based-on-hardware-registers
 */

#ifndef GML_INC_TIMER_HPP_
#define GML_INC_TIMER_HPP_

#include <functional>
#include <ratio>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "isr.hpp"
#include "gpio.hpp"
//#include "rcc.hpp"

/*using ch1 = channels::ch1;
using ch2 = channels::ch2;
using ch3 = channels::ch3;
using ch4 = channels::ch4;*/

class ratio
{
public:
	uint16_t num;
	uint16_t denom;
};

enum TIMS { tim1, tim2, tim3 };

constexpr TIM_TypeDef* get_timer_address(TIMS tim)
{
	switch(tim)
	{
		case tim1: return TIM1;
		case tim2: return TIM2;
		case tim3: return TIM3;
		default:
			//static_assert(tim && false, "unknown timer identifier");
			return nullptr;
	}
}

template <TIMS Timer_id>
class timer
{
static constexpr auto Timer = get_timer_address(Timer_id);
public:
	timer(uint32_t freq=1000)
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

	~timer()
	{

	}

	template<channels ch>
	class oc
	{
	public:
		oc(ratio duty_cycle)
		{
			TIM_OCInitTypeDef  TIM_OCInitStructure{
						TIM_OCMode_PWM1,
						TIM_OutputState_Enable,
						0,
						uint16_t(((uint32_t)period)*duty_cycle.num/duty_cycle.denom),
						TIM_OCPolarity_High };
			if(ch == ch1)
			{
				TIM_OC1Init(Timer, &TIM_OCInitStructure);
				TIM_OC1PreloadConfig(Timer, TIM_OCPreload_Enable);
			}
			else if (ch == ch2)
			{
				TIM_OC2Init(Timer, &TIM_OCInitStructure);
				TIM_OC2PreloadConfig(Timer, TIM_OCPreload_Enable);
			}
			else if (ch == ch3)
			{
				TIM_OC3Init(Timer, &TIM_OCInitStructure);
				TIM_OC3PreloadConfig(Timer, TIM_OCPreload_Enable);
			}
			else if (ch == ch4)
			{
				TIM_OC4Init(Timer, &TIM_OCInitStructure);
				TIM_OC4PreloadConfig(Timer, TIM_OCPreload_Enable);
			}
			GPIO_InitTypeDef GPIO_InitStructure{GPIO_to_device{Timer, ch}, GPIO_Speed_50MHz, GPIO_Mode_AF_PP};
			GPIO_Init(GPIO_to_device{Timer, ch}, &GPIO_InitStructure);
		}

		~oc()
		{

		}

		void set_duty(ratio duty_cycle)
		{
			auto val = uint16_t(((uint32_t)period)*duty_cycle.num/duty_cycle.denom);
			if(ch == ch1)
				TIM_SetCompare1(Timer, val);
			else if (ch == ch2)
				TIM_SetCompare2(Timer, val);
			else if (ch == ch3)
				TIM_SetCompare3(Timer, val);
			else if (ch == ch4)
				TIM_SetCompare4(Timer, val);
		}

	private:
		class gpio
		{
		public:
			gpio()
			{
				GPIO_InitTypeDef GPIO_InitStructure{GPIO_to_device{Timer, ch}, GPIO_Speed_50MHz, GPIO_Mode_AF_PP};
				GPIO_Init(GPIO_to_device{Timer, ch}, &GPIO_InitStructure);
			}

			~gpio()
			{

			}
		private:
			class rcc
			{
			public:
				rcc()
				{
					/* GPIOA and GPIOB clock enable */
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
						                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
				}

				~rcc()
				{

				}

			} rcc_instance;
		} gpio_instance;
	};

private:
	class clock
	{
	public:
		clock()
		{
			/* TIM3 clock enable */
			//rcc::enable<Timer>();
			if(Timer_id == tim3)
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			else if(Timer_id == tim2)
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		}

		~clock()
		{

		}
	} clock_instance;


	//rcc<Timer> rcc_instance;



	class nvic
	{
	public:
		nvic()
		{

		}

		~nvic()
		{

		}
	} nvic_instance;

	static constexpr uint16_t CCR1_Val = 333;
	static constexpr uint16_t CCR2_Val = 249;
	static constexpr uint16_t CCR3_Val = 166;
	static constexpr uint16_t CCR4_Val = 83;

	static uint16_t period;
};

template <TIMS Timer_id>
uint16_t timer<Timer_id>::period = 0;


#endif /* GML_INC_TIMER_HPP_ */
