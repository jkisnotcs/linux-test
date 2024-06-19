 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include <sys/ioctl.h>
 
#define WR_VALUE _IOW(1, 1, int32_t *)
#define RD_VALUE _IOR(1, 2, int32_t *)

int main()
{
    int fd;
    int32_t value, number;

    fd = open("/dev/my_device", O_RDWR);
    if (fd < 0)
    {
        printf("Failed to open /dev/my_device\n");
        return -1;
    }

    printf("Enter the Value to send: \n");
    scanf("%d", &number);
    ioctl(fd, WR_VALUE, (int32_t *)&number);
    ioctl(fd, RD_VALUE, (int32_t *)&value);
    printf("Value read from Driver: %d\n", value);

    close(fd);
    return 0;
}