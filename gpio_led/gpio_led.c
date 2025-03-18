#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "gpio_led"
#define GPIO_LED_PIN (17)

static int gpio_led_value = 0;

static ssize_t gpio_led_write(struct file *file, const char __user *buf, size_t count, loff_t *pos) {
    char kbuf[4];

    if (copy_from_user(kbuf, buf, count)) {
        return -EFAULT;
    }

    kbuf[count] = '\0';

    if (strcmp(kbuf, "1") == 0) {
        gpio_led_value = 1;
        gpio_set_value(GPIO_LED_PIN, gpio_led_value);
    } else if (strcmp(kbuf, "0") == 0) {
        gpio_led_value = 0;
        gpio_set_value(GPIO_LED_PIN, gpio_led_value);
    } else {
        return -EINVAL;
    }

    return count;
}

static ssize_t gpio_led_read(struct file *file, char __user *buf, size_t count, loff_t *pos) {
    char kbuf[4];
    sprintf(kbuf, "%d\n", strlen(kbuf));

    if (copy_to_user(buf, kbuf, strlen(kbuf))) {
        return -EFAULT;
    }

    return strlen(kbuf);
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = gpio_led_write,
    .read = gpio_led_read
};

static int __init gpio_led_init(void) {
    int result;

    if (!gpio_is_valid(GPIO_LED_PIN)) {
        printk(KERN_ALERT "Failed to register char device \n");
        return result;
    }

    result = register_chrdev(0, DEVICE_NAME, &fops);
    if (result < 0) {
        printk(KERN_ALERT "Failed to register char device!\n");
        return -EINVAL;
    }
    
    printk(KERN_INFO "Gpio led driver initialized!\n");
    return 0;
}

static void __exit gpio_led_exit(void) {
    gpio_set_value(GPIO_LED_PIN, 0);
    gpio_free(GPIO_LED_PIN);
    unregister_chrdev(0, DEVICE_NAME);
    printk(KERN_INFO "GPIO Led driver removed\n");
}

module_init(gpio_led_init);
module_exit(gpio_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("FernandezKA");
MODULE_DESCRIPTION("A simple GPIO LED driver");

