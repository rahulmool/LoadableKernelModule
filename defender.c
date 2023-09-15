#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#define MSR_IA32_PERF_STATUS2 0x198 // MSR for reading CPU frequency


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kernel Module to Get Actual CPU Frequencies for All CPUs");
MODULE_VERSION("0.1");

uint64_t msr_value(int64_t val,uint64_t plane)
{
	//0.5 is to deal with rounding
	val=(val*1024/1000);
	val=0xFFE00000&((val&0xFFF)<<21);
	val=val|0x8000001100000000;
	val=val|(plane<<40);
	return (uint64_t)val;
}

int getfreqoffset(int freq){
        if(0<=freq && freq<=650){           return -98;//assume
        }else if(650<=freq && freq<=750){   return -98;//crash
        }else if(750<=freq && freq<=850){   return -93;//crash
        }else if(850<=freq && freq<=950){   return -98;//crash
        }else if(950<=freq && freq<=1500){  return -83;//crash
        }else if(1050<=freq && freq<=1150){ return -86;//crash
        }else if(1150<=freq && freq<=1250){ return -87;
        }else if(1250<=freq && freq<=1350){ return -83;
        }else if(1350<=freq && freq<=1450){ return -83;
        }else if(1450<=freq && freq<=1550){ return -70;
        }else if(1550<=freq && freq<=1650){ return -71;
        }else if(1650<=freq && freq<=1750){ return -71;
        }else if(1750<=freq && freq<=1850){ return -69; 
        }else if(1850<=freq && freq<=1950){ return -57; 
        }else if(1950<=freq && freq<=2050){ return -57; 
        }else if(2050<=freq && freq<=2150){ return -55; 
        }else if(2150<=freq && freq<=2250){ return -57; 
        }else if(2250<=freq && freq<=2350){ return -53; 
        }else if(2350<=freq && freq<=2450){ return -56; 
        }else if(2450<=freq && freq<=2550){ return -60; 
        }else if(2550<=freq && freq<=2650){ return -64; 
        }else if(2650<=freq && freq<=2750){ return -67; 
        }else if(2750<=freq && freq<=2850){ return -68; 
        }else if(2850<=freq && freq<=2950){ return -70;
        }else if(2950<=freq && freq<=3050){ return -74;
        }else if(3050<=freq && freq<=3150){ return -75;
        }else if(3150<=freq && freq<=3250){ return -78;
        }else if(3250<=freq && freq<=3350){ return -76;
        }else if(3350<=freq && freq<=3450){ return -91;
        }else if(3450<=freq && freq<=3550){ return -100;
        }else if(3550<=freq && freq<=3650){ return -104;
        }else if(3650<=freq && freq<=3750){ return -109;
        }else if(3750<=freq && freq<=3850){ return -113;
        }else if(3850<=freq && freq<=3950){ return -123;
        }else if(3950<=freq && freq<=4050){ return -124;
        }else if(4050<=freq && freq<=4150){ return -126;
        }else if(4150<=freq && freq<=4250){ return -129;
        }else if(4250<=freq && freq<=4350){ return -133;//crash
        }else if(4350<=freq && freq<=4450){ return -108;//crash
        }else if(4450<=freq && freq<=4550){ return -109;//crash
        }else if(4550<=freq && freq<=4650){ return -122;//crash
        }else if(4650<=freq && freq<=4750){ return -121;//crash
        }else if(4750<=freq && freq<=4850){ return -121;//assume
        }else if(4850<=freq && freq<=4950){ return -121;//assume
        } 
        //pr_info("Failed to read %u\n", freq);
        return -1;
}

int maximumoffset(void)
{
    int maxoffset = -1;
    unsigned int cpu;
    unsigned int freq;
    int t;
    
    for_each_online_cpu(cpu) {
        u64 msr_value;
        rdmsrl_on_cpu(cpu, MSR_IA32_PERF_STATUS2, &msr_value);
        
        // Extract frequency information from the MSR value
        freq = (msr_value >> 8) & 0xFF;
        if(cpu==0){
            maxoffset=getfreqoffset(freq*100);
        }else{
            t=getfreqoffset(freq*100);
            if(maxoffset<t){
                maxoffset=t;
            }
        }

        // pr_info("CPU%u Frequency: %u MHz\n", cpu, freq);
    }
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
             int p,k;

    while (!kthread_should_stop()) {
        // Your code goes here
        // pr_info("Kernel module is running...\n");
        // msleep(1000); // Sleep for 1 second
        p = maximumoffset() + 20;
        k=plane0offset();
        if (k < p)
        {
            //pr_info("offset is %lld,p is %d\n", plane0offset(),p);
            wrmsr_on_cpu(0, 0x150, (u32)(msr_value(p,0) & 0xFFFFFFFF), (u32)(msr_value(p,0) >> 32));
            wrmsr_on_cpu(0, 0x150, (u32)(msr_value(p,2) & 0xFFFFFFFF), (u32)(msr_value(p,2) >> 32));
            // msleep(1000);
        }else{
            wrmsr_on_cpu(0, 0x150, (u32)(msr_value(-2,2) & 0xFFFFFFFF), (u32)(msr_value(-2,2) >> 32));
            
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
        //pr_info("Module initialization\n");

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
    //pr_info("Exiting...\n");
}

module_init(get_cpu_freq_init);
module_exit(get_cpu_freq_exit);
