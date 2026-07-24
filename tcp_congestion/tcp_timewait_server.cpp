#include<iostream>
#include<cstring>
#include<cstdlib>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
	int listen_fd=socket(AF_INET,SOCK_STREAM,0);
	if(listen_fd<0){
		perror("socket 创建失败");
		exit(1);
	}
	
	int opt=1;
	setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(listen_fd,(sockaddr*)&addr,sizeof(addr))<0){
		perror("error bind");
		close(listen_fd);
		exit(1);
	}
	
	listen(listen_fd,5);
	std::cout << "[服务器] ================================" << std::endl;
        std::cout << "[服务器] 启动成功！" << std::endl;
        std::cout << "[服务器] PID = " << getpid() << std::endl;
        std::cout << "[服务器] 监听端口: 8888" << std::endl;
        std::cout << "[服务器] " << std::endl;
        std::cout << "[服务器] 另一个终端运行观察命令ss -tan | grep 8888：" << std::endl;
        std::cout << "[服务器] " << std::endl;
        std::cout << "[服务器] 用 curl 发请求：" << std::endl;
        std::cout << "[服务器]   curl http://127.0.0.1:8888" << std::endl;
        std::cout << "[服务器] ================================" << std::endl;
	
	while(true){
		sockaddr_in client_addr{};
		socklen_t client_len=sizeof(client_addr);
		int conn_fd=accept(listen_fd,(sockaddr*)&client_addr,&client_len);
		if(conn_fd<0){
			perror("fail accept");
			continue;
		}
		char client_ip[64];
		inet_ntop(AF_INET,&client_addr.sin_addr,client_ip,sizeof(client_ip));
		std::cout << "[服务器] <<<< 收到新连接!" << std::endl;
                std::cout << "[服务器]     客户端 IP: " << client_ip << std::endl;
                std::cout << "[服务器]     客户端端口: " << ntohs(client_addr.sin_port) << std::endl;
		char buf[1024];
		int n = read(conn_fd,buf,sizeof(buf)-1);
		if(n>0){
			buf[n]='\0';
			std::cout << "[服务器]     收到数据: " << buf << std::endl;
			write(conn_fd,buf,n);
			std::cout << "[服务器]     已回射 " << n << " 字节" << std::endl;
		}
		else if(n==0)
			std::cout << "[服务器]     客户端主动关闭连接" << std::endl;
		else
			perror("error read");
		close(conn_fd);
		std::cout << "[服务器] >>>> 连接已关闭" << std::endl;
	}
	close(listen_fd);
	return 0;
 }

