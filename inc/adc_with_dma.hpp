/*
 * adc_with_dma.hpp
 *
 *  Created on: Feb 10, 2016
 *      Author: penguin
 */

#ifndef GML_INC_ADC_WITH_DMA_HPP_
#define GML_INC_ADC_WITH_DMA_HPP_

#include <adc.hpp>
#include <dma.hpp>

template <std::size_t num_of_measures>
class adc_with_dma
{
public:
	adc_with_dma(std::array<adc_inputs, num_of_measures> inputs1, std::array<adc_inputs, num_of_measures> inputs2, std::function<void(int, int)> callback)
	: __adc{inputs1, inputs2}, __dma{callback}
	{
		ADC_DMACmd(ADC1, ENABLE);
	}

	~adc_with_dma()
	{

	}

private:
	adc_dual<num_of_measures> __adc;
	dma<num_of_measures> __dma;

	//std::function<void(int, int)> __event;
};



#endif /* GML_INC_ADC_WITH_DMA_HPP_ */
