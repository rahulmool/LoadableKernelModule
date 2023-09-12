## Linux kernel headers
### Install
```bash
sudo apt install --reinstall linux-headers-$(uname -r)
```
### Run hello world program
```bash
insmod hello.ko
dmesg
rmmod hello.ko
dmseg
```