/**
 * @file main.c
 * @author Jeronimo Agullo (jeronimoagullo97@gmail.com)
 * @brief Example of pwm signal controlling a servomotor with Zephyr RTOS
 * @version 1.0
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

static const struct pwm_dt_spec pwm_servo_0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_servo0));

// Minimum and Maximum angles (0 - 180º)
#define MIN_ANGLE_PULSE PWM_USEC(500)	// 500us --> 0°
#define MAX_ANGLE_PULSE PWM_USEC(2500) 	// 2.5ms --> 180°
#define STEP PWM_USEC(100)				// 100us --> ~10°

enum direction {
	DOWN,
	UP,
};

int main(void)
{
	uint8_t dir = 0U;
	uint32_t pulse_width = MIN_ANGLE_PULSE;
	int ret;

	printk("PWM servomotor app\n");

	if (!pwm_is_ready_dt(&pwm_servo_0)) {
		printk("Error: PWM device %s is not ready\n",
		       pwm_servo_0.dev->name);
		return 0;
	}

	printk("Period: %d nsec\nMIN_ANGLE_PULSE: %lu nsec\nMAX_ANGLE_PULSE: %lu nsec\n\n",
			pwm_servo_0.period, MIN_ANGLE_PULSE, MAX_ANGLE_PULSE);


	while (1) {

		printk("pulse_width: %d us\n", pulse_width/1000);
		ret = pwm_set_pulse_dt(&pwm_servo_0, pulse_width);
		if (ret < 0) {
			printk("Error %d: failed to set pulse width\n", ret);
			return 0;
		}

		if (dir == DOWN) {
			pulse_width -= STEP;
			if (pulse_width <= MIN_ANGLE_PULSE) {
				dir = UP;
				pulse_width = MIN_ANGLE_PULSE;
			}
		} else {
			pulse_width += STEP;
			if (pulse_width >= MAX_ANGLE_PULSE) {
				dir = DOWN;
				pulse_width = MAX_ANGLE_PULSE;
			}
		}

		k_sleep(K_SECONDS(1));
	}
	return 0;
}