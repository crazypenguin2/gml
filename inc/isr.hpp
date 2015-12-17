#ifndef INTERRUPT_HPP_
#define INTERRUPT_HPP_

#include <functional>

namespace Interrupts{

enum {
    //Cortex M3 Handlers
    Reset_Handler                   = 1,
    NMI_Handler                     = 2,
    HardFault_Handler               = 3,
    MemManage_Handler               = 4,
    BusFault_Handler                = 5,
    UsageFault_Handler              = 6,
    SVC_Handler                     = 11,
    DebugMon_Handler                = 12,
    PendSV_Handler                  = 14,
    SysTick_Handler                 = 15,
    //STM32 Handlers
    WWDG_IRQHandler                 = 16,
    PVD_IRQHandler                  = 17,
    TAMPER_IRQHandler               = 18,
    RTC_IRQHandler                  = 19,
    FLASH_IRQHandler                = 20,
    RCC_IRQHandler                  = 21,
    EXTI0_IRQHandler                = 22,
    EXTI1_IRQHandler                = 23,
    EXTI2_IRQHandler                = 24,
    EXTI3_IRQHandler                = 25,
    EXTI4_IRQHandler                = 26,
    DMA1_Channel1_IRQHandler        = 27,
    DMA1_Channel2_IRQHandler        = 28,
    DMA1_Channel3_IRQHandler        = 29,
    DMA1_Channel4_IRQHandler        = 30,
    DMA1_Channel5_IRQHandler        = 31,
    DMA1_Channel6_IRQHandler        = 32,
    DMA1_Channel7_IRQHandler        = 33,
    ADC1_IRQHandler                 = 34,
    EXTI9_5_IRQHandler              = 39,
    TIM1_BRK_TIM15_IRQHandler       = 40,
    TIM1_UP_TIM16_IRQHandler        = 41,
    TIM1_TRG_COM_TIM17_IRQHandler   = 42,
    TIM1_CC_IRQHandler              = 43,
    TIM2_IRQHandler                 = 44,
    TIM3_IRQHandler                 = 45,
    TIM4_IRQHandler                 = 46,
    I2C1_EV_IRQHandler              = 47,
    I2C1_ER_IRQHandler              = 48,
    I2C2_EV_IRQHandler              = 49,
    I2C2_ER_IRQHandler              = 50,
    SPI1_IRQHandler                 = 51,
    SPI2_IRQHandler                 = 52,
    USART1_IRQHandler               = 53,
    USART2_IRQHandler               = 54,
    USART3_IRQHandler               = 55,
    EXTI15_10_IRQHandler            = 56,
    RTCAlarm_IRQHandler             = 57,
    CEC_IRQHandler                  = 58,
    TIM6_DAC_IRQHandler             = 70,
    TIM7_IRQHandler                 = 71
};

const int NUM_INTERRUPTS=80;

typedef std::function<void(int)> ISR_t;

class Manager
{
    static ISR_t ISRs[NUM_INTERRUPTS];

public:
    static void registerISR(int interruptID, ISR_t isr)
        { ISRs[interruptID]=isr; }
    static void isrDispatcher(int interruptID)
        { ISRs[interruptID](interruptID); }
};

} //namespace Interrupts

#endif //INTERRUPT_HPP_
