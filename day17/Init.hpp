#pragma once 
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "Log.hpp"
const std::string dictname="./dict.txt";
const std::string sep=":";//默认的分割符
extern Log lg;
class Init//相当于加载配置文件的动作，当创建出来，文件内容就已经被加载到map里了
{

public:
    void Spilt(std::string& line,std::string *part1,std::string* part2)
    {
        auto pos=line.find(sep);
        if(pos==std::string::npos)return ;
        *part1=line.substr(0,pos);
        *part2=line.substr(pos+1);
        return ;
    }
    Init()
    {
        std::ifstream in(dictname);//定义一个文件流，成功就打开该文件
        if(!in.is_open())
        {
           lg(Fatal,"ifstream open %s error",strerror(errno));
           exit(1);
        } 
        //读取文件里的内容
        std::string line;
        //按行读取。读取到line里
        while(std::getline(in,line))
        {
          std::string part1,part2;
          Spilt(line,&part1,&part2);
          //然后分割到map里
          dic.insert({part1,part2});
        }
        
        in.close();
    }
    std::string translation(const std::string& key)
    {
        auto iter=dic.find(key);//返回对应的该值的的迭代器
        if(iter==dic.end())return "Unkonwn";
        else return iter->second;
    }
private:
    std::unordered_map<std::string,std::string> dic; 
};