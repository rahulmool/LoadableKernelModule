## Linux kernel headers
### Install
```bash
sudo apt install --reinstall linux-headers-$(uname -r)
```

### Run array parameters program
```bash
insmod hello.ko param_var=24,535,190
dmesg
rmmod hello.ko
dmseg
```