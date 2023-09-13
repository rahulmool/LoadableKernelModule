#include <linux/init.h>
#include <linux/module.h>
#include <linux/cpufreq.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kernel Module to Get CPU Frequencies for All CPUs");
MODULE_VERSION("0.1");

static int __init get_cpu_freq_init(void) {
    unsigned int cpu;
    struct cpufreq_policy *policy;

    for_each_possible_cpu(cpu) {
        policy = cpufreq_cpu_get(cpu);

        if (!policy) {
            pr_err("Failed to get CPU policy for CPU %u\n", cpu);
            continue;
        }

        pr_info("CPU %u Frequency: %lu kHz\n", cpu, policy->cur);

        cpufreq_cpu_put(policy);
    }

    return 0;
}

static void __exit get_cpu_freq_exit(void) {
    pr_info("Exiting...\n");
}

module_init(get_cpu_freq_init);
module_exit(get_cpu_freq_exit);
