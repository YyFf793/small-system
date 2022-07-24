#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

int ts_fd;

int main(void)
{
	//1，打开触摸屏
	ts_fd = open("/dev/input/event0", O_RDWR);

	if (ts_fd == -1) {
		printf("open ts device failed!\n");
		return -1;
	}
	//2，获取（读取）绝对坐标值
	struct input_event ts;
	
	while (1) {
		read(ts_fd, &ts, sizeof(ts));
		printf("type : %d\t", ts.type);
		printf("code : %d\t", ts.code);
		printf("value : %d\n", ts.value);
	}
	//3，关闭触摸屏
	close(ts_fd);
	
	return 0;
}