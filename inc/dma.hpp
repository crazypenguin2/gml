/*
 * dma.hpp
 *
 *  Created on: Feb 8, 2016
 *      Author: penguin
 */

#ifndef GML_INC_DMA_HPP_
#define GML_INC_DMA_HPP_

#include "stm32f10x_conf.h"

template <std::size_t SIZE>
class dma
{
public:
	dma(std::function<void(uint16_t, uint16_t)> callback)
	: __callback{callback}
	{
		Interrupts::Manager::registerISR(Interrupts::DMA1_Channel1_IRQHandler,
		        std::bind(&dma::isr,this,std::placeholders::_1));

		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = reinterpret_cast<uint32_t>(&(ADC1->DR));
		DMA_InitStructure.DMA_MemoryBaseAddr = reinterpret_cast<uint32_t> (destination);
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = 16;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		/* Enable DMA1 Channel1 */
		DMA_Cmd(DMA1_Channel1, ENABLE);
	}

	~dma()
	{

	}

private:
	class rcc
	{
	public:
		rcc()
		{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		}

		~rcc()
		{

		}
	}rcc_instance;

	void isr(int)
	{
		uint16_t acc1 = 0, acc2 = 0;
		for(auto i : destination)
		{
			acc1 += *(reinterpret_cast<uint16_t *> (&i));
			acc2 += *(reinterpret_cast<uint16_t *> (&i) + 1);
		}
		acc1 /= SIZE;
		acc2 /= SIZE;
		__callback(acc1, acc2);
	}

	//volatile uint16_t res1, res2;
	volatile uint32_t destination[SIZE];

	std::function<void(uint16_t, uint16_t)> __callback;
};



#endif /* GML_INC_DMA_HPP_ */
