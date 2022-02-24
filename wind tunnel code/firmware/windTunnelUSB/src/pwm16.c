/*
 * pwm16.c
 *
 * Created: 1/3/2013 12:12:54 PM
 *  Author: Corvus
 */ 

#include "pwm16.h"



/**
 * \brief Start a PWM channel
 *
 * This function enables a channel with a given duty cycle.
 *
 * \param *config           Pointer to the PWM configuration struct
 * \param duty_cycle_scale  Duty cycle as a value between 0 and 10000.
 */
void pwm_start16(struct pwm_config *config, uint16_t duty_cycle_scale)
{
	/* Set given duty cycle */
	pwm_set_duty_cycle16(config, duty_cycle_scale);
	/* Set correct TC period */
	tc_write_period(config->tc, config->period);
	/* Enable CC channel for this TC */
	tc_enable_cc_channels(config->tc, config->cc_mask);
	/* Enable TC by setting correct clock prescaler */
	tc_write_clock_source(config->tc, config->clk_sel);
}


/**
 * \brief Function to set PWM duty cycle
 *
 * The duty cycle can be set on a scale between 0-100%. This value
 * will be used to update the CCx register for the selected PWM channel.
 *
 * \param *config           Pointer to the PWM configuration struct
 * \param duty_cycle_scale  Duty cycle as a value between 0 and 10000.
 */
void pwm_set_duty_cycle16(struct pwm_config *config, uint16_t duty_cycle_scale)
{
	Assert( duty_cycle_scale <= 10000 );
	
	tc_write_cc_buffer(config->tc, config->channel,
	(uint16_t)(((uint32_t)config->period * (uint32_t)duty_cycle_scale) / 10000));
}