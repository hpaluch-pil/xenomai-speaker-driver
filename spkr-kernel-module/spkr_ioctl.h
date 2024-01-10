/*
 * spkr_ioctl.h
 *
 *  Based on: xenomai-3.2.2/include/rtdm/uapi/gpiopwm.h
 *  Created on: Jan 10, 2024
 *      Author: Henryk Paluch, Pickering
 */

#ifndef SPKR_IOCTL_H_
#define SPKR_IOCTL_H_

#include <linux/types.h>

// XTAL frequency of i8254 timer - pitch (count) is basically divider
#define SPKR_PITCH_XTAL 1193182

// Maximum i8254 counter value for speaker pitch - SPKR_RTIOC_SET_PITCH
// NOTE: 0 is OK (=65536 for i8254)
#define SPKR_MAX_COUNT_VAL 65535

// convert count (for SPKR_RTIOC_SET_PITCH) to frequency in Hz
#define SPKR_COUNT_TO_HZ(count) \
	 ( SPKR_PITCH_XTAL / ( (count) == 0 ? 65536 : (count) ))

#define RTIOC_TYPE_SPKR          RTDM_CLASS_EXPERIMENTAL

#define SPKR_RTIOC_SET_PITCH \
        _IOW(RTIOC_TYPE_SPKR, 0x10, unsigned int)

#define SPKR_RTIOC_SET_TOGGLE_RATE \
        _IOW(RTIOC_TYPE_SPKR, 0x20, unsigned long)


#endif /* SPKR_IOCTL_H_ */
