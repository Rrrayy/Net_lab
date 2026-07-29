#include<iostream>
#include<unistd.h>
#include<cstring>
#include<sys/socket.h>
#include<signal.h>
#include<netinet/in.h>

void echo(int conn_fd){
    char buf[1024];
    int n;
    while(1){
        n= read(conn_fd,buf,sizeof(buf)-1);
        if(n>0){
            buf[n]='\0';
            std::cout<<"get "<<buf<<std::endl;
            write(conn_fd,buf,n);
        }
        else if(n==0){
            std::cout<<"客户端断开"<<std::endl;
            break;
        }
        else{
            std::cout<<"error read"<<strerror(errno)<<std::endl;
            break;
        }
    }
    close(conn_fd);
}

int main(){
    int listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd<0){
        std::cerr<<"error socket"<<std::endl;
        return 1;
    }
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listen_fd,(struct sockaddr*)&addr,sizeof(addr))<0){
        std::cerr<<"error bind"<<std::endl;
        close(listen_fd);
        return 1;
    }
    if(listen(listen_fd,128)<0){
        std::cerr<<"error listen"<<std::endl;
        close(listen_fd);
        return 1;
    }
    signal(SIGCHLD,SIG_IGN);
    while(true){
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr); 
        int conn_fd = accept(listen_fd,(struct sockaddr*)&client_addr,&client_len);
        if(conn_fd<0){
            std::cerr<<"error accept"<<std::endl;
            continue;
        }
        std::cout<<"新客户端接入"<<std::endl;
        pid_t pid = fork();
        if(pid<0){
            std::cerr<<"error fork"<<std::endl;
            close(conn_fd);
            continue;
        }
        if(pid==0){
            close(listen_fd);
            echo(conn_fd);
            exit(0);
        }
        close(conn_fd);         
    }
    close(listen_fd);
    return 0;
}
