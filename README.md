# Xenomai PC Spekaer Example driver

Example PC Speaker RDTM driver for Xenomai.

Status:
- just bare skeleton module that can be inserted and removed

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

# removing module:
sudo rmmod ./xeno_spkr.ko
dmesg
```

