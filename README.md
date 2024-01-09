# Xenomai PC Speaker Example driver

Example PC Speaker RDTM driver for Xenomai.

Status:
- early work that does something.
- kernel module in [kernel-module/](kernel-module/) creates RTDM Device `/dev/rtdm/xenospkr0`
- when device is open, speaker will beep on/off every second
- you can open device with app in [xeno-app/](xeno-app/) folder. It will open device
  (speaker start beeping on/off with toggle rate 1s) as long as app will sleep with device open (10s).
  Once app closes device speaker will be permanently off.

Tested version:
- Intel ECI 3.1: https://eci.intel.com/downloads/release-eci_3.1.zip
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

Building:
```shell
cd kernel-module && make
```
Running:
```shell
# inserting module:
sudo insmod ./xeno_spkr.ko
dmesg
```

Now build and run application:
```shell
cd xeno-app
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

