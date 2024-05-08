# CSC415-Device-Driver
**Background**:

The Linux operating system consists of the Kernel some other minor components and then a large number of Device Drivers.  Device drivers are the key to how various hardware devices interface with the computer.

**Task**:

Develop a device driver that can be loaded and run in Linux.  Then add some functionality to the device driver such as the user/application passing in a string to the device driver and the device driver returns an encrypted version of the string or passes in the excrypted string and returns the original string.  Include a document on how to build, load and interact with the device driver (after the description in the write up template) along with screen shots of output.

**Requirements**:
It must be written in C.  It must be a valid an loadable device driver with at least some user/application functionality. That includes an open, release, read, write, and at least one ioctl command.  It must also be able to be unloaded, and indicate that it has unloaded from the system.  Make use of the printk and copy_to_user functions.  The read and write functions should follow the concept of linux files and be relevent to reading or writing.

Part of the grading of the driver will be based on the functionality you choose to implement.  It can not be trivial functionality.

This must be run in the linux virtual machine.

You must also write a user application that utilizes your device driver.

There are TWO directories in your GitHub:  The first is `Module` that has your kernel module.  The second is `Test` that will have your test user application.

The writeup should have a detailed description of what your device driver does and clear instructions on how to build your kernel module and your test program and how to install the kernel module and how to use your test program.

**Example**:
Encrypt data, read and write a string, use ioctl to determine to encrypt or decrypt and to set an key. If ioctl set to encrypt, write a string, then read back the encrypted data.  Switch ioctl to decrypt then write the encrypted string and read back the original message (decrypted).

**NOTE:**
No basic calculator or palindrome allowed!

**Submission**:

Submit your write-up as a PDF on Canvas, and your code and Makefile (modify that provided) in Git along with the PDF.

**Hint**:
The provided module makefile is a key to building kernel modules.

Make sure there are no spaces in the entire path to the Module directory.

**Grading**:

This project will be graded based on meeting the following rubric. 

**Rubric**: 
|Component 	| Points |
|:------------------------------------------------------------------------|------:|
|Loadable Device Driver with Functionality as specified                   |	 35   |
|Correctness of load and unload functions                                 |	 10   |
|Test  -  user/application interaction with the device driver     	  |  	20  |
|Inline comments - meaningful describing concepts and functionality       | 	13  |
|Standard Header a define in all prior projects for each file             |  	2   |
|Write-up of project                                                      |  20   |
|Write-up includes Screen shots showing all elements of functionality     |       |
|     (include load and unload as well as application interaction         |     |

	

	
