#include<iostream>
#include<cstring>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#define BUF_SIZE 4

    void set_nonblocking(int fd){
        int old_flag = fcntl(fd,F_GETFL);
        int new_flag = old_flag | O_NONBLOCK;
        fcntl(fd,F_SETFL,new_flag);
    }

    int main(){
        int listen_fd = socket(AF_INET,SOCK_STREAM,0);
        if(listen_fd<0){
            perror("error socket"); 
            exit(1);
        }
        int opt=1;
        setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8889);    
        addr.sin_addr.s_addr = INADDR_ANY;

        if(bind(listen_fd,(sockaddr*)&addr,sizeof(addr))<0){
                perror("bind"); 
                close(listen_fd); 
                exit(1);
        }
        if(listen(listen_fd,128)<0){
                perror("error listen"); 
                close(listen_fd); 
                exit(1);
        }
        std::cout << "[ET服务器] 启动 0.0.0.0:8889" << std::endl;
        std::cout << "  BUF_SIZE=" << BUF_SIZE << "，用来验证while循环读过程" << std::endl;
        set_nonblocking(listen_fd);

        // 创建 epoll 实例
        int epfd = epoll_create(1);
        if(epfd<0){
            perror("error epoll_create"); 
            exit(1);
        }

        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLET;   
        ev.data.fd = listen_fd;
        epoll_ctl(epfd,EPOLL_CTL_ADD,listen_fd,&ev);
        std::cout << "  listen_fd=" << listen_fd << " 注册为 ET" << std::endl;

        struct epoll_event events[1024];
        char buf[BUF_SIZE];

        while(1){
                int nfds = epoll_wait(epfd,events,1024,-1);
                if(nfds<0){
                    perror("error epoll_wait");
                    break;
                }
                for(int i=0; i<nfds; i++){
                        if(events[i].data.fd == listen_fd){
                                while(1){
                                        sockaddr_in client_addr{};
                                        socklen_t client_len = sizeof(client_addr);
                                        int conn_fd = accept(listen_fd, (sockaddr*)&client_addr,&client_len);
                                        if(conn_fd<0){
                                                // accept 返回 -1 + EAGAIN = 所有连接都接完了
                                                if(errno==EAGAIN || errno==EWOULDBLOCK)
                                                        break;
                                                perror("error accept"); 
                                                break;
                                        }

                                        char ip[INET_ADDRSTRLEN];
                                        inet_ntop(AF_INET,&client_addr.sin_addr,ip,sizeof(ip));
                                        std::cout << "[接入] " << ip << " fd=" << conn_fd << std::endl;
                                       // set_nonblocking(conn_fd);

                                        //conn_fd 也注册为 ET
                                        ev.events = EPOLLIN | EPOLLET;
                                        ev.data.fd = conn_fd;
                                        epoll_ctl(epfd,EPOLL_CTL_ADD,conn_fd,&ev);
                                }

                        } else {
                                int fd = events[i].data.fd;

                                // ET 必须 while 循环读到 EAGAIN
                                while(1){
                                        memset(buf,0,BUF_SIZE);
                                        int n = read(fd,buf,BUF_SIZE);

                                        if(n>0){
                                                write(fd,buf,n);
                                                std::cout << "[回射" << fd << "][" << n << "B] "<< buf << std::endl;
                                        }
                                        else if(n==-1){
                                                // 非阻塞IO：缓冲区空了 → EAGAIN → break
                                                if(errno==EAGAIN || errno==EWOULDBLOCK){
                                                        break;  
                                                }
                                                perror("error read");
                                                epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                                                close(fd);
                                                break;
                                        }
                                        else{
                                                // n==0 → 对端关闭
                                                std::cout << "[断开] fd=" << fd << std::endl;
                                                epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                                                close(fd);
                                                break;
                                        }
                                }
                        }
                }
            }

        close(epfd);
        close(listen_fd);
        return 0;

}
