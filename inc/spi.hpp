#ifndef __SPI_HPP_
#define __SPI_HPP_

#include <functional>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "isr.hpp"

class spi
{
public:
    spi()
    {
        Interrupts::Manager::registerISR(Interrupts::SPI1_IRQHandler,
                std::bind(&spi::SPI_IRQHandler,this,std::placeholders::_1));

        /* SPI_MASTER configuration ------------------------------------------------*/
        SPI_InitTypeDef SPI_InitStructure;
        SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
        SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
        SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
        SPI_InitStructure.SPI_CRCPolynomial = 0;
        SPI_Init(SPI1, &SPI_InitStructure);

        /* Enable SPI_MASTER TXE interrupt */
        SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
    }

    void enable()
    {
        SPI_Cmd(SPI1, ENABLE);
    }

    void disable()
    {
        SPI_Cmd(SPI1, DISABLE);
    }

    ~spi()
    {}

private:
    class rcc
    {
    public:
        rcc()
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        }

        ~rcc()
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);
        }
    } rcc_instance;

    class gpio
    {
    public:
        gpio()
        {
            GPIO_InitTypeDef GPIO_InitStructure;

            /* Disable JTAG-DP */
            GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

            /* Enable SPI1 Pins Software Remapping */
            GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

            /* Configure SPI_MASTER pins: SCK and MOSI ---------------------------------*/
            /* Configure SCK and MOSI pins as Alternate Function Push Pull */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            GPIO_Init(GPIOB, &GPIO_InitStructure);

            /* Configure SCK and MOSI pins as Alternate Function Push Pull */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(GPIOB, &GPIO_InitStructure);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            GPIO_Init(GPIOC, &GPIO_InitStructure);
        }

        ~gpio()
        {
            GPIO_InitTypeDef GPIO_InitStructure;

            /* Enable JTAG-DP */
            GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, DISABLE);

            /* Disable SPI1 Pins Software Remapping */
            GPIO_PinRemapConfig(GPIO_Remap_SPI1, DISABLE);

            /* Restore in default state  SPI_MASTER pins: SCK and MOSI ----*/
            /* Restore SCK and MOSI pins --------------------------------- */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            GPIO_Init(GPIOB, &GPIO_InitStructure);

            /* Restore SCK and MOSI pins --------------------------------- */
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            GPIO_Init(GPIOB, &GPIO_InitStructure);

            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            GPIO_Init(GPIOC, &GPIO_InitStructure);
        }
    private:
        class rcc
        {
        public:
            rcc()
            {
                /* Enable GPIO clock for SPI_MASTER */
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
            }
            ~rcc()
            {
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, DISABLE);
            }
        } rcc_instance;
    } gpio_instance;

    class nvic
    {
    public:
        nvic()
        {
            SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE); /*Bug with soft reset*/

            NVIC_InitTypeDef NVIC_InitStructure;

            /* Configure and enable SPI_MASTER interrupt -------------------*/
            NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
        }

        ~nvic()
        {
            NVIC_InitTypeDef NVIC_InitStructure;

            /* Disable SPI_MASTER interrupt --------------------------------*/
            NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);
        }
    } nvic_instance;

    void SPI_IRQHandler(int)
    {
        disable();
    }
};

#endif //__SPI_HPP_
