#include<iostream>
#include<fstream>
#include<stdio.h>
#include<map>
using namespace std;
#define VOID 1  //void
#define INT 2   //int
#define FLOAT 3 //float
#define DOUBLE 4    //double
#define IF 5    //if
#define ELSE 6  //else
#define FOR 7   //for
#define DO 8    //do
#define WHILE 9 //while
#define PLUS 10 //+
#define MINUS 11    //-
#define MULTI 12    //*
#define DIVID 13   //   /
#define DG 14   //>>
#define DL 15   //<<
#define PE 16   //+=
#define ME 17   //-=
#define MULE 18 //*=
#define DIVE 19 // /=
#define AND 20  // &
#define OR 21   // |
#define AA 22   // &&
#define OO 23   // ||
#define NOT 24  // !
#define LH  25  //{
#define RH  26  //}
#define LZ  27  //[
#define RZ  28  //]
#define LX  29  //(
#define RX  30  //)
#define COMMENT 31  // <>
#define COMMENT_BEFORE 32   // /*
#define COMMENT_AFTER 33    // */
#define RETURN 34//return
#define PRINT 35    //printf
#define SCAN 36 //scanf
#define SINGEL_COMMENT 37//   //
#define EE 38   //==
#define NE 39   //!=
#define LT 40   //<
#define GT 41   //>
#define comma 42    //,
#define endofline 43    //;
#define MAIN 44 //main
#define EQ 45   //=
#define PP 46   //++
#define MM 47   //--
#define NUM 48  //数字
#define ID  49  //用户定义的标识符
fstream infile,outfile;
map<string,int> lookup;
void init()
{
    lookup["void"]=1;
    lookup["int"]=2;
    lookup["float"]=3;
    lookup["double"]=4;
    lookup["if"]=5;
    lookup["else"]=6;
    lookup["for"]=7;
    lookup["do"]=8;
    lookup["while"]=9;
    lookup["+"]=10;
    lookup["-"]=11;
    lookup["*"]=12;
    lookup["/"]=13;
    lookup[">>"]=14;
    lookup["<<"]=15;
    lookup["+="]=16;
    lookup["-="]=17;
    lookup["*="]=18;
    lookup["/="]=19;
    lookup["&"]=20;
    lookup["|"]=21;
    lookup["&&"]=22;
    lookup["||"]=23;
    lookup["!"]=24;
    lookup["{"]=25;
    lookup["}"]=26;
    lookup["["]=27;
    lookup["]"]=28;
    lookup["("]=29;
    lookup[")"]=30;
    lookup["<>"]=31;
    //lookup["/*"]=32;
    //lookup["*/"]=33;
    lookup["return"]=34;
    lookup["printf"]=35;
    lookup["scanf"]=36;
    //lookup["//"]=37;
    lookup["=="]=38;
    lookup["!="]=39;
    lookup["<"]=40;
    lookup[">"]=41;
    lookup[","]=42;
    lookup[";"]=43;
    lookup["main"]=44;
    lookup["="]=45;
    lookup["++"]=46;
    lookup["--"]=47;
}
void OUT(int c,string val="null")
{   if(c==ID||c==NUM) {
        string str = "(" + to_string(c) + "," + val + ")";
        outfile << str<<endl;
    } else{
        string str = "(" + to_string(c) + "," + val + ")";
        outfile<< str<<endl;
    }
}
int main(int argc,char* argv[])
{
    if(argc==1) {
        infile.open("/Users/savannasilver/Desktop/program/C++/error_test_1.txt", ios::in);
        outfile.open("/Users/savannasilver/Desktop/program/C++/error_output1.txt",ios::out|ios::trunc);
        cout<<"error test No.1"<<endl;
    }
    else
    {
        int a = stoi(argv[1]);
        switch (a) {
            case 1:
            {
                infile.open("/Users/savannasilver/Desktop/program/C++/error_test_1.txt", ios::in);
                outfile.open("/Users/savannasilver/Desktop/program/C++/error_output1.txt",ios::out|ios::trunc);
                cout<<"error test No.1"<<endl;
                break;
            }
            case 2:
            {
                infile.open("/Users/savannasilver/Desktop/program/C++/error_test_2.txt",ios::in);
                outfile.open("/Users/savannasilver/Desktop/program/C++/error_output2.txt",ios::out|ios::trunc);
                cout<<"error test No.2"<<endl;
                break;
            }
            case 3:
            {
                infile.open("/Users/savannasilver/Desktop/program/C++/success_test_1.txt",ios::in);
                outfile.open("/Users/savannasilver/Desktop/program/C++/success_output1.txt",ios::out|ios::trunc);
                cout<<"success test No.1"<<endl;
                break;
            }
            case 4:
            {
                infile.open("/Users/savannasilver/Desktop/program/C++/success_test_2.txt",ios::in);
                outfile.open("/Users/savannasilver/Desktop/program/C++/success_output2.txt",ios::out|ios::trunc);
                cout<<"success test No.2"<<endl;
                break;
            }

        }
    }
    char c;
    init();
    bool flag=false,flag2=false;
    /*  是否为注释，true为当前是注释,flag为多行注释，flag2为单行注释
     *
     * */
    while((c=infile.get())!=EOF) {
        //标识符
        if(isalpha(c)&&!flag&&!flag2)
        {
            string s= "";
            while(isalpha(c)||isdigit(c))
            {
                s+=c;
                c=infile.get();
            }
            infile.seekg(-1,ios::cur);
            if(lookup[s]!=0){
                OUT(lookup[s]);
            } else{ //用户定义的
                if(s.size()>32){
                    string s = "error: over 32 characters\n";
                    cout<<"error information put to txt file"<<endl;
                    outfile<<s;
                }
                else {
                    OUT(ID, s);
                }
            }
        }
        //无符号整数
        else if(isdigit(c)&&!flag&&!flag2)
        {
            string s= "";
            while(isdigit(c))
            {
                s+=c;
                c=infile.get();
            }
            infile.seekg(-1,ios::cur);
            c = infile.get();
            infile.seekg(-1,ios::cur);
            if(isalpha(c))  //数字加字母是不允许的
            {
                cout<<"error information put to txt file"<<endl;
                string s = "error: unknown number\n";
                outfile<<s;
            }
            else if(lookup[s]!=0)
            {
                OUT(lookup[s]);
            } else{ //用户定义的
                OUT(NUM,s);
            }
        }
        //
        else if(c==' '||c=='\n') {
            if (c == '\n') flag2 = false;
            continue;
        }
        else {
            if(!flag&&!flag2) {
                switch (c) {
                    case ',':{
                        OUT(lookup[","]);
                        break;
                    }
                    case ';':{
                        OUT(lookup[";"]);
                        break;
                    }
                    case '+': {
                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup["+"]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '=') {
                            OUT(lookup["+="]);
                        }else if(c=='+')
                        {
                            OUT(lookup["++"]);
                        }
                        else {
                            OUT(lookup["+"]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '-': {
                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup["-"]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '=') {
                            OUT(lookup["-="]);
                        }else if(c=='-')
                        {
                            OUT(lookup["--"]);
                        }
                        else {
                            OUT(lookup["-"]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '*': {

                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup["*"]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '=') {
                            OUT(lookup["*="]);
                        } else if (c == '/') {
                            OUT(lookup["*/"]);
                            flag = false;
                        } else {
                            OUT(lookup["*"]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '/': {
                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup["/"]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '=') {
                            OUT(lookup["/="]);
                        } else if (c == '*') {
                            flag = true;
                        } else if (c == '/') {
                            flag2 = true;
                        } else {
                            OUT(lookup["/"]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '<': {
                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup["<"]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '=') {
                            OUT(lookup["<="]);
                        } else if (c == '<') {
                            OUT(lookup["<<"]);
                        }else if(c=='>')
                        {
                            OUT(lookup["<>"]);
                        }
                        else {
                            OUT(lookup["<"]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '>': {
                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup[">"]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '=') {
                            OUT(lookup[">="]);
                        } else if (c == '>') {
                            OUT(lookup[">>"]);
                        } else {
                            OUT(lookup[">"]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '=': {
                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup["="]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '=') {
                            OUT(lookup["=="]);
                        } else {
                            OUT(lookup["="]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '!': {
                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup["!"]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '=') {
                            OUT(lookup["!="]);
                        } else {
                            OUT(lookup["!"]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '&': {
                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup["&"]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '&') {
                            OUT(lookup["&&"]);
                        } else {
                            OUT(lookup["&"]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '|': {
                        c = infile.get();
                        if(c==EOF)
                        {
                            OUT(lookup["|"]);
                            infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                            break;
                        }
                        if (c == '|') {
                            OUT(lookup["||"]);
                        } else {
                            OUT(lookup["|"]);
                            infile.seekg(-1, ios::cur); //不是就back
                        }
                        break;
                    }
                    case '{': {
                        OUT(lookup["{"]);
                        break;
                    }
                    case '}': {
                        OUT(lookup["}"]);
                        break;
                    }
                    case '(': {
                        OUT(lookup["("]);
                        break;
                    }
                    case ')': {
                        OUT(lookup[")"]);
                        break;
                    }
                    case '[': {
                        OUT(lookup["["]);
                        break;
                    }
                    case ']': {
                        OUT(lookup["]"]);
                        break;
                    }
                    default:
                    {
                        string error="error: unknown single character\n";
                        cout<<"error information put to txt file"<<endl;
                        outfile<<error;
                    }

                }
            }
            else{//检查注释是否结束
                if(c=='*')
                {
                    c=infile.get();
                    if(c==EOF)
                    {
                        OUT(lookup["*"]);
                        infile.seekg(-1,ios::cur);  //防止在while里又读一遍
                        break;
                    }
                    if(c=='/')
                    {
                        flag=false;
                    }
                    else
                    {
                        infile.seekg(-1,ios::cur);
                    }
                }
            }
        }
    }
    outfile.close();
    infile.close();
    cout<<"complete"<<endl;
    return 0;
}

