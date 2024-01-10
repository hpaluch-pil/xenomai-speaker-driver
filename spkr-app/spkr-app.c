/*
 * spkr-app.c - Xenomai Speaker - user space application
 *
 * Author: Henryk Paluch, Pickering
 * Tested on Intel ECI 3.1, Xenomai 3.2.2, Debian 11
 * Based on:
 * - https://github.com/andreynech/rtdm-pwm
 */

// headers below are actually POSIX wrappers from
// /usr/include/xenomai/cobalt/
#include <stdio.h>
#include <unistd.h>
// stock /usr/include (C Runtime library)
#include <string.h>

// RTDM is Xenomai Real-Time Driver Model kernel driver API.
#include <rtdm/rtdm.h>
#include <xenomai/version.h>

#include "spkr_ioctl.h"

#define DEVICE_NAME "/dev/rtdm/xenospkr0"

// sleep 0.5 s
#define SLEEP_US 500000

#define TOGGLE_RATE_NS 500000000UL

int main(int argc, char **argv)
{
	int fd, ret = 1,tmp = 0;
	int i;
	unsigned int pitch = 1000;

	rt_printf("Xenomai build Runtime Version: %s!\n", XENO_VERSION_STRING);
	fd = open(DEVICE_NAME, O_RDWR);
	if (fd < 0){
		printf("ERROR: open %s (err=%d: %s)\n", DEVICE_NAME, fd, strerror(-fd));
		goto exit0;
	}
	printf("OK: Device '%s' open, fd=%d\n", DEVICE_NAME, fd);

	for(i=0;i<20;i++){
		pitch += 100;
		ret = ioctl(fd, SPKR_RTIOC_SET_PITCH, pitch );
		if (ret < 0){
			printf("ERROR: ioctl %s SPKR_RTIOC_SET_PITCH (err=%d: %s)\n", DEVICE_NAME, ret, strerror(-ret));
			goto exit1;
		}
		printf("Sleeping for %d [us], Pitch count=%u (%u [Hz]) ...\n",
				SLEEP_US, pitch, SPKR_COUNT_TO_HZ(pitch));
		ret = usleep(SLEEP_US);
		if (ret < 0){
			printf("ERROR: usleep(%d) (err=%d: %s)\n", SLEEP_US, ret, strerror(-ret));
			goto exit1;
		}
	}
	// try increased toggle rate
	ret = ioctl(fd, SPKR_RTIOC_SET_TOGGLE_RATE, TOGGLE_RATE_NS );
	if (ret < 0){
		printf("ERROR: ioctl %s SPKR_RTIOC_SET_TOGGLE_RATE (err=%d: %s)\n", DEVICE_NAME, ret, strerror(-ret));
		goto exit1;
	}
	printf("Toggle rate changed to %lu [ns]\n", TOGGLE_RATE_NS);
	// and wait 2 seconds
	printf("Sleeping 2 seconds...\n");
	ret = sleep(2);
	if (ret < 0){
		printf("ERROR: sleep(%d) (err=%d: %s)\n", 2, ret, strerror(-ret));
		goto exit1;
	}

	ret = 0;
exit1:
	tmp = close(fd);
	if (tmp < 0){
		ret = tmp;
		printf("ERROR: close %s fd=%d (err=%d: %s)\n",
			       	DEVICE_NAME, fd, tmp, strerror(-tmp));
	} else {
		printf("OK: Device '%s' closed.\n", DEVICE_NAME);
	}
	fd = -1;
exit0:
	printf("Exiting %s with ret=%d\n", argv[0], ret);
	return ret;
}
