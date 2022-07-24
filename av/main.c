#include <stdio.h>
#include "av.h"
#include "lcdjpg.h"
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int lcd_fd, ts_fd, av_fd;
int *lcd_ptr;

int dev_init()
{
	lcd_fd = open("/dev/fb0", O_RDWR);

	if (lcd_fd == -1) {
		printf("open lcd device failed!\n");
		return -1;
	}

	//为lcd屏幕建立内存映射关系
	lcd_ptr = (int*)mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);

	if (lcd_ptr ==  MAP_FAILED) {
		printf("mmap failed!\n");
		return -2;
	}
	
	ts_fd = open("/dev/input/event0", O_RDONLY);

	if (ts_fd == -1) {
		printf("open ts device failed!\n");
		return -3;
	}
	
	mplayer_init();
	
	return 0;
}

int dev_uninit()
{
	munmap(lcd_ptr, 800*480*4);
	close(lcd_fd);
	close(ts_fd);
}

int gif()
{
	int i = 0;
	char buf[1024];

	for (i = 0; i < 37; i++) {
		//printf("%s\n", buf);
		if (i >= 0 && i < 10) {
			sprintf(buf, "IMG0000%d.jpg", i);
		}else if (i >= 10 && i < 99) {
			sprintf(buf, "IMG000%d.jpg", i);
		}
		lcd_draw_jpg(150, 40,buf, NULL, 0, 0);
		usleep(30000);
	}
}

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

int video()
{
	int x, y;
	int count = 0;
	char play_cmd[1024];
	char mv_name[10][1024] = {"1.mp4", "2.mp4", "3.mp4"};
	
	while (1) {
		get_xy(&x, &y);
		printf("(%d, %d)\n", x, y);
		if (x >= 350 && x < 450 && y >= 380 && y < 480) {
			sprintf(play_cmd,"mplayer   %s  -slave  -input file=/tmp/fifo  -zoom -x 600 -y 430 -geometry 100:0 &",mv_name[count]);
			system(play_cmd);
		}else if (x >= 700 && x < 800 && y >= 380 && y < 480) {
			mute();
		}
	}
	
	return 0;
}

int main(void)
{
	
	//1，设备的初始化
	dev_init();
	
	//2，电影播放器
	video();

	//3，设备的卸载
	dev_uninit();

	return 0;
}
