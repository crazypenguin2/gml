#ifndef __AVERAGE_HPP_
#define __AVERAGE_HPP_

#include "adc.hpp"
#include <functional>

class average
{
public:
    average(int depth=64)
    : _depth{depth}
    {
        //array = new int[_depth];
    }

    ~average()
    {
        //delete[] array;
    }

    int get()
    {
        return value;
    }
private:
    int _depth;
    int value{0};
    int array[64];
    adc adc_instance{std::bind(&average::callback,this,std::placeholders::_1)};

    void callback(int val)
    {
        static int i{0};
        *(array+i) = val;
        if(_depth==++i)
        {
            i = 0;
            value = 0;
            for(int i = 0; i<_depth; i++ )
                value += *(array + i);
            value/=_depth;
        }
    }
};

#endif //__AVERAGE_HPP_
