#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "base64" // Device name
#define ENCRYPT _IOW(0, 0, unsigned long) // Encryption control code
#define DECRYPT _IOW(0, 1, unsigned long) // Decryption control code
#define BUFFER_SIZE 4096

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xun Qin");

static int major_number; // Major device number

char buffer[BUFFER_SIZE]; // Store the message from user space
char result[BUFFER_SIZE]; // Store the result

// Base64 encode table
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Base64 encode
static void base64_encode(char *message, char *result) {
    printk(KERN_INFO "Base64 encoding message: %s\n", message);
    int i = 0, j = 0;
    int len = strlen(message);
    while (i < len) {
        int a = message[i++];
        int b = (i < len) ? message[i++] : 0;
        int c = (i < len) ? message[i++] : 0;
        int triple = (a << 0x10) + (b << 0x08) + c;
        result[j++] = base64_table[(triple >> 3 * 6) & 0x3F];
        result[j++] = base64_table[(triple >> 2 * 6) & 0x3F];
        result[j++] = base64_table[(triple >> 1 * 6) & 0x3F];
        result[j++] = base64_table[(triple >> 0 * 6) & 0x3F];
    }
    while (j % 4) {
        result[j++] = '=';
    }
    // Add null terminator
    result[j] = '\0';
}

// Base64 decode
static void base64_decode(char *message, char *result) {
    printk(KERN_INFO "Base64 decoding message: %s\n", message);
    int i = 0, j = 0;
    int len = strlen(message);
    while (i < len) {
        int a = strchr(base64_table, message[i++]) - base64_table;
        int b = strchr(base64_table, message[i++]) - base64_table;
        int c = strchr(base64_table, message[i++]) - base64_table;
        int d = strchr(base64_table, message[i++]) - base64_table;
        int triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);
        result[j++] = (triple >> 2 * 8) & 0xFF;
        result[j++] = (triple >> 1 * 8) & 0xFF;
        result[j++] = (triple >> 0 * 8) & 0xFF;
    }
    while (result[j - 1] == '=') {
        j--;
    }
    // Add null terminator
    result[j] = '\0';
}

// Use base64 encoding 
static void encrypt(char *message, char *result) {
    base64_encode(message, result);
}

// Use base64 decoding
static void decrypt(char *message, char *result) {
    base64_decode(message, result);
}

static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    try_module_get(THIS_MODULE); // Increase module reference count
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device released\n");
    module_put(THIS_MODULE); // Decrease module reference count
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t len, loff_t *offset) {
	len = strlen(result) + 1;
    copy_to_user(buffer, result, len); // Copy the result to user space
    printk(KERN_INFO "Read message: %s\n", result);
    return len;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
    copy_from_user(buffer, buff, len); // Copy the message from user space
    printk(KERN_INFO "Write message: %s\n", buffer);
    return len;
}

// Function controlling encryption and decryption
static ssize_t device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case ENCRYPT:
            encrypt(buffer, result);
            break;
        case DECRYPT:
            decrypt(buffer, result);
            break;
        default:
            printk(KERN_ALERT "Invalid command\n");
            return -EINVAL;
    }
    return 0;
}

// File operation structure
static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl
};

// Module initialization function
static int __init hello_init(void) {
	printk(KERN_INFO "Hello, world!\n");
    major_number = register_chrdev(0, DEVICE_NAME, &fops); // Register device and get major device number
    if (major_number < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", major_number);
        return major_number;
    }
    printk(KERN_INFO "Assigned major number %d. To create a dev file with 'mknod /dev/%s c %d 0'.\n", major_number, DEVICE_NAME, major_number);

    return 0;
}

// Module cleanup function
static void __exit hello_exit(void) {
	printk(KERN_INFO "Goodbye, world!\n");
    unregister_chrdev(major_number, DEVICE_NAME); // Unregister device
}

module_init(hello_init);
module_exit(hello_exit);
