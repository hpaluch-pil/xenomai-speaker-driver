/*
 * spkr_ioctl.h
 *
 *  Based on: xenomai-3.2.2/include/rtdm/uapi/gpiopwm.h
 *  Created on: Jan 10, 2024
 *      Author: lxi
 */

#ifndef SPKR_IOCTL_H_
#define SPKR_IOCTL_H_

#include <linux/types.h>

// XTAL frequency of i8254 timer - pitch is basiclly divider
#define SPKR_PITCH_XTAL 1193182

#define RTIOC_TYPE_SPKR          RTDM_CLASS_EXPERIMENTAL

#define SPKR_RTIOC_SET_PITCH \
        _IOW(RTIOC_TYPE_SPKR, 0x10, unsigned int)

#define SPKR_RTIOC_SET_TOGGLE_RATE \
        _IOW(RTIOC_TYPE_SPKR, 0x20, unsigned long)


#endif /* SPKR_IOCTL_H_ */
