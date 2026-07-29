 # Net_lab — 计算机网络实验合集

  计算机网络核心机制的验证实验，从 Wireshark 抓包到 HTTP 服务器、DNS 解析、TCP 状态观察、拥塞控制模拟、阻塞/回射编程、 socket 入门-- epoll 并发模型
```
  Net_lab/
  ├── wireshark_capture/          Wireshark 抓包拆解
  │   ├── 01_ethernet_mac.png     链路层 · 以太网帧（MAC地址）
  │   ├── 02_ip_layer.png         网络层 · IP分组（源/目的IP）
  │   ├── 03_tcp_segment.png      传输层 · TCP段（源/目的端口）
  │   └── 04_http_message.png     应用层 · HTTP请求报文
  ├── http_server/                手写 HTTP 服务器
  │   ├── http_server.c           socket/bind/listen/accept 全链路 + 路径解析
  │   └── http_server             编译产物
  ├── dns_resolver/               DNS 解析工具
  │   ├── dns_resolver.c          getaddrinfo 遍历 addrinfo 链表 + inet_ntop
  │   └── dns_resolver            编译产物
  ├── TCP_handshake/              TCP 三次握手四次挥手验证
  │   └── TCP三握四挥.png          seq/ack 验证
  ├── tcp_congestion/             TCP 拥塞控制实验
  │   ├── tcp_timewait_server.cpp  TIME_WAIT 观察（ss 验证 2MSL）
  │   ├── tcp_congestion.cpp      拥塞控制模拟器（慢启动/拥塞避免/超时/快速重传）
  │   ├── tcp_congestion          编译产物
  │   └── tcp_timewait_server     编译产物
  └── net_programming/            Socket 编程 
      ├── tcp_hello_server.cpp    第一个 TCP 服务器（hello）
      ├── tcp_hello_client.cpp    TCP 客户端
      ├── tcp_echo_server.cpp     TCP 回射服务器（阻塞 IO，单连接）
      ├── tcp_echo_client.cpp     TCP 回射客户端
      ├── select_echo_server.cpp  select 多路复用（一个线程管多个连接）
      ├── epoll_lt_server.cpp     epoll LT 模式
      ├── epoll_et_server.cpp     epoll ET 模式（非阻塞 IO + while EAGAIN）
      ├── fork_echo_server.cpp    多进程并发模型（accept→fork→子echo）
      └── thread_echo_server.cpp  多线程并发模型（accept→thread.detach）

```

 ## 快速开始

  ```bash
  git clone https://github.com/Rrrayy/Net_lab.git
  cd Net_lab

  # HTTP 服务器
  cd http_server && gcc http_server.c -o http_server && ./http_server

  # DNS 解析器
  cd ../dns_resolver && gcc -g dns_resolver.c -o dns_resolver && ./dns_resolver

  # TIME_WAIT 观察
  cd ../tcp_congestion
  g++ -g tcp_timewait_server.cpp -o tcp_timewait_server && ./tcp_timewait_server

  # 拥塞控制模拟器
  g++ -g tcp_congestion.cpp -o tcp_congestion && ./tcp_congestion

  # Socket 主线实验
  cd ../net_programming

  # 1：hello
  g++ -o tcp_hello_server tcp_hello_server.cpp && g++ -o tcp_hello_client tcp_hello_client.cpp

  # 2：回射（阻塞 IO 局限验证）
  g++ -o tcp_echo_server tcp_echo_server.cpp && g++ -o tcp_echo_client tcp_echo_client.cpp
  # 开两个客户端 → 第二个 read 卡住，引出 select

  # 3：select 多路复用
  g++ -o select_echo_server select_echo_server.cpp && ./select_echo_server
  # 多客户端同时连接，全部可回射

  # 4：epoll LT + ET
  g++ -std=c++17 -o epoll_lt_server epoll_lt_server.cpp && g++ -std=c++17 -o epoll_et_server epoll_et_server.cpp
  # ET 验证：BUF_SIZE=4 输入hello,不 while 读会丢 "o"

  #  5：多进程 + 多线程并发模型
  g++ -std=c++17 -o fork_echo_server fork_echo_server.cpp && ./fork_echo_server
  g++ -std=c++17 -o thread_echo_server thread_echo_server.cpp -lpthread && ./thread_echo_server
  # ps aux 看父子进程 / 多终端同时 echo

  ```

  ## 环境

  - 系统：Ubuntu（Wireshark / curl / ss / gdb / strace）
  - 编译器：g++，标准 C++17
  - C 程序：gcc（http_server / dns_resolver）
