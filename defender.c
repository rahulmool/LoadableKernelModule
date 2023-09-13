#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kernel Module to Get Actual CPU Frequencies for All CPUs");
MODULE_VERSION("0.1");

int getfreqoffset(int freq){
        if(0<=freq && freq<=650){           return -128;//assume
        }else if(650<=freq && freq<=750){   return -128;//crash
        }else if(750<=freq && freq<=850){   return -123;//crash
        }else if(850<=freq && freq<=950){   return -128;//crash
        }else if(950<=freq && freq<=1500){  return -113;//crash
        }else if(1050<=freq && freq<=1150){ return -116;//crash
        }else if(1150<=freq && freq<=1250){ return -117;
        }else if(1250<=freq && freq<=1350){ return -113;
        }else if(1350<=freq && freq<=1450){ return -113;
        }else if(1450<=freq && freq<=1550){ return -100;
        }else if(1550<=freq && freq<=1650){ return -101;
        }else if(1650<=freq && freq<=1750){ return -101;
        }else if(1750<=freq && freq<=1850){ return -99; 
        }else if(1850<=freq && freq<=1950){ return -87; 
        }else if(1950<=freq && freq<=2050){ return -87; 
        }else if(2050<=freq && freq<=2150){ return -85; 
        }else if(2150<=freq && freq<=2250){ return -87; 
        }else if(2250<=freq && freq<=2350){ return -83; 
        }else if(2350<=freq && freq<=2450){ return -86; 
        }else if(2450<=freq && freq<=2550){ return -90; 
        }else if(2550<=freq && freq<=2650){ return -94; 
        }else if(2650<=freq && freq<=2750){ return -97; 
        }else if(2750<=freq && freq<=2850){ return -98; 
        }else if(2850<=freq && freq<=2950){ return -100;
        }else if(2950<=freq && freq<=3050){ return -104;
        }else if(3050<=freq && freq<=3150){ return -105;
        }else if(3150<=freq && freq<=3250){ return -108;
        }else if(3250<=freq && freq<=3350){ return -106;
        }else if(3350<=freq && freq<=3450){ return -121;
        }else if(3450<=freq && freq<=3550){ return -130;
        }else if(3550<=freq && freq<=3650){ return -134;
        }else if(3650<=freq && freq<=3750){ return -139;
        }else if(3750<=freq && freq<=3850){ return -143;
        }else if(3850<=freq && freq<=3950){ return -153;
        }else if(3950<=freq && freq<=4050){ return -154;
        }else if(4050<=freq && freq<=4150){ return -156;
        }else if(4150<=freq && freq<=4250){ return -159;
        }else if(4250<=freq && freq<=4350){ return -163;//crash
        }else if(4350<=freq && freq<=4450){ return -138;//crash
        }else if(4450<=freq && freq<=4550){ return -139;//crash
        }else if(4550<=freq && freq<=4650){ return -152;//crash
        }else if(4650<=freq && freq<=4750){ return -151;//crash
        }else if(4750<=freq && freq<=4850){ return -151;//assume
        }else if(4850<=freq && freq<=4950){ return -151;//assume
        } 
        pr_info("Failed to read %u\n", freq);
        return -1;
}

int maximumoffset(void)
{
    unsigned int cpu;
    char freq_file_path[64];
    char freq_buf[32];
    struct file *file;
    int len;
    int maxoffset = -1;
    int ret, t,i;
    int int_value;
    for_each_possible_cpu(cpu)
    {
        int_value = 0;
        snprintf(freq_file_path, sizeof(freq_file_path), "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq", cpu);

        // Open the frequency file for reading
        file = filp_open(freq_file_path, O_RDONLY, 0);
        if (IS_ERR(file))
        {
            pr_err("Failed to open %s\n", freq_file_path);
            continue;
        }

        // Read CPU frequency
        len = kernel_read(file, freq_buf, sizeof(freq_buf), &file->f_pos);
        if (len < 0)
        {
            pr_err("Failed to read %s\n", freq_file_path);
            filp_close(file, NULL);
            continue;
        }

        // Null-terminate the buffer
        freq_buf[len - 1] = '\0';

        // Print the CPU frequency for this CPU

        // Use simple_strtoul to convert the string to an integer
        // pr_info("String: %s\n", freq_buf);
        for(i=0;i<len-1;i++){
            int_value = int_value*10 + freq_buf[i] - '0';
        }
        // ret = kstrtouint(freq_buf, len, &int_value);
        //pr_info("Integer: %u\n", int_value);
        t = getfreqoffset(int_value/1000);
        //pr_info("offset is %u,%d,maxoffset=%d,%u/n", t,t,maxoffset,maxoffset);
        if (t < maxoffset )
        {
            maxoffset = t;
          //              pr_info("maxoffset is %d",maxoffset);

        }
        // pr_info("CPU %u Frequency: %u kHz\n", cpu, freq_buf);

        // Close the file
        filp_close(file, NULL);
    }
    // pr_info("maxoffset is %d\n", maxoffset);
    return maxoffset;
}
int64_t unpack_offset(uint64_t msr_response)
{
    int64_t x = msr_response >> 21;
    int y;
    if (x < 1024)
        y = x * 1000 / 1024;
    else
        y = (2048 - x) * 1000 / 1024;
    y = y + 1;  // rounding
    y = y * -1; // make it negative
    return (int)y;
}
int64_t plane0offset(void)
{
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

    // pr_info("MSR 0x150 value: 0x%llx\n", msr_value);
    // pr_info("offset is %lld\n", unpack_offset(msr_value));
    return unpack_offset(msr_value);
}

static int my_thread(void *data) {
             int p;

    while (!kthread_should_stop()) {
        // Your code goes here
        // pr_info("Kernel module is running...\n");
        // msleep(1000); // Sleep for 1 second
        p = maximumoffset() + 50;
        if (plane0offset() < p)
        {
            wrmsr_on_cpu(0, 0x150, (u32)(0x80000011fec00000 & 0xFFFFFFFF), (u32)(0x80000011fec00000 >> 32));
            pr_info("offset is %lld,p is %d\n", plane0offset(),p);
            // msleep(1000);
        }
    }

    return 0;
}

static struct task_struct *thread;

static int __init get_cpu_freq_init(void)
{
    // pr_info("maximumoffset: %d\n", maximumoffset());
    // pr_info("offset is %lld\n",plane0offset());
    // int p;
    // while (notStopped)
    // {
    //     p = maximumoffset() + 50;
    //     if (plane0offset() < p)
    //     {
    //         //wrmsr_on_cpu(0, 0x150, (u32)(0x80000011fec00000 & 0xFFFFFFFF), (u32)(0x80000011fec00000 >> 32));
    //         pr_info("offset is %lld\n", plane0offset());
    //         // msleep(1000);
    //     }
    // }
        pr_info("Module initialization\n");

    // Create a kernel thread that runs your loop
    thread = kthread_run(my_thread, NULL, "my_thread");

    if (IS_ERR(thread)) {
        pr_err("Failed to create kernel thread\n");
        return PTR_ERR(thread);
    }

    return 0;
}

static void __exit get_cpu_freq_exit(void)
{
    pr_info("Exiting...\n");
}

module_init(get_cpu_freq_init);
module_exit(get_cpu_freq_exit);
