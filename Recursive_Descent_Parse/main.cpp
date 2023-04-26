#include <iostream>
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
string str;
pair<int,string> p;
pair<int,string> split(string s);
bool S();
bool L();
bool L_();
bool S_();
bool E();
bool E_();
bool T();
bool T_();
bool A();
bool V();
bool M();
bool F();
int main(int argc,char* argv[])
{

    infile.open("/Users/savannasilver/Desktop/program/lab2/test4.txt",ios::in);
    outfile.open("/Users/savannasilver/Desktop/program/lab2/test4_out.txt",ios::out|ios::trunc);

    while(getline(infile,str))
    {   p = split(str);
        string info;
        if(S())
        {
            info = "right!\n";
            outfile<<info;
            cout<<info;
        }
        else
        {
            info = "wrong!\n";
            cout<<info;
            outfile<<info;
        }
    }
    return 0;
}
bool S()
{
    if(p.first==IF)
    {
        if(getline(infile,str))
            p = split(str);
        if(p.first==LX)
        {
            if(getline(infile,str))
                p = split(str);
            if(E())
            {
                if(p.first==RX)
                {
                    if(getline(infile,str))
                        p = split(str);
                    if(p.first==LH)
                    {
                        if(getline(infile,str))
                            p = split(str);
                        if(L())
                        {
                            if(p.first==RH)
                            {
                                if(getline(infile,str))     //可能已经结束了
                                    p = split(str);
                                if(p.first==ELSE)
                                {
                                    if(getline(infile,str))
                                        p = split(str);
                                    if(p.first==LH)
                                    {
                                        if(getline(infile,str))
                                            p = split(str);
                                        if(L())
                                        {
                                            if(p.first==RH)
                                                return true;
                                            else return false;
                                        }
                                        else return false;
                                    }
                                }
                                else return true;
                            }
                            else return false;
                        }
                        else return false;
                    }
                    else return false;
                }
                else return false;
            }
            else return false;
        }
        else return false;
    }
    else if(p.first==ID) {
        if (L()) {
            if (p.first == endofline) {
                if (getline(infile, str))
                    p = split(str);
                return true;
            } else return false;
        }
        else return false;
    }
    else return false;
}
bool L()
{
    if(p.first==ID)
    {
        if(S_())
        {
            if(L_())
            {
                if(getline(infile,str))
                    p = split(str);
                return true;
            }
            else return false;
        }
    }
    else return false;
}
bool L_()
{
    if(p.first==endofline)
    {
        if(getline(infile,str))
            p = split(str);
        if(S_())
        {
            return true;
        }
        else return false;
    }
    else return false;
}
bool S_()
{

    if(p.first==ID)
    {
        if(V())
        {
            if(p.first==EQ)
            {
                if(getline(infile,str))
                    p = split(str);
                return E();
            }
            else return false;
        }
        else return false;
    }
    else return false;
}
bool E()
{
    if(p.first==ID||p.first==LX||p.first==NUM) //
    {
        if(T())
        {
            if(E_())
            {
                return true;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}
bool E_()
{

    if(p.first==PLUS||p.first==MINUS)
    {
        if(A())
        {
            if(T())
            {
                return E_();
            }
            else return false;
        }
        else return false;
    }
    else
    {
        if(p.first==RX||p.first==endofline){
            return true;
        }
        else return false;
    }
}
bool T()
{

    if(p.first==ID||p.first==LX||p.first==NUM)
    {
        if(F())
        {
            if(T_())
            {
                return true;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}
bool T_()
{

    if(p.first==MULTI||p.first==DIVID)
    {
        if(M())
        {
            if(F())
            {
                return T_();
            }
            else return false;
        }
        else return false;
    } else{
        if(p.first==RX||p.first==endofline||p.first==PLUS||p.first==MINUS)
        {
            return true;
        }
        else return false;
    }
}
bool M()
{

    if(p.first==MULTI||p.first==DIVID)
    {
        if(getline(infile,str))
            p = split(str);
        return true;
    }
    else return false;
}
bool A()
{

    if(p.first==PLUS||p.first==MINUS)
    {
        if(getline(infile,str))
            p = split(str);
        return true;
    }
    else return false;
}
bool F()
{

    if(p.first==LX)
    {
        if(getline(infile,str))
            p = split(str);
        if(E())
        {
            if(p.first==RX)
            {
                if(getline(infile,str))
                    p = split(str);
                return true;
            }
            else return false;
        }
        else return false;
    }
    else if(p.first==ID||p.first==NUM)
    {
        if(getline(infile,str))
            p = split(str);
        return true;
    }
    else return false;
}
bool V()
{
    if(p.first==ID)
    {
        if(getline(infile,str))
            p = split(str);
        return true;
    }
    else return false;
}
pair<int,string> split(string s)
{
    int index = s.find(',');
    string s1 = s.substr(1,index-1);
    int ans = stoi(s1);
    string s2 = s.substr(index+1,s.size()-index-2);
    return {ans,s2};
}