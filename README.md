 # Net_lab — 计算机网络实验合集

  计算机网络核心机制的验证实验，从 Wireshark 抓包到 HTTP 服务器、DNS 解析、TCP 状态观察、拥塞控制模拟、阻塞/回射编程......
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
  └── net_programming/            Socket 网络编程
      ├── tcp_hello_server.cpp    第一个TCP程序（hello）
      ├── tcp_hello_client.cpp    TCP 客户端
      ├── tcp_echo_server.cpp     TCP 回射服务器（read→write 循环）
      ├── tcp_echo_client.cpp     键盘输入回射客户端
      ├── tcp_hello_server       编译产物
      └── tcp_hello_client       编译产物

```

## 快速开始

  ```bash
  git clone https://github.com/Rrrayy/Net_lab.git
  cd Net_lab

  # HTTP 服务器
  cd http_server
  gcc http_server.c -o http_server
  ./http_server
  # 浏览器访问 http://localhost:8080

  # DNS 解析工具
  cd dns_resolver
  gcc -g dns_resolver.c -o dns_resolver
  ./dns_resolver

  # TIME_WAIT 观察
  cd tcp_congestion
  g++ -g tcp_timewait_server.cpp -o tcp_timewait_server
  ./tcp_timewait_server
  # 另开终端：curl http://127.0.0.1:8888 && ss -tan | grep 8888

  # 拥塞控制模拟器
  g++ -g tcp_congestion.cpp -o tcp_congestion
  ./tcp_congestion

  # TCP hello
  cd net_programming
  g++ -o tcp_hello_server tcp_hello_server.cpp
  g++ -o tcp_hello_client tcp_hello_client.cpp
  # 终端1：./tcp_hello_server
  # 终端2：./tcp_hello_client

  # TCP 回射服务器
  g++ -o tcp_echo_server tcp_echo_server.cpp
  g++ -o tcp_echo_client tcp_echo_client.cpp
  # 终端1：./tcp_echo_server
  # 终端2：./tcp_echo_client
  # 终端3：./tcp_echo_client（第二个连上了但没被处理→阻塞IO问题）
```

  ## 环境

  - 系统：Ubuntu（实验运行）
  - 编译器：g++，标准 C++17
  - 工具：Wireshark / curl / ss / gdb / strace
