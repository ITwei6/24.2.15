#pragma once
#include "Log.hpp"
#include <iostream>
#include "Init.hpp"
Init init;//刚加载程序时，文件里的内容就加载到map里了。
extern Log lg;
class TASK//构建任务，就是一旦获取到连接后，就将客户端的网络信息存到任务里，让服务器根据这个信息去服务客户端
{
    
public:    
    TASK(int &sockfd,const std::string& ip,uint16_t& port):_sockfd(sockfd),_clientip(ip),_clientport(port)
    {}
    void run()
    {
         char inbuffer[1024];
        // while(true)
         
          ssize_t n=read(_sockfd,inbuffer,sizeof(inbuffer));
          if(n>0)
          {
            inbuffer[n]=0;
            std::cout<<"client key： "<<inbuffer<<std::endl;
            //加工处理一下
            std::string echo_string=init.translation(inbuffer);

            //将加工处理的数据发送会去
            n=write(_sockfd,echo_string.c_str(),echo_string.size());//写入过程也可能会失败，操作系统会发送信号将该进程杀死的，我们不想被杀死，就要忽略这个信息
            if(n<0)
            {
              //失败了，我们将信号忽略，但将日志打印出来
              lg(Fatal,"write error :%d ,errstring :%s",errno,strerror(errno));
            }
          }
          else if(n==0)//如果没有用户连接了，那么就会读到0.服务器端也就不要再读了
          {
            lg(Info,"%s:%d quit, server close sockfd: %d",_clientip.c_str(),_clientport,_sockfd);
            
          }
          else
          {
            lg(Fatal,"read errno: %d, errstring: %s",errno,strerror(errno));
          }
          close(_sockfd);//任务只处理一次，服务器端处理完任务后，就会将该套接字关闭，线程池里的线程就不会一直在执行
         //客户端如果想再使用服务，需要重新连接，而该套接字已经被关闭，客户端也需要重新创建
    }
    void operator()()
    {
        run();
    }
    ~TASK()
    {
    }
public:
   int _sockfd;
   std::string _clientip;
   uint16_t _clientport;
};