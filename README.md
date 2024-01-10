# Xenomai PC Speaker Example driver

Example PC Speaker RDTM driver for Xenomai.

Status:
- early work that does something.
- kernel module in [spkr-kernel-module/](spkr-kernel-module/) creates RTDM Device `/dev/rtdm/xenospkr0`
- when device is open, speaker will beep on/off every second
- you can open device with app in [xeno-app/](xeno-app/) folder. It will open device
  (speaker start beeping on/off with toggle rate 1s) as long as app will sleep with device open (10s).
  Once app closes device speaker will be permanently off.

Tested version:
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
```
Running:
```shell
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


Once finished you can remove module using:
```
sudo rmmod ./xeno_spkr.ko
dmesg
```


# Credits

Following projects (and many others were used) to create this example:

- https://github.com/andreynech/rtdm-pwm
- Various parts from Xenomai source, mostly:
  - `xenomai-3.2.2/kernel/drivers/gpiopwm`
- Linux kernel speaker source:
  - `sound/drivers/pcsp/pcsp_input.c`
- https://wiki.osdev.org/PC_Speaker
- https://www.cs.usfca.edu/~cruse/cs630f08/lesson15.ppt

