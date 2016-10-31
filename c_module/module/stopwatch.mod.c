#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xbf073a9e, "module_layout" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x6c8d5ae8, "__gpio_get_value" },
	{ 0x89e96c2, "__register_chrdev" },
	{ 0x859c6dc7, "request_threaded_irq" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x11f447ce, "__gpio_to_irq" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x81701d44, "device_create" },
	{ 0x97255bdf, "strlen" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0x45a55ec8, "__iounmap" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0x7426f76c, "init_timer_key" },
	{ 0xef6febb0, "kmalloc_caches" },
	{ 0x4aabc7c4, "__tracepoint_kmalloc" },
	{ 0x9fb1b2ac, "kmem_cache_alloc_notrace" },
	{ 0x8a7c6f1, "add_timer" },
	{ 0x37e74642, "get_jiffies_64" },
	{ 0x37a0cba, "kfree" },
	{ 0x1b945caf, "del_timer" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x2196324, "__aeabi_idiv" },
	{ 0xea147363, "printk" },
	{ 0xff178f6, "__aeabi_idivmod" },
	{ 0x9d669763, "memcpy" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

