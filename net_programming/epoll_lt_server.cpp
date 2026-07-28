  #include<iostream>
  #include<unistd.h>
  #include<cstdlib>
  #include<cstring>
  #include<sys/socket.h>
  #include<netinet/in.h>  
  #include<arpa/inet.h>
  #include<sys/epoll.h>

  int main(){
      int listen_fd = socket(AF_INET,SOCK_STREAM,0);
      if(listen_fd<0){perror("error socket"); exit(1);}
      int opt=1;
      setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
      sockaddr_in addr{};
      addr.sin_family = AF_INET;
      addr.sin_port = htons(8888);
      addr.sin_addr.s_addr = INADDR_ANY;
      if(bind(listen_fd,(sockaddr*)&addr,sizeof(addr))<0){
          perror("error bind"); close(listen_fd); exit(1);
      }
      if(listen(listen_fd,128)<0){
          perror("error listen"); close(listen_fd); exit(1);
      }
      std::cout << "[LT服务器] 启动 0.0.0.0:8888" << std::endl;

      int epfd = epoll_create(1);
      if(epfd<0){perror("error epoll_create"); exit(1);}

      struct epoll_event ev;
      ev.events = EPOLLIN;
      ev.data.fd = listen_fd;
      epoll_ctl(epfd,EPOLL_CTL_ADD,listen_fd,&ev);

      struct epoll_event events[1024];
      char buf[1024];

      while(1){
          int nfds = epoll_wait(epfd,events,1024,-1);
          if(nfds<0){perror("error epoll_wait"); break;}

          for(int i=0; i<nfds; i++){
              if(events[i].data.fd == listen_fd){
                  sockaddr_in client_addr{};
                  socklen_t client_len = sizeof(client_addr);
                  int conn_fd = accept(listen_fd,(sockaddr*)&client_addr,&client_len);
                  if(conn_fd<0){perror("error accept"); continue;}
                  char ip[INET_ADDRSTRLEN];
                  inet_ntop(AF_INET,&client_addr.sin_addr,ip,sizeof(ip));
                  std::cout << "[接入] " << ip << ":" << ntohs(client_addr.sin_port) << " fd=" << conn_fd << std::endl;
                  ev.events = EPOLLIN;
                  ev.data.fd = conn_fd;
                  epoll_ctl(epfd,EPOLL_CTL_ADD,conn_fd,&ev);

              }else{
                  int fd = events[i].data.fd;
                  int n = read(fd,buf,sizeof(buf));
                  if(n>0){
                      write(fd,buf,n);
                      buf[n]='\0';
                  std::cout << "[回射" << fd << "] " << buf<<std::endl;
                  }else if(n==0){
                      std::cout << "[断开] fd=" << fd << std::endl;
                      epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                      close(fd);
                  }else{
                      perror("read");
                      epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                      close(fd);
                  }
              }
          }
      } 

      close(epfd);
      close(listen_fd);
      return 0;
  }
