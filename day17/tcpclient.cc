#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


void Usage(std::string proc)
{
    std::cout<<"\n\rUsage: "<<proc<<" port[1024+]\n"<<std::endl;
}
//./tcpclient ip port
int main(int args,char* argv[])
{
    if(args!=3)
    {
     Usage(argv[0]);
     exit(1);
    }
    std::string serverip=argv[1];
    uint16_t serverport = std::stoi(argv[2]);
    struct sockaddr_in server;
    socklen_t len=sizeof(server);
    server.sin_family=AF_INET;
    server.sin_port=htons(serverport);
    inet_pton(AF_INET,serverip.c_str(),&server.sin_addr);

    while(true)
    {
    //创建套接字
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        std::cout<<"create sockfd err "<<std::endl;
    }
    //创建套接字成功，创建完套接字后该干什么?
    //连接服务器端的套接字，所以客户端用户需要知道服务器端的网络信息的
    int cnt=10;
    bool isreconnect=false;
    do
    { 
       int n=connect(sockfd,(struct sockaddr*)&server,len);
      if(n<0)//服务器关闭了，肯定会连接失败
      {
        isreconnect=true;
        cnt--;
        std::cout<<"connect sock err...,cnt: "<<cnt<<std::endl;
        sleep(12);
      }
      else//重连成功了
      {
        break;
      }
    }while(cnt&&isreconnect);
    //连接成功
    //连接成功后，就可以直接通信了，就可以直接给对方写消息了。
    if(cnt==0)
    {
        std::cerr<<"user offline.."<<std::endl;
        break;//用户直接不玩了
    }
    std::string message;
   
    std::cout<<"Please enter#";
    getline(std::cin,message);
    //往套接字里写
    int n=write(sockfd,message.c_str(),message.size());
    if(n<0)//服务器端会将该套接字关闭，然后就写不进去了。需要重新创建套接字连接
    {
        std::cerr<<"write error..."<<std::endl;
        continue;
    }
    char outbuffer[1024];
    //接收服务器发送的加工处理消息

    n=read(sockfd,outbuffer,sizeof(outbuffer));
    if(n>0)
    {
       outbuffer[n]=0;
       std::cout<<outbuffer<<std::endl;
    }
    close(sockfd);
    }
    return 0;
 
}