 # Net_lab — 计算机网络实验合集

  计算机网络核心机制的验证实验。
```

Net_lab/
  ├── wireshark_capture/       # Wireshark 抓包拆解
  │   ├── 01_ethernet_mac.png  # 链路层 · 帧
  │   ├── 02_ip_layer.png      # 网络层 · 包
  │   ├── 03_tcp_segment.png   # 传输层 · 段
  │   └── 04_http_message.png  # 应用层 · 报文
  ├── http_server/             # 手写 HTTP 服务器
      ├── http_server.c        # socket 实现，路径解析（/、/about、404）
      └── http_server          # 编译产物
```


  ## 快速开始

  ```bash
  git clone https://github.com/Rrrayy/Net_lab.git
  cd Net_lab
  # 直接查看 wireshark_capture/ 下各层截图

 # HTTP 服务器
  cd http_server
  gcc http_server.c -o http_server
  ./http_server
  # 浏览器访问 http://localhost:8080

  环境

  - 系统：Ubuntu（实验运行）
  - 编译器：g++，标准 C++17
  - 工具：Wireshark / curl / gdb / strace
