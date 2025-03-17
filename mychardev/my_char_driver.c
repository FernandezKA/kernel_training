/**
 * @file my_char_driver.c
 * @author FernandezKA (fernandes.kir@yandex.ru)
 * @brief
 * @version 0.1
 * @date 2025-03-17
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychardev"
#define BUFFER_SIZE 1024

static char device_buffer[BUFFER_SIZE];
static int device_open_count = 0;

static int device_open(struct inode *inode, struct file *file)
{
    if (device_open_count)
    {
        return -EBUSY;
    }

    device_open_count++;
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    device_open_count--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{

    size_t bytes_to_read = strlen(device_buffer) - *offset;

    if (bytes_to_read <= 0)
    {
        return 0;
    }

    if (len < bytes_to_read)
    {
        bytes_to_read = len;
    }

    if (copy_to_user(buf, device_buffer + *offset, bytes_to_read))
    {
        return -EFAULT;
    }

    *offset += bytes_to_read;
    return bytes_to_read;
}

static ssize_t device_write(struct file* file, const char __user *buf, size_t len, loff_t *offset) {
    if (len > BUFFER_SIZE) {
        len = BUFFER_SIZE - 1;
    }

    if (copy_from_user(device_buffer, buf, len)) {
        return -EFAULT;
    }

    device_buffer[len] = '\0';
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write
};

static int __init my_char_driver_init(void) {
    int result = register_chrdev(0, DEVICE_NAME, &fops);
    if (result < 0) {
        printk(KERN_ALERT "Failed to register char device %d\n", result);
        return result;
    }

    printk(KERN_INFO "Char device registered with major number%d \n", result);
    return 0;
}

static void __exit my_char_driver_exit(void) {
    unregister_chrdev(0, DEVICE_NAME);
    printk(KERN_INFO "Char device unregister\n");
}

module_init(my_char_driver_init);
module_exit(my_char_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FernandezKA");
MODULE_DESCRIPTION("Simple Character Device Driver");
