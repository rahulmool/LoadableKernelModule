#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/smp.h>
#include <asm/msr.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("CPU Frequency Monitoring Module");

#define MSR_IA32_PERF_STATUS 0x198 // MSR for reading CPU frequency


static int __init cpufreq_module_init(void) {
    unsigned int cpu;
    
    for_each_online_cpu(cpu) {
        u64 msr_value;
        rdmsrl_on_cpu(cpu, MSR_IA32_PERF_STATUS, &msr_value);
        
        // Extract frequency information from the MSR value
        unsigned int freq = (msr_value >> 8) & 0xFF;
        
        pr_info("CPU%u Frequency: %u MHz\n", cpu, freq);
    }
    return 0;
}

static void __exit cpufreq_module_exit(void) {
    hrtimer_cancel(&hr_timer);
    pr_info("CPU Frequency Monitoring Module unloaded.\n");
}

module_init(cpufreq_module_init);
module_exit(cpufreq_module_exit);
