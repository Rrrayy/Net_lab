#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
	int sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd<0){
		perror("error socket");
		exit(1);
	}
	sockaddr_in server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8888);
	inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);
	if(connect(sock_fd,(sockaddr*)&server_addr,sizeof(server_addr))<0){
		perror("error connect");
		close(sock_fd);
		exit(1);
	}
	std::cout << "[客户端] 已连接到服务器！" << std::endl;
	char buf[1024]={};
	int n = read(sock_fd,buf,sizeof(buf)-1);
	if(n>0){
		buf[n]='\0';
		std::cout << "[客户端] 收到服务器: " << buf << std::endl;
	}
	close(sock_fd);
	std::cout << "[客户端] 连接已关闭" << std::endl;
	return 0;
}
