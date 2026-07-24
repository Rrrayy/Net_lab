#include <iostream>
#include <string>

  int main(){
      double cwnd = 1;       // 当前拥塞窗口
      double ssthresh = 16;  // 慢启动阈值
      int rtt = 0;           // 当前 RTT 计数

      std::cout << "=== TCP 拥塞控制模拟 ===" << std::endl;
      std::cout << "初始: cwnd=" << cwnd << ", ssthresh=" << ssthresh << std::endl;

      // 模拟 20 个 RTT
      for(rtt = 1; rtt <= 20; rtt++){
          // 判断当前阶段
          std::string phase;
          if(cwnd < ssthresh)      phase = "慢启动";
          else if(cwnd >= ssthresh) phase = "拥塞避免";

          // 记录变化前的 cwnd
          double old_cwnd = cwnd;

          // 模拟 RTT 10 的超时
          if(rtt == 10){
              ssthresh = cwnd / 2;
              if(ssthresh < 2) ssthresh = 2;
              cwnd = 1;
              std::cout << "[RTT " << rtt << "] *** 超时! ***" << std::endl;
              std::cout << "    ssthresh = " << old_cwnd << "/2 = " << ssthresh << ", cwnd = 1 (重新慢启动)" << std::endl;
              continue;
          }

          // 模拟 RTT 15 的快速重传（3个重复ACK）
          if(rtt == 15){
              ssthresh = cwnd / 2;
              if(ssthresh < 2) ssthresh = 2;
              cwnd = ssthresh + 3;
              std::cout << "[RTT " << rtt << "] *** 3个重复ACK！ (快速重传+快速恢复) ***" << std::endl;
              std::cout << "    ssthresh = " << old_cwnd << "/2 = " << ssthresh  << ", cwnd = ssthresh+3 = " << cwnd << std::endl;
              continue;
          }
	
	    if(cwnd < ssthresh){
              cwnd *= 2;  // 慢启动：指数增长
          } else {
              cwnd += 1;  // 拥塞避免：线性增长
          }

          std::cout << "[RTT " << rtt << "] " << phase
                    << ": " << old_cwnd << " → " << cwnd
                    << " (ssthresh=" << ssthresh << ")" << std::endl;
      }

      return 0;
  }
