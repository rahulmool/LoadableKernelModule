cmd_/home/mool/Documents/LoadableKernelModule/LoadableKernelModule/defender.mod := printf '%s\n'   defender.o | awk '!x[$$0]++ { print("/home/mool/Documents/LoadableKernelModule/LoadableKernelModule/"$$0) }' > /home/mool/Documents/LoadableKernelModule/LoadableKernelModule/defender.mod