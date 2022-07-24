#include <stdio.h>
#include "lcdjpg.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int lcd_fd;
int *lcd_ptr;


int main(void)
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
		return -1;
	}


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


	munmap(lcd_ptr, 800*480*4);

	close(lcd_fd);

	return 0;
}
