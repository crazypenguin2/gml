/*
 * gpio.hpp
 *
 *  Created on: Jan 17, 2016
 *      Author: penguin
 */

#ifndef GML_INC_GPIO_HPP_
#define GML_INC_GPIO_HPP_

#include <algorithm>
#include <tuple>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

enum channels {ch1, ch2, ch3, ch4};
/*using ch1 = channels::ch1;
using ch2 = channels::ch2;
using ch3 = channels::ch3;
using ch4 = channels::ch4;*/
enum records {device, channel, port, pin};

constexpr auto timer_to_pin_table = {
		std::make_tuple(TIM2, ch1, GPIOA, GPIO_Pin_0),
		std::make_tuple(TIM2, ch2, GPIOA, GPIO_Pin_1),
		std::make_tuple(TIM2, ch3, GPIOA, GPIO_Pin_2),
		std::make_tuple(TIM2, ch4, GPIOA, GPIO_Pin_3),
		std::make_tuple(TIM3, ch1, GPIOA, GPIO_Pin_6),
		std::make_tuple(TIM3, ch2, GPIOA, GPIO_Pin_7),
		std::make_tuple(TIM3, ch3, GPIOB, GPIO_Pin_0),
		std::make_tuple(TIM3, ch4, GPIOB, GPIO_Pin_1)};

//using gpio_pair = std::tuple<GPIO_TypeDef*, uint16_t>;

class GPIO_to_device
{
public:
	//template<D* device, GPIO_TypeDef * port, uint16_t pin> GPIO
	constexpr GPIO_to_device(TIM_TypeDef* device, GPIO_TypeDef * port, uint16_t pin)
	{
		for(auto iter : timer_to_pin_table)
		{
			if(		std::get<::device>(iter) == device
				&&	std::get<::port>(iter) == port
				&&	std::get<::pin>(iter) == pin)
			{
				__gpio = iter;
			}
		}
	}

	constexpr GPIO_to_device(TIM_TypeDef* device, channels ch)
	{
		for(auto iter : timer_to_pin_table)
		{
			if(		std::get<::device>(iter) == device
				&&	std::get<::channel>(iter) == ch)
			{
				__gpio = iter;
			}
		}
	}

	/*constexpr operator channel()
	{
		return std::get<::channel>(__gpio);
	}*/

	constexpr operator GPIO_TypeDef*()
	{
		return std::get<::port>(__gpio);
	}

	constexpr operator uint16_t()
	{
		return std::get<::pin>(__gpio);
	}

private:
	decltype(std::make_tuple(TIM3, ch1, GPIOA, GPIO_Pin_6)) __gpio; //FIXME: it's a bull shit, but works.
};


#endif /* GML_INC_GPIO_HPP_ */
