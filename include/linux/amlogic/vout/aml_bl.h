/*
 * include/linux/amlogic/vout/aml_bl.h
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
*/

#ifndef __INC_AML_BL_H
#define __INC_AML_BL_H
#include <linux/workqueue.h>
#include <linux/amlogic/aml_gpio_consumer.h>
#include <linux/pinctrl/consumer.h>

#define BLPR(fmt, args...)      pr_info("bl: "fmt"", ## args)
#define BLERR(fmt, args...)      pr_info("bl error: "fmt"", ## args)
#define AML_BL_NAME		"aml-bl"

#define BL_LEVEL_MAX		255
#define BL_LEVEL_MIN		10
#define BL_LEVEL_OFF		1

#define BL_LEVEL_MID		128
#define BL_LEVEL_MID_MAPPED	BL_LEVEL_MID
#define BL_LEVEL_DEFAULT	BL_LEVEL_MID

#define XTAL_FREQ_HZ		(24*1000*1000) /* unit: Hz */
#define XTAL_HALF_FREQ_HZ	(24*1000*500)  /* 24M/2 in HZ */

#define BL_FREQ_DEFAULT		1000 /* unit: HZ */
#define BL_FREQ_VS_DEFAULT	2    /* multiple 2 of vfreq */

enum bl_chip_type_e {
	BL_CHIP_M8 = 0,
	BL_CHIP_M8B,
	BL_CHIP_M8M2,
	BL_CHIP_G9TV,
	BL_CHIP_G9BB,
	BL_CHIP_GXTVBB,
	BL_CHIP_MAX,
};

/* for lcd backlight power */
enum bl_ctrl_method_e {
	BL_CTRL_GPIO = 0,
	BL_CTRL_PWM,
	BL_CTRL_PWM_COMBO,
	BL_CTRL_LOCAL_DIMING,
	BL_CTRL_EXTERN,
	BL_CTRL_MAX,
};

enum bl_pwm_method_e {
	BL_PWM_NEGATIVE = 0,
	BL_PWM_POSITIVE,
	BL_PWM_METHOD_MAX,
};

enum bl_pwm_port_e {
	BL_PWM_A = 0,
	BL_PWM_B,
	BL_PWM_C,
	BL_PWM_D,
	BL_PWM_E,
	BL_PWM_F,
	BL_PWM_VS,
	BL_PWM_MAX,
};

enum bl_off_policy_e {
	BL_OFF_POLICY_NONE = 0,
	BL_OFF_POLICY_ALWAYS,
	BL_OFF_POLICY_ONCE,
	BL_OFF_POLICY_MAX,
};

#define BL_GPIO_OUTPUT_LOW      0
#define BL_GPIO_OUTPUT_HIGH     1
#define BL_GPIO_INPUT           2

#define BL_GPIO_MAX             0xff
#define BL_GPIO_NUM_MAX         5
struct bl_gpio_s {
	char name[15];
	struct gpio_desc *gpio;
	int flag;
};

struct bl_pwm_config_s {
	unsigned int index;
	enum bl_pwm_method_e pwm_method;
	enum bl_pwm_port_e pwm_port;
	unsigned int level_max;
	unsigned int level_min;
	unsigned int pwm_freq; /* pwm_vs: 1~4(vfreq), pwm: freq(unit: Hz) */
	unsigned int pwm_duty; /* unit: % */
	unsigned int pwm_duty_max; /* unit: % */
	unsigned int pwm_duty_min; /* unit: % */
	unsigned int pwm_cnt; /* internal used for pwm control */
	unsigned int pwm_pre_div; /* internal used for pwm control */
	unsigned int pwm_max; /* internal used for pwm control */
	unsigned int pwm_min; /* internal used for pwm control */
	unsigned int pwm_level; /* internal used for pwm control */
	unsigned int pwm_gpio;
	unsigned int pwm_gpio_off;
	unsigned int pinmux_flag;
};

struct bl_config_s {
	char name[30];
	unsigned int level_default;
	unsigned int level_min;
	unsigned int level_max;
	unsigned int level_mid;
	unsigned int level_mid_mapping;

	enum bl_ctrl_method_e method;
	unsigned int en_gpio;
	unsigned int en_gpio_on;
	unsigned int en_gpio_off;
	unsigned int power_on_delay;
	unsigned int power_off_delay;
	unsigned int dim_max;
	unsigned int dim_min;

	struct bl_pwm_config_s *bl_pwm;
	struct bl_pwm_config_s *bl_pwm_combo0;
	struct bl_pwm_config_s *bl_pwm_combo1;
	unsigned int pwm_on_delay;
	unsigned int pwm_off_delay;

	struct bl_gpio_s bl_gpio[BL_GPIO_NUM_MAX];
	struct pinctrl *pin;
	unsigned int pinmux_flag;
};

#define BL_INDEX_DEFAULT     0

/* backlight_properties: state */
/* Flags used to signal drivers of state changes */
/* Upper 4 bits in bl props are reserved for driver internal use */
#define BL_STATE_LCD_ON               (1 << 3)
#define BL_STATE_BL_POWER_ON          (1 << 1)
#define BL_STATE_BL_ON                (1 << 0)
struct aml_bl_drv_s {
	unsigned int index;
	unsigned int level;
	unsigned int state;
	struct device             *dev;
	struct bl_config_s        *bconf;
	struct backlight_device   *bldev;
	struct workqueue_struct   *workqueue;
	struct delayed_work       bl_delayed_work;
};

extern struct aml_bl_drv_s *aml_bl_get_driver(void);
extern void bl_pwm_config_init(struct bl_pwm_config_s *bl_pwm);
extern enum bl_pwm_port_e bl_pwm_str_to_pwm(const char *str);
extern void bl_pwm_ctrl(struct bl_pwm_config_s *bl_pwm, int status);

#define BL_GPIO_OUTPUT_LOW		0
#define BL_GPIO_OUTPUT_HIGH		1
#define BL_GPIO_INPUT			2

#endif

