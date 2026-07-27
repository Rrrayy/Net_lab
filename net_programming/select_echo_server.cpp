#include<iostream>
#include<cstring>
#include<unistd.h>
#include<cstdlib>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/select.h>

int main(){
    int listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd<0){
        perror("socket error");
        exit(1);
    }
    int opt=1;
    setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(listen_fd, (sockaddr*)&addr, sizeof(addr))<0){
        perror("bind error");
        close(listen_fd);
        exit(1);
    }

    std::cout << "[服务器] 监听 0.0.0.0:8888" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    if(listen(listen_fd, 128) < 0){
        perror("listen error"); close(listen_fd); exit(1);
    }
    fd_set all_fds;
    fd_set read_fds;
    FD_ZERO(&all_fds);
    FD_SET(listen_fd,&all_fds);
    int max_fd = listen_fd;
    char buf[1024];
   
    while(1){
        read_fds = all_fds;
        int nready  = select(max_fd+1,&read_fds,NULL,NULL,NULL);
        if(nready<0){
            perror("select error");
            exit(1);  
        }
        if(nready == 0) continue;
        for(int fd=0; fd<=max_fd;fd++){
            if(!FD_ISSET(fd,&read_fds)) continue;
            if(fd == listen_fd){
                 sockaddr_in client_addr{};
                 socklen_t client_len = sizeof(client_addr);
                 int conn_fd = accept(listen_fd, (sockaddr*)&client_addr, &client_len);
                 if(conn_fd < 0){ perror("accept"); continue; }

                 char ip[INET_ADDRSTRLEN];
                 inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
                 std::cout << "[新连接] " << ip << ":" << ntohs(client_addr.sin_port) << " → fd=" << conn_fd << std::endl;
                 FD_SET(conn_fd, &all_fds);  // 新fd加入监控
                 if(conn_fd > max_fd) max_fd = conn_fd;
            }
            else{
                int n = read(fd, buf, sizeof(buf));
                if(n <= 0){  // 断开或出错
                    if(n == 0)
                        std::cout << "[断开] fd=" << fd << std::endl;
                    else
                        perror("read");
                    close(fd);
                    FD_CLR(fd, &all_fds);  // 从监控移除
            }
            else{       // 读到数据 → 回射
                write(fd, buf, n);
                buf[n] = '\0';
                std::cout << "[来自" << fd << "] " << buf;
            }
        }
    }
  }   
    close(listen_fd);
    return 0; 
}
