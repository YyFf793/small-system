#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int *lcd_ptr;

int lcd_draw_point(int x, int y, int color)
{
	//lcd_ptr[x+y*800] = color;
	*(lcd_ptr+x+y*800) = color;
}


//const char* pathname :想要显示的图片文件名
//int x, int y :指定位置
//int w, int h :固定大小
int lcd_draw_bmp(const char *pathname, int x, int y, int w, int h)
{
	//1，打开图片文件
	int bmp_fd = open(pathname, O_RDWR);
	
	if (bmp_fd == -1) {
		printf("open bmp file failed!\n");
		return -1;
	}
    
    //2，获取图片颜色数据
	char rgb_buf[w*h*3];
	char header[54];
	
	read(bmp_fd, header, 54);
	read(bmp_fd, rgb_buf, w*h*3);
	
	//3，关闭图片文件
	close(bmp_fd);

    //24 --> 32
	int i, j, k = 0;
	
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			int b = rgb_buf[k++];		//k = k+1;
			int g = rgb_buf[k++];
			int r = rgb_buf[k++];
			
			int color = b;
			color |= (g << 8);
			color |= (r << 16);
			//上下颠倒
			lcd_draw_point(i+x, h-1-j+y, color);
			//*(lcd_ptr+i+(479-j)*800) = color;
			//lcd_ptr[i+(479-j)*800] = color;
		}
	}

    return 0;
}

int main(void)
{
		//4，打开lcd设备文件
	int lcd_fd = open("/dev/fb0", O_RDWR);
	
	if (lcd_fd == -1) {
		printf("open lcd device failed!\n");
		return -1;
	}
	
	//为lcd屏幕建立内存映射关系
	lcd_ptr = (int*)mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
	
	if (lcd_ptr ==  MAP_FAILED) {
		printf("mmap failed!\n");
		return -1;
	}
	
		
	/* //上下颠倒 0 479 1 478 2 477 3 476 .. 
	for (j = 0; j < 480; j++) {
		for (i = 0; i < 800; i++) {
			lcd_buf[i+j*800] = color_buf[(480-1-j)*800+i];
		}
	} */

    lcd_draw_bmp("add.bmp", 300, 180, 200, 120);

	
	//6，关闭lcd设备文件
	close(lcd_fd);
	
	return 0;
}
