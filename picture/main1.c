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
	int color_buf[800*480];
	int lcd_buf[800*480];
	char header[54];
	
	read(bmp_fd, header, 54);
	read(bmp_fd, rgb_buf, 800*480*3);
	
	//3，关闭图片文件
	close(bmp_fd);
	
	//4，打开lcd设备文件
	int lcd_fd = open("/dev/fb0", O_RDWR);
	
	if (lcd_fd == -1) {
		printf("open lcd device failed!\n");
		return -1;
	}
	
	//24 --> 32
	int i, j, k = 0;
	
	for (j = 0; j < 480; j++) {
		for (i = 0; i < 800; i++) {
			int b = rgb_buf[k++];		//k = k+1;
			int g = rgb_buf[k++];
			int r = rgb_buf[k++];
			
			int color = b;
			color |= (g << 8);
			color |= (r << 16);
			
			color_buf[i+j*800] = color;
		}
	}
	
	//上下颠倒 0 479 1 478 2 477 3 476 .. 
	for (j = 0; j < 480; j++) {
		for (i = 0; i < 800; i++) {
			lcd_buf[i+j*800] = color_buf[(480-1-j)*800+i];
		}
	}
	
	//5，往lcd写入图片颜色数据
	write(lcd_fd, lcd_buf, 800*480*4);
	//6，关闭lcd设备文件
	close(lcd_fd);
	
	return 0;
}
