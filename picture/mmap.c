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


//const char* pathname :��Ҫ��ʾ��ͼƬ�ļ���
//int x, int y :ָ��λ��
//int w, int h :�̶���С
int lcd_draw_bmp(const char *pathname, int x, int y, int w, int h)
{
	//1����ͼƬ�ļ�
	int bmp_fd = open(pathname, O_RDWR);
	
	if (bmp_fd == -1) {
		printf("open bmp file failed!\n");
		return -1;
	}
    
    //2����ȡͼƬ��ɫ����
	char rgb_buf[w*h*3];
	char header[54];
	
	read(bmp_fd, header, 54);
	read(bmp_fd, rgb_buf, w*h*3);
	
	//3���ر�ͼƬ�ļ�
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
			//���µߵ�
			lcd_draw_point(i+x, h-1-j+y, color);
			//*(lcd_ptr+i+(479-j)*800) = color;
			//lcd_ptr[i+(479-j)*800] = color;
		}
	}

    return 0;
}

int main(void)
{
		//4����lcd�豸�ļ�
	int lcd_fd = open("/dev/fb0", O_RDWR);
	
	if (lcd_fd == -1) {
		printf("open lcd device failed!\n");
		return -1;
	}
	
	//Ϊlcd��Ļ�����ڴ�ӳ���ϵ
	lcd_ptr = (int*)mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
	
	if (lcd_ptr ==  MAP_FAILED) {
		printf("mmap failed!\n");
		return -1;
	}
	
		
	/* //���µߵ� 0 479 1 478 2 477 3 476 .. 
	for (j = 0; j < 480; j++) {
		for (i = 0; i < 800; i++) {
			lcd_buf[i+j*800] = color_buf[(480-1-j)*800+i];
		}
	} */

    lcd_draw_bmp("add.bmp", 300, 180, 200, 120);

	
	//6���ر�lcd�豸�ļ�
	close(lcd_fd);
	
	return 0;
}
