#include <linux/kernel.h>  // printk(), pr_*()
#include <linux/module.h>  // THIS_MODULE, MODULE_VERSION, ...
#include <linux/init.h>    // module_{init,exit}()
#include <linux/smp.h>     // get_cpu(), put_cpu()
#include <linux/cpufreq.h> // cpufreq_get()
#include <linux/cpumask.h> // cpumask_{first,next}(), cpu_online_mask

//step 2 : create variable
int param_var[3]={0,0,0};

//step 3 : register (macro)
//moduel_param(name_var,type,permission)
/*
	S_IRUSR : read permission
	S_IWUSR : write permission
	S_IXUSR : execute permission
	S_IWGRP : write permission for group
	S_IRGRP : read permission for group
	S_IRUSR | S_IWUSR : read and write permission for user
*/
#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

module_param_array(param_var,int, NULL, S_IRUSR | S_IWUSR);
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
        return 0;
}
unsigned long long x86_get_msr(int msr)
{
    unsigned long msrl = 0, msrh = 0;

    /* NOTE: rdmsr is always return EDX:EAX pair value */
    asm volatile ("rdmsr" : "=a"(msrl), "=d"(msrh) : "c"(msr));

    return ((unsigned long long)msrh << 32) | msrl;
}

void display(void){
	printk(KERN_ALERT "msr value of x150 is %d\n",x86_get_msr(0x150));
	printk(KERN_ALERT "Param[0] = %d\n",getfreqoffset(param_var[0]));
		printk(KERN_ALERT "Param[1] = %d\n",param_var[1]);
		printk(KERN_ALERT "Param[2] = %d\n",param_var[2]);
        unsigned cpu = cpumask_first(cpu_online_mask);

        while (cpu < nr_cpu_ids) {
                pr_info("CPU: %u, freq: %u kHz\n", cpu, cpufreq_get(cpu));
                cpu = cpumask_next(cpu, cpu_online_mask);
        }

}
static int hello_init(void){
	printk(KERN_ALERT "Hello, world\n");
	display();
	return 0;
}
static void hello_exit(void){
	printk(KERN_ALERT "Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("OSLAB");
MODULE_DESCRIPTION("A simple Hello World Module");

