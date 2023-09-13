#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kernel Module to Get Actual CPU Frequencies for All CPUs");
MODULE_VERSION("0.1");

static int __init get_cpu_freq_init(void) {
    unsigned int cpu;
    char freq_file_path[64];
    char freq_buf[32];
    struct file *file;
    int len;

    for_each_possible_cpu(cpu) {
        snprintf(freq_file_path, sizeof(freq_file_path), "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq", cpu);

        // Open the frequency file for reading
        file = filp_open(freq_file_path, O_RDONLY, 0);
        if (IS_ERR(file)) {
            pr_err("Failed to open %s\n", freq_file_path);
            continue;
        }

        // Read CPU frequency
        len = kernel_read(file, freq_buf, sizeof(freq_buf), &file->f_pos);
        if (len < 0) {
            pr_err("Failed to read %s\n", freq_file_path);
            filp_close(file, NULL);
            continue;
        }

        // Null-terminate the buffer
        freq_buf[len-1] = '\0';

        // Print the CPU frequency for this CPU
        pr_info("CPU %u Frequency: %s kHz\n", cpu, freq_buf);

        // Close the file
        filp_close(file, NULL);
    }

    return 0;
}

static void __exit get_cpu_freq_exit(void) {
    pr_info("Exiting...\n");
}

module_init(get_cpu_freq_init);
module_exit(get_cpu_freq_exit);
