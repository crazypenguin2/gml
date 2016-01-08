#ifndef __USART_HPP_
#define __USART_HPP_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "isr.hpp"
#include <functional>

class usart
{
public:
	usart()
	{
		auto isr = [&] (int x)
		{
			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
			{
				/* Read one byte from the receive data register */
				RxBuffer[RxCounter++] = (USART_ReceiveData(USART1) & 0x7F);

				if(RxCounter == NbrOfDataToRead)
				{
				/* Disable the EVAL_COM1 Receive interrupt */
					USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
				}
			}

			if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
			{
			/* Write one byte to the transmit data register */
				USART_SendData(USART1, TxBuffer[TxCounter++]);

				if(TxCounter == NbrOfDataToTransfer)
				{
					/* Disable the EVAL_COM1 Transmit interrupt */
				    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
				}
			}
		};
		//Interrupts::Manager::registerISR(Interrupts::USART1_IRQHandler,
		//                std::bind(&usart::USARTx_IRQHandler,this,std::placeholders::_1));
		Interrupts::Manager::registerISR(Interrupts::USART1_IRQHandler, isr);

		USART_InitTypeDef USART_InitStructure;

		/* USARTx configured as follow:
		        - BaudRate = 9600 baud
		        - Word Length = 8 Bits
		        - Two Stop Bit
		        - Odd parity
		        - Hardware flow control disabled (RTS and CTS signals)
		        - Receive and transmit enabled
		  */
		  USART_InitStructure.USART_BaudRate = 9600;
		  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		  USART_InitStructure.USART_StopBits = USART_StopBits_2;
		  USART_InitStructure.USART_Parity = USART_Parity_Odd;
		  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		  USART_Init(USART1, &USART_InitStructure);

		  /* Enable the EVAL_COM1 Transmit interrupt: this interrupt is generated when the
		     EVAL_COM1 transmit data register is empty */
		  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

		  /* Enable the EVAL_COM1 Receive interrupt: this interrupt is generated when the
		     EVAL_COM1 receive data register is not empty */
		  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
private:
	class rcc
	{
	public:
		rcc()
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		}
		~rcc()
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
		}
	} rcc_instance;

	class gpio
	{
	public:
		gpio()
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			/* Configure USART Tx as alternate function push-pull */
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);


			/* Configure USART Rx as input floating */
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
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
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
			}
			~rcc()
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, DISABLE);
			}
		} rcc_instance;
	} gpio_instance;

	class nvic
	{
	public:
		nvic()
		{
			  NVIC_InitTypeDef NVIC_InitStructure;

			  /* Enable the USARTx Interrupt */
			  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			  NVIC_Init(&NVIC_InitStructure);
		}
		~nvic()
		{

		}
	} nvic_instance;

	void USARTx_IRQHandler(int)
	{
	  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	  {
	    /* Read one byte from the receive data register */
	    RxBuffer[RxCounter++] = (USART_ReceiveData(USART1) & 0x7F);

	    if(RxCounter == NbrOfDataToRead)
	    {
	      /* Disable the EVAL_COM1 Receive interrupt */
	      USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	    }
	  }

	  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	  {
	    /* Write one byte to the transmit data register */
	    USART_SendData(USART1, TxBuffer[TxCounter++]);

	    if(TxCounter == NbrOfDataToTransfer)
	    {
	      /* Disable the EVAL_COM1 Transmit interrupt */
	      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	    }
	  }
	}

	static constexpr size_t RxBufferSize = 0x20;
	static constexpr size_t TxBufferSize = 19;

	static const uint8_t TxBuffer[];
	uint8_t RxBuffer[RxBufferSize];
	uint8_t NbrOfDataToTransfer = TxBufferSize;
	uint8_t NbrOfDataToRead = RxBufferSize;
	uint8_t TxCounter = 0;
	uint16_t RxCounter = 0;
};

#endif //__UART_HPP_
