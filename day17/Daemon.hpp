//守护进程
#pragma once

#include <signal.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const std::string nullfile="/dev/null";
void Daemon(const std::string &cwd="")
{
    //第一步忽略其他异常信号，防止被信号杀死
    signal(SIGCLD,SIG_IGN);
    signal(SIGPIPE,SIG_IGN);
    signal(SIGSTOP,SIG_IGN);

    //第二步将自己变成新的会话，不受其他会话管理
    if(fork()>0)exit(0);
    //让孙子进程来创建新的会话，因为自成组长的进程不能创建新会话
    setsid();//让使用该函数的进程创建新的会话，并属于该会话
    
    //第三步更改当前进程的路径
    //根据需要传入不同的路径就更改到不同路径下
    if(!cwd.empty())
    {
        chdir(cwd.c_str());
    }

    //第四步，将标准输入，标准输出，标志错误，重定向到垃圾桶文件里，新的会话不再与终端关联
    int fd=open(nullfile.c_str(),O_RDWR);
    if(fd>0)//打开成功
    {
       
       dup2(fd,0);
       dup2(fd,1);
       dup2(fd,2);
       close(fd);
    }


}