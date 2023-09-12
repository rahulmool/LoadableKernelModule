## Linux kernel headers
### Install
```bash
sudo apt install --reinstall linux-headers-$(uname -r)
```

### Run linux parameters program
```bash
insmod hello.ko param_var=2323
dmesg
rmmod hello.ko
dmseg
```