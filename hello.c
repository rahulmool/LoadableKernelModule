#include<linux/init.h>
#include<linux/module.h>
//step 1 : include header file
#include<linux/moduleparam.h>

//step 2 : create variable
int param_var=0;

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
module_param(param_var,int,S_IRUSR | S_IWUSR);
void display(void){
	printk(KERN_ALERT "Param value = %d\n",param_var);
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

