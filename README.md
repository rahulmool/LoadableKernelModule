## Linux kernel headers
### Install
```bash
sudo apt install --reinstall linux-headers-$(uname -r)
apt install dwarves

```

### Run array parameters program
```bash
insmod hello.ko param_var=24,535,190
dmesg
rmmod hello.ko
dmseg
```
### Cahnge the following lines of  /boot/config-6.2.0-32-generic
CONFIG_MODULE_SIG_FORMAT=n
CONFIG_MODULE_SIG=n

sudo modprobe kvm-intel