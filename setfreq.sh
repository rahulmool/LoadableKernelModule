# Set the CPU core number and desired frequency
CPU_CORE=0  # Replace with the desired core number (e.g., 0 for the first core)
FREQUENCY=800000  # Replace with the desired frequency in kHz (e.g., 2 GHz)

# Set the CPU frequency scaling governor to "userspace" for manual control
echo "userspace" | sudo tee /sys/devices/system/cpu/cpu$CPU_CORE/cpufreq/scaling_governor

# Set the desired frequency
echo $FREQUENCY | sudo tee /sys/devices/system/cpu/cpu$CPU_CORE/cpufreq/scaling_setspeed
