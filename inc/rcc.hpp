/*
 * rcc.hpp
 *
 *  Created on: Feb 1, 2016
 *      Author: penguin
 */

#ifndef GML_INC_RCC_HPP_
#define GML_INC_RCC_HPP_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

namespace rcc
{

template<TIM_TypeDef * device>
void enable()
{
	if(device == TIM3)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	else if(device == TIM2)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
};



} //namespace rcc

#endif /* GML_INC_RCC_HPP_ */
