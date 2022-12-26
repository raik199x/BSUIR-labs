#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include <asm/io.h>

#define CFX_DEVICE "tcfxm"
#define KLOG KERN_CONT

const u32 PCI_ENABLE_BIT     = 0x80000000;
const u16 PCI_CONFIG_ADDRESS = 0xCF8;
const u16 PCI_CONFIG_DATA    = 0xCFC;

/*!
 * \brief Meta Inforrmation
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TimuriAlvarez");
MODULE_DESCRIPTION("Linux I/O port access kernel module");

/*!
 * \brief Device access functions declarations
 */
static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);

/*!
 * \brief Device access structure that binds these functions
 */
static struct file_operations fops = {
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};

/*!
 * \brief Major device number
 */
static int major;
#define DYNAMIC_MAJOR_NUMBER 0

/*!
 * \brief Kernel module loader
 */
static int __init tcfxm_init(void) {
	major = register_chrdev(DYNAMIC_MAJOR_NUMBER, CFX_DEVICE, &fops);
	if (major < 0)
	{
		printk(KERN_ALERT "tcfx module load failed: device registration failed with code %d\n", major);
		return major;
	}
	printk(KLOG "tcfx module has been loaded.\n");
	printk(KLOG "tcfx major value: %d.\n", major);
	return 0;
}

/*!
 * \brief Kernel module unloader
 */
static void __exit tcfxm_exit(void) {
	unregister_chrdev(major, CFX_DEVICE);
	printk(KLOG "tcfx module has been unloaded.\n");
	return;
}

/*!
 * \brief Device access functions definitions
 */
#define TCFX_SIZE 4
static u32 retvalue = 0;
static u8 mutex = 0;

#define TAKE_MUTEX(MTX) MTX = 1
#define GIVE_MUTEX(MTX) MTX = 0
#define IS_MTX_TKN(MTX) (MTX != 0)

static int dev_open(struct inode *inodep, struct file *filep) {
	if IS_MTX_TKN(mutex) return -EFAULT;
	TAKE_MUTEX(mutex);
	//printk(KLOG "cfx device opened\n");
	return 0;
}

static ssize_t dev_write(struct file *filep, const char* buffer, size_t len, loff_t *offset) {
	u8 bus, slot, func, pcireg;
	u32 tmpvalue;
	ssize_t errors = 0;
	u8 message[TCFX_SIZE];

	/* get buffer size */
	size_t message_size = len;

	if (message_size > TCFX_SIZE ) {
		message_size = TCFX_SIZE;
	}
	/* write data to the buffer */
	errors = copy_from_user(message, buffer, message_size);
	if (errors == 0)
	{
		bus = message[0];
		slot = message[1];
		func = message[2];;
		pcireg = message[3];
		//printk(KLOG "Resiving: %lu\n", message_size);
		tmpvalue = PCI_ENABLE_BIT | (bus << 16) | (slot << 11) | (func << 8) | (pcireg << 2);
		outl(tmpvalue, PCI_CONFIG_ADDRESS);
		retvalue = inl(PCI_CONFIG_DATA);
		if ((bus == 1 && slot == 0 && func == 0) || retvalue != 0xFFFFFFFF)
			printk(KLOG "tcfx module:\t%02x:%02x.%02x:\t0x%x -> 0x%x\t0x%08x -> 0x%08x\n", bus, slot, func, tmpvalue, PCI_CONFIG_ADDRESS, PCI_CONFIG_DATA, retvalue);
	}
	return errors == 0 ? message_size : -EFAULT;
}

static int dev_release(struct inode *inodep, struct file *filep) {
	GIVE_MUTEX(mutex);
	//printk(KLOG "cfx device closed\n");
	return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
	ssize_t errors = 0;
	u8 message[TCFX_SIZE];

	message[0] = retvalue & 0xFF;
	retvalue = retvalue >> 8;
	message[1] = retvalue & 0xFF;
	retvalue = retvalue >> 8;
	message[2] = retvalue & 0xFF;
	retvalue = retvalue >> 8;
	message[3] = retvalue & 0xFF;

	//printk(KLOG "Sending: %lu\n", TCFX_SIZE);
	errors = copy_to_user(buffer, message, TCFX_SIZE);

	return errors == 0 ? TCFX_SIZE : -EFAULT;
}

/*!
 * \brief Kernel module functions binding
 */
module_init(tcfxm_init);
module_exit(tcfxm_exit);
