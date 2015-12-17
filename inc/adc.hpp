#ifndef __ADC_HPP_
#define __ADC_HPP_

#include "stm32f10x.h"
#include "isr.hpp"
#include <functional>

class adc
{
public:
    adc()
    {
        Interrupts::Manager::registerISR(Interrupts::ADC1_IRQHandler,
                std::bind(&adc::ADC_IRQHandler,this,std::placeholders::_1));

        /* ADC1 configuration ------------------------------------------------------*/
        ADC_InitTypeDef ADC_InitStructure;
        ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
        ADC_InitStructure.ADC_ScanConvMode = DISABLE;
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel = 1;
        ADC_Init(ADC1, &ADC_InitStructure);
        /* ADC1 regular channels configuration */
        ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_28Cycles5);
        /* Enable ADC1 EOC interrupt */
        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

        ADC_TempSensorVrefintCmd(ENABLE);

        /* Enable ADC1 */
        ADC_Cmd(ADC1, ENABLE);

        /* Enable ADC1 reset calibration register */
        ADC_ResetCalibration(ADC1);
        /* Check the end of ADC1 reset calibration register */
        while(ADC_GetResetCalibrationStatus(ADC1));

        /* Start ADC1 calibration */
        ADC_StartCalibration(ADC1);
        /* Check the end of ADC1 calibration */
        while(ADC_GetCalibrationStatus(ADC1));

        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    }
private:
    class rcc
    {
    public:
        rcc()
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
        }
        ~rcc()
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , DISABLE);
        }
    } rcc_instance;

    class nvic
    {
    public:
        nvic()
        {
            NVIC_InitTypeDef NVIC_InitStructure;

            /* Configure and enable ADC interrupt */
            NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
        }

        ~nvic()
        {
            NVIC_InitTypeDef NVIC_InitStructure;

            /* Disable ADC interrupt */
            NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);
        }
    } nvic_instance;

    void ADC_IRQHandler(int)
    {
        ADC_Cmd(ADC1, DISABLE);
        /* Get injected channel13 converted value */
        value = ADC_GetConversionValue(ADC1);
    }
};

#endif //__ADC_HPP_
