/*
 * pwm.hpp
 *
 *  Created on: Mar 8, 2016
 *      Author: penguin
 */

#ifndef GML_INC_PWM_HPP_
#define GML_INC_PWM_HPP_

#include <adc_with_dma.hpp>
#include <timer.hpp>

class pwm
{
public:
	pwm()
	{

	}

private:
	static constexpr std::size_t num_of_measures = 3;
	static constexpr  std::array<adc_inputs, num_of_measures> inputs1{in14, in14, in14}; //resistor
	static constexpr  std::array<adc_inputs, num_of_measures> inputs2{in15, in15, in15}; //pwm
	adc_with_dma<num_of_measures> __adc{inputs1, inputs2, [=](uint16_t res1, uint16_t res2)
		{
			int32_t err = res1 - res2;
			if (err > 3)
				err = 3;
			else if (err < -3)
				err = -3;
			int32_t num = t2ch1.num + err;
			if (num < 0)
				num = 0;
			else if (num > t2ch1.denom)
				num = t2ch1.denom;
			t2ch1.num = num;
			t2oc1.set_duty(t2ch1);
		}};
	timer<tim2> __tim{100000};
	ratio t2ch1{0 , 1000};
	timer<tim2>::oc<ch1> t2oc1{t2ch1};
	timer<tim2>::oc<ch2> t2oc2{ratio{1 , 3}};

	/*auto callback = [](uint16_t res1, uint16_t res2)
		{
			int32_t err = res1 - res2;
			if (err > 3)
				err = 3;
			else if (err < -3)
				err = -3;
			int32_t num = t2ch1.num + err;
			if (num < 0)
				num = 0;
			else if (num > t2ch1.denom)
				num = t2ch1.denom;
			t2ch1.num = num;
			t2oc1.set_duty(t2ch1);
		};*/
};



#endif /* GML_INC_PWM_HPP_ */
