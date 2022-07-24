#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>//basename():从路径中获取文件名及后缀
 
 
int main(int argc, char * argv[])
{
	if(argc != 3) {
		printf("Please enter the PORT number and IP of the file receiver\n");
		return -1;
	}
	
	int port = atoi(argv[2]);
	
	//创建TCP通信socket
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_socket == -1)
	{
		perror("create socketfd is fail\n");
		return -1;
	}
	
	
	
	//目的地址
	struct sockaddr_in dest_addr = {0};
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port); 	
	dest_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	int ret = -1;
	//请求连接目标地址
 
	ret = connect(tcp_socket, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	if(ret != 0)//连接成功
	{
		perror("connect failed\n");
		return -1;
	}
	
	printf("\nConnect succeed!\n");
	
	char file_path[128] = {0};//文件路径
	char file_info[2048] = {0};//文件信息
	
	
	char buf[1024] = {0};
	
	//获取用户输入的文件路径
	printf("Please enter file path: ");
	scanf("%s", file_path);
	
	//从文件路径中获取文件名，如"test/a.txt" ==> "a.txt"
	char file_name[128] = {0};
	printf("current : %s\n", basename(file_path));
	strncpy(file_name, basename(file_path), sizeof(file_name));
	
	//打开文件
	int fd = open(file_path, O_RDWR);
	if (fd == -1)
	{
		printf("open [%s]  failed", file_path);
		return -1;
	}	
	
	//计算文件大小
	struct stat statbuf;
	fstat(fd, &statbuf);
	int len = statbuf.st_size;
	//int len = lseek(fd, 0, SEEK_END);
 
	//文件光标偏移到文件开始位置
	lseek(fd, 0, SEEK_SET);
	
	
	//将文件大小和文件名存放到file_info
	sprintf(file_info, "%d", len);
	strcpy(file_info + 16, file_name);
		
	
	// 将需要上传的文件名告诉对方 
	write(tcp_socket, file_info, 144);	
 
	int send_len = 0;//记录发送了多少字节
	
	while (1)
	{	
		bzero(buf, sizeof(buf));
		//读取数据
		ret = read(fd, buf, sizeof(buf));
		if (ret <= 0)
		{
			printf("send file[%s] succeed!!!!\n", file_name);
			break;
		}
			
		//发送数据
		write(tcp_socket, buf, ret);
		
		send_len += ret;//统计发送了多少字节
		
		//上传文件的百分比 
		printf("uploading %.2f%%\n", (float)send_len/len * 100);
	}
	
	// 关闭文件 
	close(fd);
	//关闭通信socket 
	close(tcp_socket);
	return 0;
}