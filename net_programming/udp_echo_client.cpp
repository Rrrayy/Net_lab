#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("error socket");
        exit(1);
    }
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buf[1024];
    sockaddr_in from_addr{};
    socklen_t from_len = sizeof(from_addr);

    while(1){
        bzero(buf, sizeof(buf));
        std::cout << "请输入: ";
        std::cin.getline(buf, sizeof(buf));

        sendto(sockfd, buf, strlen(buf), 0,(sockaddr*)&server_addr, sizeof(server_addr));

        int n = recvfrom(sockfd, buf, sizeof(buf)-1, 0,(sockaddr*)&from_addr, &from_len);
        if(n > 0){
            buf[n] = '\0';
            std::cout << "回射: " << buf << std::endl;
        }
    }
    close(sockfd);
    return 0;
}
