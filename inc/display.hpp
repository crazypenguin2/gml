#ifndef __DISPLAY_HPP_
#define __DISPLAY_HPP_

#include "spi.hpp"
#include "systick.hpp"
#include <functional>

class display
{
public:
    /*display()
    : systick_instance(std::function<void(void)>(std::bind(&display::systick_handler,this)))
    {}*/

    void print(int value)
    {
        uint8_t digit[length];
        digit[0] = value/1000;
        digit[1] = (value%1000)/100;
        digit[2] = (value%100)/10;
        digit[3] = value%10;
        for(int i=3; i>=0; i--)
            segment[i] = (decode_table[digit[i]] << 8) + (segment[i] & 0x00FF);
    }

private:
    int length = 4;
    static const uint8_t decode_table[];
    uint16_t segment[4]{0x003F, 0x00CF, 0x00F3, 0x00FC};

    spi spi_instance;
    systick systick_instance{ std::bind(&display::systick_handler,this) };

    void strobe()
    {
        GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);
        GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);
    }

    void systick_handler()
    {
        strobe();
        static int i{3};
        SPI_I2S_SendData(SPI1, *(segment+i));
        spi_instance.enable();
        if(!i--)
            i=3;
    }
};

#endif //__DISPLAY_HPP_
