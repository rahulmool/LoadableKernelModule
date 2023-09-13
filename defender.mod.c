#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xd8025f97, "kthread_create_on_node" },
	{ 0x33b2abe8, "wake_up_process" },
	{ 0x17de3d5, "nr_cpu_ids" },
	{ 0x9e683f75, "__cpu_possible_mask" },
	{ 0x53a1e8d9, "_find_next_bit" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x88139c8d, "filp_open" },
	{ 0x469dcca3, "kernel_read" },
	{ 0xbf957606, "filp_close" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0x23d1b90, "wrmsr_on_cpu" },
	{ 0x20ba4f3e, "rdmsr_on_cpu" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x122c3a7e, "_printk" },
	{ 0x453e7dc, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "A7C94DAE9642FD330B461CB");
