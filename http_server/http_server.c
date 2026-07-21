#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

void build_response(const char* path,char* resp,size_t size){
	char* body;
	if( strcmp(path,"/") == 0 || strcmp(path," ") == 0)
		body = "<html><h1>Home</h1><ahref='/about'>About</a><html>";
	else if(strcmp(path, "/about") == 0)
		body = "<html><h1>About></h1><p>RR HTTP server.</p><html>";
	else
        	body = "<html><h1>404 Not Found</h1></html>";
	snprintf(resp,size,"HTTP/1.1 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "\r\n"
          "%s", body);
}

int main(){
	int fd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(fd, (struct sockaddr*)&addr, sizeof(addr));
      	listen(fd, 10);
      	printf("Server: http://localhost:8080\n");
        while(1){
          int client = accept(fd, NULL, NULL);
          char buf[4096] = {0};
          read(client, buf, 4096);
          printf("Request:\n%s\n", buf);
          char method[16], path[256];
          sscanf(buf, "%s %s", method, path);
          char resp[4096];
          build_response(path, resp, sizeof(resp));
          write(client, resp, strlen(resp));
          close(client);
      }
      close(fd);
      return 0;
}
