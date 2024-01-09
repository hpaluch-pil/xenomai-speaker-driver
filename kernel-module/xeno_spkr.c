/*
 * xeno_spkr.c - Xenomain Speaker RDTM Driver
 *
 * Based on:
 * - Xenomai driver: xenomai-3.2.2/kernel/drivers/gpiopwm/gpiopwm.c
 *
 * Copyright: Henryk Paluch
 * Portions Copyright:
 * -Copyright (C) 2015 Jorge Ramirez <jro@xenomai.org>. (gpiopwm.c)
 *
 */

#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <rtdm/driver.h>

MODULE_AUTHOR("Henryk Paluch <henryk.paluch@pickering.cz>");
MODULE_DESCRIPTION("RDTM PC Speaker driver");
MODULE_VERSION("0.0.1");
MODULE_LICENSE("GPL");

static int __init __spkr_init(void)
{
	int ret = 0;

	rtdm_printk("%s:%d called\n", __func__, __LINE__);
	return ret;
}


static void __exit __spkr_exit(void)
{
	 rtdm_printk("%s:%d called\n", __func__,__LINE__);
}

module_init(__spkr_init);
module_exit(__spkr_exit);


