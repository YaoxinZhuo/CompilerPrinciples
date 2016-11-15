//
//  main.cpp
//  SyntaxAnalyzer
//
//  Created by YaoxinZhuo on 16/11/2016.
//  Copyright © 2016 YaoxinZhuo. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <set>
#include <map>//引入map，快速查找符号和关键字，也可以对应他们的序号，方便打印

using namespace std;

map<string,int> Symbols;//string符号，int种别码
map<string,int> Keywords;//string关键词，int种别码
set<char> LegalSymbols;

string inputString;
string token;
char ch;
int p;
int syn;
int n;
int i;
bool allFlag = true;
bool legalString = true;
void lrparse();
void statement();
void expression();
void term();
void factor();

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

bool isLegalSymbol(char ch)
{
    if(isDigit(ch) || isLetter(ch) || LegalSymbols.count(ch) == 1 || ch == ' ')
    {
        return true;
    }
    else
    {
        return false;
    }
}

string handleSpace(string s)//一开始犯错删去空格用，后来发现空格是辅助程序的必要东西，所以保留在这急着
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
    //10是自定义变量名,11数字
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
    
    LegalSymbols.insert('+');
    LegalSymbols.insert('-');
    LegalSymbols.insert('*');
    LegalSymbols.insert('/');
    LegalSymbols.insert(':');
    LegalSymbols.insert('=');
    LegalSymbols.insert('<');
    LegalSymbols.insert('>');
    LegalSymbols.insert(';');
    LegalSymbols.insert('(');
    LegalSymbols.insert(')');
    LegalSymbols.insert('#');
    
    Keywords.insert(pair<string,int>("begin",1));
    Keywords.insert(pair<string,int>("if",2));
    Keywords.insert(pair<string,int>("then",3));
    Keywords.insert(pair<string,int>("while",4));
    Keywords.insert(pair<string,int>("do",5));
    Keywords.insert(pair<string,int>("end",6));
    
}

void scanner()//扫一个单词然后输出
{
    token = "";
    
    
    if(legalString)//合法字符串才能进行词法分析
    {
        
        while(inputString[i] == ' ' && i!= inputString.length()-1)
        {
            i++;
        }
        token = "";
        
        if(isLetter(inputString[i]))//标识别符，看看是不是keywords或者ID
        {
            while ((isLetter(inputString[i]) || isDigit(inputString[i]) ) )
            {
                token += inputString[i];
                i++;
            }
            int keywordNumber = isKeyword(token);
            if(keywordNumber == -1)//返回－1表示非关键字
            {
                keywordNumber = 10;
                cout<<"("<<keywordNumber<<","<<token<<")";
            }
            else
            {
                cout<<"("<<keywordNumber<<","<<token<<")";
            }
            syn = keywordNumber;
            token = "";//clear
        }
        else if(isDigit(inputString[i]))//判断是不是数字
        {
            while (isDigit(inputString[i]))
            {
                token += inputString[i];
                i++;
            }
            if(isLetter(inputString[i]))
            {
                cout<<"变量名不能以数字开头"<<endl;
                token = "";
                //break;
            }
            else
            {
                cout<<"("<<11<<","<<token<<")";
                token = "";//clear
            }
            syn = 11;//数字
        }
        //余下字符
        else if( isLegalSymbol(inputString[i]) && !isDigit(inputString[i]) && !isLetter(inputString[i]) && inputString[i] != ' ')
        {
            token += inputString[i];
            i++;
            if((inputString[i-1] == ':' || inputString[i-1] == '<' || inputString[i-1] == '>') && (inputString[i] == '=' || inputString[i] == '>'))//如果下一个字符符合要求，加入
            {
                token += inputString[i];
                i++;
            }
            else//否则i的下一个得放到下一轮判定,i--还原
            {
                //i--;
            }
            int otherwordsNumber = Symbols[token];
            syn = otherwordsNumber;
            cout<<"("<<otherwordsNumber<<","<<token<<")";
            token = "";//clear
        }
        else
        {
            //syn = -999表示读完了都没有正确终止
            syn = -999;
        }
    }
}

//在此之上都是词法分析器的代码，这里开始下面都是语法分析期新加的部分。
void factor()
{
    cout<<"//进入factor"<<endl;
    if(syn == 10 || syn == 11)//10是标识符，11是整常熟
    {
        scanner();
    }
    else if(syn == 27)//是否 (
    {
        scanner();
        expression();
        if(syn == 28)// 接着是右括号的话
        {
            scanner();
        }
        else
        {
            allFlag = false;
            cout<<"\n//error，factor，是否缺少右括号)"<<endl;
        }
    }
    else
    {
        allFlag = false;
        cout<<"\n//语法分析error，factor,非左括号（"<<endl;
    }
    cout<<"\t 退出factor"<<endl;
}

void term()
{
    cout<<"//进入term"<<endl;
    factor();
    while(syn==15 || syn==16) //判断是否 * /
    {
        scanner();
        factor();
    }
    /*if(syn!=15 && syn!= 16 && syn!= 26)//一开始看文档过分相信，看到报错就报错
     {
     allFlag = false;
     cout<<"//term出错，符号不是*或者/"<<endl;
     } */
    cout<<"\t退出term"<<endl;
}

void expression()
{
    cout<<"//进入expression"<<endl;
    term();
    while(syn==13 || syn == 14 )//判断是否是 - +
    {
        scanner();
        term();
    }
    /*if(syn !=13 && syn != 14 && syn != 26)//一开始写代码看到文档出错就报错，错了。。。
     {
     allFlag = false;
     cout<<"//expression出错，不是+ 或 -"<<endl;
     }*/
    cout<<"\t退出expression"<<endl;
}

void statement()
{
    cout<<"//进入Statement"<<endl;
    switch(syn)
    {
        case 10://如果是一个变量
        {
            scanner();
            if(syn == 18)//如果接下来是一个 := 赋值符号
            {
                cout<<"//已有一个变量，且赋值符号:=判定成功，读取数值"<<endl;
                scanner();
                expression();//调用expression
            }
            else
            {
                allFlag = false;
                cout<<"\n//语法error！statement,缺少一个赋值符号 :=\n,但还要读取"<<endl;
                scanner();
                expression();
                //return;
            }
            break;
        }
        case 1:
        {
            allFlag = false;
            cout<<"\n\nstatement出错，又来一个begin，错误！\n\n"<<endl;
            break;
        }
        case 11:
        {
            allFlag = false;
            cout<<"\n//statement,第一个词是一个数字，错误！"<<endl;
            break;
        }
        case 26:
        {
            allFlag = false;
            cout<<"\n\n\t  ; 分号错误\n\n"<<endl;
            break;
        }
        default:
        {
            cout<<"\n第一个词错误！\n"<<endl;
        }
    }
    cout<<"\t 退出statement"<<endl;
}

void lrparse()
{
    cout<<"//进入lrparse"<<endl;
    scanner();//递归下降分析程序中的是begin后的调用scanner
    statement();
    int endFlag = false;
    while(1)
    {
        switch(syn)
        {
            case(26):
            {
                cout<<"//读到一个；一个语句结束了，下一个开始"<<endl;
                scanner();
                statement();
                break;
            }
            case(1):
            {
                cout<<"\n即使两个begin，也要继续分析\n"<<endl;
                scanner();
                statement();
                break;
            }
            case(6):
            {
                endFlag = true;
                cout<<"正常end"<<endl;
                break;
            }
            case (-999):
            {
                allFlag = false;
                endFlag = true;
                cout<<"\nsyn=-999表示读完了，都没有终止符号，很遗憾\n"<<endl;
            }
            default:
            {
                cout<<"该结束了"<<endl;
                endFlag = true;
                break;
            }
        }
        if(endFlag)
        {
            break;
        }
    }
    
    /*if(syn!=26)
     {
     cout<<"缺少;"<<endl;
     } */
    cout<<"\t退出lrparse"<<endl;
}

int main()
{
    preProcess();//预处理，加载symbols,keywords等
    printf("请输入字符串：\n");
    getline(cin,inputString);
    //先遍历检查非法符号
    for (int i = 0; i < inputString.length(); i ++)
    {
        if(isLegalSymbol(inputString[i]) == false)
        {
            cout<<"出现非法字符, 中止程序!"<<endl;
            legalString = false;//没有legalstring就不能开始scanner
        }
    }
    if(legalString)
    {
        //	p = 0;//起始Position从0开始
        i = 0;
        syn = -1;
        //inputString = handleSpace(inputString); 可以把空格运用到分割中，真的编译器不是这么简单的
        //示范性编译输出 
        cout<<"你的输入是："<<inputString<<endl<<"词法分析器演示:"<<endl;
        while(i!=inputString.length())
        {
            scanner();
        }
        cout<<endl<<"现在开始是正式的语法分析器部分:\n"<<endl; 
        i = 0; 
        scanner();
        if(syn == 1)//如果说，这是一个开始begin
        {
            cout<<"//有begin"<<endl;
            lrparse();//begin
        }
        else
        { 
            allFlag = false;
            cout<<"//缺少begin,没有begin也要继续分析"<<endl;
            //即使没有begin，分析也要继续？
            i=0;//没有begin也要重来 
            lrparse();
        }
        if(syn==6)//end
        {
            scanner();
            if(syn==0 && allFlag==true )
            {
                cout<<"Success!"<<endl; 
            }
            else if(syn == 6)
            {
                allFlag = false;
                cout<<"两个end"<<endl;
            } 
            if(allFlag==false)
            {
                cout<<"Unsuccess!"<<endl;
                cout<<"语法错误 "<<endl; 
            }
        }
        else
        {
            printf("缺少end\n");
            cout<<"Unsuccess!"<<endl;
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    return 0;
}

