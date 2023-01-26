/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS	1

#define SW0_NODE	DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
static struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0});							 

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

void main(void)
{
	int ret;

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	ret = gpio_pin_interrupt_configure_dt(&button,GPIO_INT_EDGE_TO_ACTIVE);
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT);
	
	while (1) {
		int val = gpio_pin_get_dt(&button);
		if (val >= 0) {
			gpio_pin_set_dt(&led, 1 - val);
			gpio_pin_set_dt(&led1, val);
		}
		else{
			gpio_pin_set_dt(&led, val + 1);
			gpio_pin_set_dt(&led1, 1 - val);
		}

		k_msleep(SLEEP_TIME_MS);
	}
}
