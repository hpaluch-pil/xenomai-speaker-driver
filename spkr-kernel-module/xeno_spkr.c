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

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/types.h>
#include <rtdm/driver.h>
#include <rtdm/rtdm.h>

#include "spkr_ioctl.h"

MODULE_AUTHOR("Henryk Paluch <henryk.paluch@pickering.cz>");
MODULE_DESCRIPTION("RDTM PC Speaker driver");
MODULE_VERSION("0.0.1");
MODULE_LICENSE("GPL");


#define SPKR_PREFIX "SPKR "
#define prx_debug(fmt,...) rtdm_printk(SPKR_PREFIX "DEBUG %s:%d: " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)
#define prx_info(fmt,...) rtdm_printk(SPKR_PREFIX  "..... %s:%d: " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)
#define prx_err(fmt,...) rtdm_printk(SPKR_PREFIX   "ERROR at %s:%d: " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

#define SPKR_PROFILE_VER 1

struct spkr {
	unsigned int period;
};

struct spkr_priv {
	rtdm_timer_t base_timer;
	unsigned char spkr_data;
	unsigned int configured;
	unsigned long period; // speaker toggle (on/off) rate in [ns]
	unsigned long ticks;
	unsigned int count; // speaker Pitch (beep frequency) i8254 counter value
};


static void spkr_handle_base_timer(rtdm_timer_t *timer)
{
	struct spkr_priv *ctx = container_of(timer, struct spkr_priv, base_timer);

	ctx->ticks++;

	ctx->spkr_data ^= 3;

	// from kernel/sound/drivers/pcsp/pcsp_input.c

	if (ctx->spkr_data){
		/* enable counter 2 */
	    outb_p(inb_p(0x61) | 3, 0x61);
	} else {
		/* disable counter 2 */
        outb(inb_p(0x61) & 0xFC, 0x61);
	}

	prx_debug("Timer tick #%lu, speaker is %s", ctx->ticks, ctx->spkr_data ? "ON" : "OFF" );
}

// set speaker pitch (frequency of beep)
static void spkr_setpitch(struct spkr_priv *ctx)
{
	// TODO: Refuse ctx->count > 65535 (overflow)
	unsigned div32 = ctx->count;
	if (div32 == 0){
		div32 = 65536; // i8254 works that way.
	}
    /* set command for counter 2, 2 byte write */
    outb_p(0xB6, 0x43);
    /* select desired HZ */
    outb_p(ctx->count & 0xff, 0x42);
    outb((ctx->count >> 8) & 0xff, 0x42);
    prx_info("Speaker pitch set to count=%u (%u [Hz]) ", ctx->count, SPKR_PITCH_XTAL / div32);
}

static int spkr_start_toggle_timer(struct spkr_priv *ctx)
{
	int ret = 0;

	ret = rtdm_timer_start(&ctx->base_timer, ctx->period, ctx->period,
			 RTDM_TIMERMODE_RELATIVE);
	if (ret < 0){
		prx_err("rtdm_timer_start: period=%lu failed with err=%d", ctx->period, ret);
	}
	prx_info("Set speaker toggle rate to %lu [ns]", ctx->period);
	return ret;
}

static int spkr_open(struct rtdm_fd *fd, int oflags)
{
	int ret = 0;
	struct spkr_priv *ctx = rtdm_fd_to_private(fd);

	prx_debug("called");

	ctx->configured = 0;
	ctx->period = 1000000000UL;
	ctx->ticks = 0;
	ctx->spkr_data = 0;
	ctx->count=500;

	ret = rtdm_timer_init(&ctx->base_timer, spkr_handle_base_timer, "spkr_timer");
	if (ret < 0){
		prx_err("rtdm_timer_init: failed with err=%d", ret);
		goto exit1;
	}

	spkr_setpitch(ctx);

	ret = spkr_start_toggle_timer(ctx);
	if (ret < 0){
		prx_err("rtdm_timer_start: failed with err=%d", ret);
		rtdm_timer_destroy(&ctx->base_timer);
		goto exit1;
	}

	ctx->configured = 1;
exit1:
	return ret;
}

static void spkr_close(struct rtdm_fd *fd)
{
	struct spkr_priv *ctx = rtdm_fd_to_private(fd);

	prx_debug("called");

	if (!ctx->configured)
		return;

	rtdm_timer_destroy(&ctx->base_timer);
	/* disable counter 2 */
    outb(inb_p(0x61) & 0xFC, 0x61);
}

static int spkr_ioctl_rt(struct rtdm_fd *fd, unsigned int request, void __user *arg)
{
        struct spkr_priv *ctx = rtdm_fd_to_private(fd);

        switch (request) {
        case SPKR_RTIOC_SET_PITCH:
        	ctx->count = (unsigned)arg;
        	spkr_setpitch(ctx);
        	return 0;
        case SPKR_RTIOC_SET_TOGGLE_RATE:
        	ctx->period = (unsigned long)arg;
        	return spkr_start_toggle_timer(ctx);
        default:
                return -EINVAL;
        }
        return 0;
}


static struct rtdm_driver spkr_driver = {
	.profile_info           = RTDM_PROFILE_INFO(xenospkr,
						    RTDM_CLASS_EXPERIMENTAL,
						    RTDM_SUBCLASS_GENERIC,
						    SPKR_PROFILE_VER),
	.device_flags		= RTDM_NAMED_DEVICE | RTDM_EXCLUSIVE,
	.device_count		= 1,
	.context_size		= sizeof(struct spkr_priv),
	.ops = {
		.open		= spkr_open,
		.close		= spkr_close,
		.ioctl_rt	= spkr_ioctl_rt,
		//.ioctl_nrt	= gpiopwm_ioctl_nrt,
	},
};

static struct rtdm_device device = {
		.driver = &spkr_driver,
		.label = "xenospkr%d",
};


static int __init __spkr_init(void)
{
	int ret = 0;

	prx_debug("called");
	ret = rtdm_dev_register(&device);
	if (ret){
		prx_err("rtdm_dev_register failed with err=%d", ret);
	}
	return ret;
}


static void __exit __spkr_exit(void)
{
	 prx_debug("called");
	 rtdm_dev_unregister(&device);
}

module_init(__spkr_init);
module_exit(__spkr_exit);


