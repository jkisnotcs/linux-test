#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/err.h>

#define WR_VALUE _IOW(1, 1, int32_t *)
#define RD_VALUE _IOR(1, 2, int32_t *)

int32_t value = 0;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev;

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
	{
    case WR_VALUE:
        return copy_from_user(&value, (int32_t *)arg, sizeof(value));
    case RD_VALUE:
        return copy_to_user((int32_t *)arg, &value, sizeof(value));
    }
    return 0;
}

static struct file_operations fops = {
   .owner = THIS_MODULE,
   .unlocked_ioctl = my_ioctl,
};

static int __init my_driver_init(void)
{
    if ((alloc_chrdev_region(&dev, 0, 1, "my_dev")) < 0)
        return -1;

    cdev_init(&my_cdev, &fops);

    if ((cdev_add(&my_cdev, dev, 1)) < 0)
        goto r_class;

    if (IS_ERR(dev_class = class_create(THIS_MODULE, "my_class")))
        goto r_class;

    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "my_device")))
        goto r_device;

    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit my_driver_exit(void)
{
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("qinchi1@h-partner.com");
MODULE_DESCRIPTION("Simple Linux device driver");
MODULE_VERSION("1.5");