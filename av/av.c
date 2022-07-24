#include "av.h"


//播放器初始化
void mplayer_init()
{
	//在临时目录创建管道文件
	if (access("/tmp/fifo",F_OK)) {
		mkfifo("/tmp/fifo",0777);
	}
	
	//打开管道文件
	av_fd = open("/tmp/fifo",O_RDWR);
}


//发送指令
void send_cmd(char *cmd)
{
	write(av_fd, cmd, strlen(cmd));
}

//继续播放
int res_play()
{
	system("killall -18  mplayer");
}


//暂停播放
int pause_playback()
{
	send_cmd("pause\n");
}

//停止播放
int stop_play()
{
	system("killall -9  mplayer");
}

//音量增加
int vol_up()
{
	send_cmd("volume +3000\n");
}

//音量减少
int vol_down()
{
	send_cmd("volume -3000\n");
}


//快进
int fast_forward()
{
	send_cmd("seek +5\n");
}

//快退
int my_rewind()
{
	send_cmd("seek -5\n");
}

//静音
int mute()
{
	send_cmd("mute\n");
}
