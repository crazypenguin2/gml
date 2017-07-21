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
	timer(uint32_t freq=1000);

	~timer() {}

    void set_frequency(uint32_t freq);

    uint32_t get_frequency()
    {
        return SystemCoreClock / (TIM_GetCounter(Timer) * (TIM_GetPrescaler(Timer) + 1));
    }

	template<channels ch>
	class oc
	{
	public:
		oc(ratio duty_cycle);
		~oc() {}
		void set_duty(ratio duty_cycle);

	private:
		class gpio
		{
		public:
			gpio()
			{
				GPIO_InitTypeDef GPIO_InitStructure{GPIO_to_device{Timer, ch}, GPIO_Speed_50MHz, GPIO_Mode_AF_PP};
				GPIO_Init(GPIO_to_device{Timer, ch}, &GPIO_InitStructure);
			}

			~gpio()	{}
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

				~rcc() {}

			} rcc_instance;
		} gpio_instance;
	};

	template<channels ch>
	class ic
	{
	public:
		ic();
		~ic() {}

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

#include "timer_impl.hpp"

#endif /* GML_INC_TIMER_HPP_ */
