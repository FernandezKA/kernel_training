#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FernandezKA");
MODULE_DESCRIPTION("Hello kernel world");

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello kernel world\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye kernel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
