#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<sys/socket.h>
#include<netinet/in.h>

int main(){

        //"创建监听"
        int listen_fd = socket(AF_INET,SOCK_STREAM,0);
        if(listen_fd<0){
                perror("error listen");
                exit(1);
        }

        //设置地址复用
        int opt=1;
        setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

        //绑定地址和端口
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8888);
        addr.sin_addr.s_addr = INADDR_ANY;

        if(bind(listen_fd , (sockaddr*)&addr , sizeof(addr))<0){
                perror("error bind");
                close(listen_fd);
                exit(1);
        }

        //开始监听
        if(listen(listen_fd,128)<0){
                perror("error listen");
                close(listen_fd);
                exit(1);
        }

        std::cout << "[服务器] 监听 127.0.0.1:8888" << std::endl;

        //等待客户连接
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int conn_fd = accept(listen_fd , (sockaddr*)&client_addr,&client_len);
        if(conn_fd<0){
                perror("error accept");
                close(listen_fd);
                exit(1);
        }
        std::cout << "[服务器] 客户端已连接！" << std::endl;

        //发送消息
        char buf[1024];
	int n;
	while((n=read(conn_fd,buf,sizeof(buf)))>0)
		write(conn_fd,buf,n);
        std::cout << "[服务器] 客户端已断开 "<< std::endl;

        //关闭连接
        close(conn_fd);
        close(listen_fd);
        std::cout << "[服务器] 连接已关闭" << std::endl;
        return 0;
}
