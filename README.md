## Linux kernel headers
### Install
```bash
sudo apt install --reinstall linux-headers-$(uname -r)
```

# Note
This program is only intended to check the overhead of the program nothing else

# Run
```bash
make
sudo su
insmod defender.ko
```
