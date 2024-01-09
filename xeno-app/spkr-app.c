/*
 * spkr-app.c - Xenomai Speaker - user space application
 *
 * Tested on Intel ECI 3.1, Xenomai 3.2.2, Debian 11
 * Based on:
 * - https://github.com/andreynech/rtdm-pwm
 */

// headers below are actually POSIX wrappers from
// /usr/include/xenomai/cobalt/
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <rtdm/rtdm.h>
#include <xenomai/version.h>

#define DEVICE_NAME "/dev/rtdm/xenospkr0"

#define SLEEP_S 3

int main(int argc, char **argv)
{
	int fd, ret = 1,tmp = 0;

	rt_printf("Xenomai build Runtime Version: %s!\n", XENO_VERSION_STRING);
	fd = open(DEVICE_NAME, O_RDWR);
	if (fd < 0){
		printf("ERROR: open %s (err=%d: %s)\n", DEVICE_NAME, fd, strerror(-fd));
		goto exit0;
	}
	printf("Sleeping for %d [s]...\n", SLEEP_S);
	ret = sleep(SLEEP_S);
	if (ret < 0){
		printf("ERROR: sleep(%d) (err=%d: %s)\n", SLEEP_S, ret, strerror(-ret));
		goto exit1;
	}

	ret = 0;
exit1:
	tmp = close(fd);
	if (tmp < 0){
		ret = tmp;
		printf("ERROR: close %s fd=%d (err=%d: %s)\n",
			       	DEVICE_NAME, fd, tmp, strerror(-tmp));

	}
	fd = -1;
exit0:
	printf("Exiting %s with ret=%d\n", argv[0], ret);
	return ret;
}
