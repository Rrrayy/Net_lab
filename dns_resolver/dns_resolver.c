#include<stdio.h>
  #include<string.h>
  #include<netdb.h>
  #include<sys/socket.h>
  #include<arpa/inet.h>
  #include<time.h>

  int main(int argc, char* argv[]){
        if(argc<2){
                printf("用法: %s <域名1> [域名2] ...\n", argv[0]);
                printf("示例: %s www.baidu.com www.taobao.com\n", argv[0]);
                return 1;
        }

        printf("\n═══ DNS 解析工具 ═══\n");
        printf("参数个数: %d 个域名\n\n", argc - 1);

        for(int i=1;i<argc;++i){
                char *domain = argv[i];
                printf("━━━ [%d/%d] 正在解析: %s ━━━\n", i, argc - 1, domain);

                struct addrinfo hints;
                memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_STREAM;

                struct timespec ts_start, ts_end;
                clock_gettime(CLOCK_MONOTONIC, &ts_start);

                struct addrinfo *res;
                int status = getaddrinfo(domain, "80", &hints, &res);

                clock_gettime(CLOCK_MONOTONIC, &ts_end);
                double elapsed = (ts_end.tv_sec - ts_start.tv_sec) * 1000.0
                               + (ts_end.tv_nsec - ts_start.tv_nsec) / 1000000.0;

                if(status != 0){
                        printf(" 解析失败 [%s]: %s\n", domain, gai_strerror(status));
                        continue;
                }

                int cnt_v4 = 0, cnt_v6 = 0;
                struct addrinfo *cur = res;
                while(cur != NULL){
                        if(cur->ai_family == AF_INET){
                                struct sockaddr_in *addr4 = (struct sockaddr_in *)cur->ai_addr;
                                char ipv4_str[INET_ADDRSTRLEN];
                                inet_ntop(AF_INET, &addr4->sin_addr, ipv4_str, sizeof(ipv4_str));
                                unsigned short port = ntohs(addr4->sin_port);
                                printf("   IPv4 #%d: %s :%d\n", ++cnt_v4, ipv4_str, port);
                        }
                        else if(cur->ai_family == AF_INET6){
                                struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)cur->ai_addr;
                                char ipv6_str[INET6_ADDRSTRLEN];
                                inet_ntop(AF_INET6, &addr6->sin6_addr, ipv6_str, sizeof(ipv6_str));
                                unsigned short port = ntohs(addr6->sin6_port);
                                printf("   IPv6 #%d: [%s] :%d\n", ++cnt_v6, ipv6_str, port);
                        }
                        cur = cur->ai_next;
                }

                int total = cnt_v4 + cnt_v6;
                printf("  %s: %.2f ms | IPv4=%d | IPv6=%d | 合计=%d\n\n",
                       domain, elapsed, cnt_v4, cnt_v6, total);

                freeaddrinfo(res);
        }

        printf("━━━ 解析完毕 ━━━\n");
        return 0;
  }
