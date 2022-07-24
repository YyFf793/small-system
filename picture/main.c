#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	//1，打开图片文件
	int bmp_fd = open("1.bmp", O_RDWR);
	
	if (bmp_fd == -1) {
		printf("open bmp file failed!\n");
		return -1;
	}
	//2，获取图片颜色数据
	char rgb_buf[800*480*3];
	
	read(bmp_fd, rgb_buf, 800*480*3);
	
	//3，关闭图片文件
	close(bmp_fd);
	
	//4，打开lcd设备文件
	int lcd_fd = open("/dev/fb0", O_RDWR);
	
	if (lcd_fd == -1) {
		printf("open lcd device failed!\n");
		return -1;
	}
	//5，往lcd写入图片颜色数据
	write(lcd_fd, rgb_buf, 800*480*3);
	//6，关闭lcd设备文件
	close(lcd_fd);
	
	return 0;
}