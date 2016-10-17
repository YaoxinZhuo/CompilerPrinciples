//
//  main.cpp
//  SimpleLexicalanalyzer
//
//  Created by YaoxinZhuo on 10/17/16.
//  Copyright © 2016 YaoxinZhuo. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
#include <map>//引入map，快速查找符号和关键字，也可以对应他们的序号，方便打印

using namespace std;

map<string,int> Symbols;//string符号，int种别码
map<string,int> Keywords;//string关键词，int种别码

bool isDigit(char ch)
{
    if(ch <= '9' && ch >= '0')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isLetter(char ch)
{
    if( (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') )
    {
        return true;
    }
    else
    {
        return false;
    }
}

int isSymbol(string str)
{
    if(Symbols.count(str) != 0)
    {
        return Symbols[str];
    }
    else
    {
        return -1;
    }
}

int isKeyword(string str)
{
    if(Keywords.count(str) != 0)
    {
        return Keywords[str];
    }
    else
    {
        return -1;
    }
}

string handleSpace(string s)
{
    string result;
    
    int j = 0;
    
    for(int i = 0;i < s.length();i ++)
    {
        if(s[i]!=' ')
        {
            result += s[i];
            j++;
        }
    }
    
    return result;
}

void preProcess()
{
    //pair插入效率高于直接 Symbols["*"] = 1; 这样
    Symbols.insert(pair<string, int>("+",13));
    Symbols.insert(pair<string, int>("-",14));
    Symbols.insert(pair<string, int>("*",15));
    Symbols.insert(pair<string, int>("/",16));
    Symbols.insert(pair<string, int>(":",17));
    Symbols.insert(pair<string, int>(":=",18));
    Symbols.insert(pair<string, int>("<",20));
    Symbols.insert(pair<string, int>("<>",21));
    Symbols.insert(pair<string, int>("<=",22));
    Symbols.insert(pair<string, int>(">",23));
    Symbols.insert(pair<string, int>(">=",24));
    Symbols.insert(pair<string, int>("=",25));
    Symbols.insert(pair<string, int>(";",26));
    Symbols.insert(pair<string, int>("(",27));
    Symbols.insert(pair<string, int>(")",28));
    Symbols.insert(pair<string, int>("#",0));
    
    Keywords.insert(pair<string,int>("begin",1));
    Keywords.insert(pair<string,int>("if",2));
    Keywords.insert(pair<string,int>("then",3));
    Keywords.insert(pair<string,int>("while",4));
    Keywords.insert(pair<string,int>("do",5));
    Keywords.insert(pair<string,int>("end",6));

}

int main(int argc, const char * argv[]) {
    preProcess();//预处理，加载symbols,keywords等
    
    string inputString;
    string token;
    
    getline(cin,inputString);

    //inputString = handleSpace(inputString); 可以把空格运用到分割中，真的编译器不是这么简单的
    //cout<<inputString<<endl;示范性编译输出
    
    for(int i = 0;i < inputString.length();i ++)
    {
        if(inputString[i] == ' ')
        {
            token = "";//clear
            continue;
        }
        
        if(isLetter(inputString[i]))//标识别符，看看是不是keywords或者ID
        {
            while (isLetter(inputString[i]) || isDigit(inputString[i]) )
            {
                token += inputString[i];
                i++;
            }
            int keywordNumber = isKeyword(token);
            if(keywordNumber == -1)
            {
                keywordNumber = 10;
                cout<<"("<<keywordNumber<<","<<token<<")";
            }
            else
            {
                cout<<"("<<keywordNumber<<","<<token<<")";
            }
            token = "";//clear
        }
        if(isDigit(inputString[i]))//判断是不是数字
        {
            while (isDigit(inputString[i]))
            {
                token += inputString[i];
                i++;
            }
            cout<<"("<<11<<","<<token<<")";
            token = "";//clear
        }
        //余下字符
        if(!isDigit(inputString[i]) && !isLetter(inputString[i]) && inputString[i] != ' ')
        {
            token += inputString[i];
            i++;
            if(inputString[i] == '=' || inputString[i] == '>')//如果下一个字符符合要求，加入
            {
                token += inputString[i];
            }
            else//否则i的下一个得放到下一轮判定,i--还原
            {
                i--;
            }
            int otherwordsNumber = Symbols[token];
            cout<<"("<<otherwordsNumber<<","<<token<<")";
            token = "";//clear
        }
    }
    
    
    cout<<endl;
    
    
    return 0;
}
