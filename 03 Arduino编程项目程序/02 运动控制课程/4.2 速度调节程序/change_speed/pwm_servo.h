/**
 * @file pwm_servo.h
 * @brief Header file for PWM servo control in MagDog project.
 * 
 * This file contains the definition of the PWM servo control functions and structures used in the MagDog project.
 * 
 * @author LuYongping
 * @date 2023-12-20
 */

#ifndef PWM_SERVOS_H
#define PWM_SERVOS_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void pwm_servo_init(void);
void pwm_servo_deinit(void);
int pwm_servo_set_position(uint32_t servo_index, uint32_t pulsewidth, uint32_t duration);
int pwm_servo_set_offset(uint32_t servo_index, int offset);
int pwm_servo_position(int8_t servo_id,int position = 0,int duration_ms = 0);
int pwm_servo_save_offset(int8_t servo_id);
int get_pwm_servo_offset(int8_t servo_id);
int set_pwm_servo_offset(int8_t servo_id, int new_offset);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
