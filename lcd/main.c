#include <stdio.h>
#include "lcdjpg.h"

int ts_fd, lcd_fd;
int *lcd_ptr;

int get_xy(int *x, int *y)
{
	struct input_event ts;
	int x_read = 0, y_read = 1;
	
	while (1) {
		read(ts_fd, &ts, sizeof(ts));
		
		//绝对坐标事件(x, y)  (y (y (x, (x, (y, fin up
		if (ts.type == EV_ABS) {
			if (ts.code == ABS_X && x_read == 0) {
				*x = ts.value;
				//printf("(%d,", ts.value);
				x_read = 1;
				y_read = 0;
			}
			if (ts.code == ABS_Y && y_read == 0) {
				*y = ts.value;
				//printf(" %d)\n", ts.value);
				x_read = 0;
				y_read = 1;
			}
		}
		
		//键盘事件
		if (ts.type == EV_KEY) {
			if (ts.code == BTN_TOUCH ) {
				if (ts.value == KEY_RESERVED) {
					printf("fin up\n");
					break;
				}else if (ts.value == KEY_ESC) {
					printf("fin down\n");
				}
			}
		}
	}
	
	return 0;
}


int main(void)
{
	//1，打开触摸屏
	ts_fd = open("/dev/input/event0", O_RDWR);

	if (ts_fd == -1) {
		printf("open ts device failed!\n");
		return -1;
	}
	
	lcd_fd = open("/dev/fb0", O_RDWR);
	
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
	
	array[10][1024] = {"1.jpg", "2.jpg", "3.jpg"};
	//0~2
	int x, y;
	int count = 0;
	
	while (1) {
		get_xy(&x, &y);
		printf("(%d, %d)\n", x, y);
		if (x >= 0 && x < 400 && y >= 0 && y < 480) {
		//if (x >= 0 && x < 512 && y >= 0 && y < 600) {
			count--;
			//控制
			if(count == -1) {
				count = 2;
			}
			lcd_draw_jpg(0,0, array[count],NULL, 0, 0);
		}else if (x >= 400 && x < 800 && y >= 0 && y < 480) {
		//}else if (x >= 512 && x < 1024 && y >= 0 && y < 600) {
			
		}
	}
	
	
	munmap(lcd_ptr, 800*480*4);
	close(ts_fd);
	close(lcd_fd);
	
	
	return 0;
}