#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define DEVICE_PATH "/dev/base64" // Device path
#define ENCRYPT _IOW(0, 0, unsigned long) // Encryption control code
#define DECRYPT _IOW(0, 1, unsigned long) // Decryption control code
#define BUFFER_SIZE 4096

void print_yellow(char* message) {
    printf("\033[1;33m%s\033[0m", message);
}

void print_green(char* message) {
    printf("\033[1;32m%s\033[0m", message);
}

int main() {
    int fd;
    char message[] = "Shall I compare thee to a summer's day?\nThou art more lovely and more temperate:\nRough winds do shake the darling buds of May,\nAnd summer's lease hath all too short a date:\nSometime too hot the eye of heaven shines,\nAnd often is his gold complexion dimm'd;\nAnd every fair from fair sometime declines,\nBy chance or nature's changing course untrimm'd;";
    char encrypted[BUFFER_SIZE] = {0};
    char decrypted[BUFFER_SIZE] = {0};
    int len;

    // Open the device file
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device...");
        return -1;
    }

    // encode
    len = strlen(message) + 1;
    write(fd, message, len);
    ioctl(fd, ENCRYPT, 0);
    read(fd, encrypted, 0);

    // decode
    len = strlen(encrypted) + 1;
    write(fd, encrypted, len);
    ioctl(fd, DECRYPT, 0);
    read(fd, decrypted, 0);

    // close the device file
    close(fd);

    print_yellow("Original: \n");
    print_green(message);
    print_yellow("\nEncoded: \n");
    print_green(encrypted);
    print_yellow("\nDecoded: \n");
    print_green(decrypted);


    return 0;
}
