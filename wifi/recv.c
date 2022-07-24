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
 
 
int main(int argc, const char *argv[])
{
	//判断命令行参数是否匹配
	if(argc != 2) {
		fprintf(stderr, "Usage : %s port\n", argv[0]);
		return -1;
	}
	
	//1，创建tcp通信socket	买电话
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (tcp_socket == -1) {
		perror("create socketfd failed\n");
		return -1;
	}
	
	
	//2，绑定地址（搞电话卡，激活：ip地址，端口）
	struct sockaddr_in local_addr = {0};
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(atoi(argv[1])); 
	local_addr.sin_addr.s_addr = INADDR_ANY;//让系统自动检测本地网卡IP并绑定
	
	int ret = bind(tcp_socket, (struct sockaddr *)&local_addr, sizeof(local_addr));
	
	if (ret == -1) {
		perror("bind  failed\n");
		return -1;
	}
	
	
	//3，设置监听队列
	ret = listen(tcp_socket, 5);
	
	if(ret == -1) {
		perror("listen is fail\n");
		return -1;
	}
	
	
	//客户端地址信息
	struct sockaddr_in client_addr={0};
	int len = sizeof(client_addr);
		 
	printf("server is running\n");
	
	//通信socket
	int new_socket = 0;
	
	while (1) {
		//等待客户端的请求
		new_socket = accept(tcp_socket,(struct sockaddr *)&client_addr,&len);
		
		if(new_socket < 0) {
			perror("accept error\n");
			continue;
		}
		
		//输出对方的IP地址信息
		printf("client connected [%s:%d]\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
				
		char file_len[16] = {0};//文件长度
		char file_name[128] = {0};//文件名称
		char buf[1024] = {0};//数据缓冲区
		//读取文件大小和文件名称
		read(new_socket, buf, sizeof(buf));
		
		strncpy(file_len, buf, sizeof(file_len));//取出文件大小
		strncpy(file_name, buf+sizeof(file_len), sizeof(file_name));//取出文件名称
		
		printf("ready receive!!!! file name:[%s] file size:[%s] \n",file_name, file_len);
		
		//新的文件名
		sprintf(buf, "%s", file_name);
		
		//创建新文件
		int fd = open(buf, O_RDWR | O_CREAT | O_TRUNC, 0666);
		
		int size = atoi(file_len);//文件大小
		int write_len = 0;//记录已写入的字节数
		
		//接收文件
		while(1) {
			bzero(buf, 1024);
			
			//接收文件数据
			ret = read(new_socket, buf, sizeof(buf));
			if( ret <= 0) {
				printf("\n [%s] receive file done!!!\n", file_name);
				break;
			}
			
			//将数据写入文件	
			write(fd, buf, ret);
			
			write_len += ret;//记录写入的字节数
 
			//动态的输出接收进度
			printf("uploading %.2f%% \n", (float)write_len/size * 100);
			
		}
		break;
	}
	
	//关闭socket
	close(new_socket);
	close(tcp_socket);
	
	return 0;
}