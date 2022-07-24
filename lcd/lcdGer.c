#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	
	int buf[800*480];
	
	int i;
	int j; 
	int color1= 0x00000000;//? 
	int color2= 0x00ff0000;//? 
	int color3= 0x00ffff00;//? 
	for ( j = 0; j < 160; j++) {
		for (i = 0; i < 800; i++) {
			//buf[] = 0x00ffff00;
			buf[i+j*800] = color1;		//2*800+200
		}
	}
	
		for ( j = 160; j < 320; j++) {
		for (i = 0; i < 800; i++) {
			//buf[] = 0x00ffff00;
			buf[i+j*800] = color2;		//2*800+200
		}
	}
	
		for ( j = 320; j < 480; j++) {
		for (i = 0; i < 800; i++) {
			//buf[] = 0x00ffff00;
			buf[i+j*800] = color3;		//2*800+200
		}
	}
	
	int lcd = open("/dev/fb0",O_RDWR);
	if(lcd==-1){
		printf("open lcd device faild!\n");
		return -1;
}

	write(lcd,buf,800*480*4);

	close(lcd);

	return 0;
}
