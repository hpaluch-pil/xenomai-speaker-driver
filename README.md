# Xenomai PC Speaker Example driver

Trivial Example of PC Speaker RDTM driver for Xenomai.

Status:
- it works: kernel driver will beep on PC speaker and toggle it on/off
- kernel module in [spkr-kernel-module/](spkr-kernel-module/) creates RTDM Device `/dev/rtdm/xenospkr0`
- you can open that device with app in [spkr-app/](spkr-app/) folder (only Xenomai apps are allowed
  to access device in RT mode). It will open device and
  - sets pitch (beep frequency) decreasing every 0.5 s, default speaker toggle rate 1s
  - after 10 seconds reduce toggle rate to 0.5s and sleeps 2 seconds.

Tested Xenomai version:
- Intel ECI 3.1: https://eci.intel.com/downloads/release-eci_3.1.zip
  - above ZIP contains `release-eci_3.1/Edge-Controls-for-Industrial/eci-release.tar.gz`
  - when you unpack `eci-release.tar.gz` there is Debian 11 repository
    under `eci-release/targets/packages/bullseye/eci-bullseye` - it was published
    on Interanet and installed on top of existing Debian 11
  - you have follow instructions on: https://eci.intel.com/docs/3.1/getstarted/installing_packages.html
  - at least these Debian 11 packages should be installed:
    ```shell
    sudo apt install -y eci-customizations
    sudo apt-get reinstall '(firmware-linux-nonfree|linux-firmware$)'
    sudo apt install -y linux-intel-rt linux-image-intel-xenomai
    sudo apt install -y eci-xenomai
    sudo update-grub
    ```
  - you need to reboot system and ensure that Dovetail kernel version
    is booted, in my example `5.10.179-intel-ese-standard-lts-dovetail+`

Details:
- hosted on Debian 11
- repository from above ECI.
- kernel: `5.10.179-intel-ese-standard-lts-dovetail+`
- runtime:
  ```shell
  $ dpkg -l xenomai-runtime

  | Status=Not/Inst/Conf-files/Unpacked/halF-conf/Half-inst/trig-aWait/Trig-pend
  |/ Err?=(none)/Reinst-required (Status,Err: uppercase=bad)
  ||/ Name            Version      Architecture Description
  +++-===============-============-============-=================================
  ii  xenomai-runtime 3.2.2-2      amd64        Xenomai runtime utilities
  ```

Building this project:
```shell
cd spkr-kernel-module && make
# append debug=1 to above "make" to enable DEBUG messages.
```
Running:
```shell
# removing old module (only if you already inserted it before):
sudo rmmod xeno_spkr
# inserting module:
sudo insmod ./xeno_spkr.ko
dmesg
```

Now build and run application:
```shell
cd spkr-app
make
./spkr-app
```
You should see beeps with 1s toggle on/off as long
as device is open.

While application is running you can see if it really uses Xenomai by
querying:

```shell
$ cat /proc/xenomai/sched/threads

CPU  PID    CLASS  TYPE      PRI   TIMEOUT       STAT       NAME
  0  0      idle   core       -1   -             R          [ROOT/0]
  1  0      idle   core       -1   -             R          [ROOT/1]
  2  0      idle   core       -1   -             R          [ROOT/2]
  3  0      idle   core       -1   -             R          [ROOT/3]
  0  336    rt     core       98   -             W          [rtnet-stack]
  0  337    rt     core        0   -             W          [rtnet-rtpc]
  1  6277   weak   cobalt      0   428ms75us     D          spkr-app
```

Last line is our `spkr-app` application.


Once finished you can remove module using:
```
sudo rmmod ./xeno_spkr.ko
dmesg
```


# Credits

Following projects (and many others) were used to create this example:

- https://github.com/andreynech/rtdm-pwm
- Various parts from Xenomai source, mostly:
  - `xenomai-3.2.2/kernel/drivers/gpiopwm`
- Linux kernel speaker source:
  - `sound/drivers/pcsp/pcsp_input.c`
- https://wiki.osdev.org/PC_Speaker
- https://www.cs.usfca.edu/~cruse/cs630f08/lesson15.ppt

