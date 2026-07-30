#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>

int main(){
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        perror("error socket");
        exit(1);
    }
    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(sockfd,(sockaddr*)&addr,sizeof(addr))<0){
        perror("error bind");
        close(sockfd);
        exit(1);
    }
    std::cout << "[UDP服务器] 监听 0.0.0.0:8888" << std::endl;
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    char buf[1024];
    while(1){
        bzero(buf,sizeof(buf));
        int n=recvfrom(sockfd,buf,sizeof(buf)-1,0,(sockaddr*)&client_addr,&client_len);
        if(n<0){
            perror("error recvfrom");
            continue;
        }
        buf[n] = '\0'; 
        std::cout << "[收到] " << inet_ntoa(client_addr.sin_addr)<< ":" << ntohs(client_addr.sin_port)
        << " → " << buf << std::endl;

        sendto(sockfd, buf, n, 0,(sockaddr*)&client_addr, client_len);
    } 
    close(sockfd);
    return 0;
}
