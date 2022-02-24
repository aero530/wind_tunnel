/*
 * pwm16.h
 *
 * Created: 1/3/2013 12:13:05 PM
 *  Author: Corvus
 */ 


#ifndef PWM16_H_
#define PWM16_H_


#include "compiler.h"
#include "asf.h"
#include "stdint.h"


void pwm_start16(struct pwm_config *config, uint16_t duty_cycle_scale);

void pwm_set_duty_cycle16(struct pwm_config *config,uint16_t duty_cycle_scale);

#endif /* PWM16_H_ */