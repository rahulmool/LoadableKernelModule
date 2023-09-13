#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cpumask.h>
#include <linux/cpu.h>
#include <asm/msr.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("MSR 0x150 Reader/Writer");

static int __init msr_reader_init(void) {
    u32 lo, hi;
    u64 msr_value;
    u64 write_value = 0x8000001000000000; // Replace with your desired value

    // Check if MSR 0x150 exists
    // if (!cpu_has_msr) {
    //     pr_err("MSR is not supported on this CPU\n");
    //     return -ENODEV;
    // }

    // Write to MSR 0x150
    wrmsr_on_cpu(0, 0x150, (u32)(write_value & 0xFFFFFFFF), (u32)(write_value >> 32));

    // Read from MSR 0x150
    rdmsr_on_cpu(0, 0x150, &lo, &hi);
    msr_value = ((u64)hi << 32) | lo;

    pr_info("MSR 0x150 value: 0x%llx\n", msr_value);

    return 0;
}

static void __exit msr_reader_exit(void) {
    pr_info("MSR reader module unloaded\n");
}

module_init(msr_reader_init);
module_exit(msr_reader_exit);
