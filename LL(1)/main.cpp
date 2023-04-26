#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<map>
#include<set>
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
string S;
vector<string> nter;  //非终结符
vector<string> ter;   //终结符
map<string,vector<string>> P; //规则
map<string,set<string>> first;
map<string,set<string>> follow;
map<string,map<string,string>> table;
vector<string> st;
int first_sum();
int follow_sum();
void lab3_init();
void first_init();
void follow_init();
void table_init();
pair<int,string> split(string s);
string init_string();
int main()
{
    //下面是初始化
    lab3_init();
    first_init();
    follow_init();
    cout<<"\n------------------------------first集和follow集----------------------------"<<endl;
    cout<<"       first             follow"<<endl;
    for(auto i : first)
    {   if(find(nter.begin(),nter.end(),i.first)-nter.begin()==nter.size()) continue;
        cout<<i.first<<"      ";
        string info1;
        for(auto j : first[i.first])
            info1+=j+" ";
        cout<<info1;
        for(int j = 17-info1.size();j>=0;--j) cout<<" ";
        for(auto j : follow[i.first])
            cout<<j<<" ";
        cout<<endl;
    }
    table_init();
    cout<<"\n------------------------------LL(1)分析表----------------------------"<<endl;
    cout<<"\t";
    for(auto i:ter) cout<<i<<"\t\t";
    cout<<endl;
    for(auto i : table)
    {   cout<<i.first<<"\t";

        for(auto j : ter)
        {   if(table[i.first][j]=="null") cout<<"N"<<"\t\t";
            else    cout<<table[i.first][j]<<"\t\t";
        }
        cout<<endl;
    }
    /*
     * right test:
     * test1 a = a + b
     * test2 a = a * ( a + b ) - c;
     * wrong test:
     * test3 a + b;
     * test4 a = a + b);
     */

    infile.open("/Users/savannasilver/Desktop/program/lab3/test2.txt",ios::in);
    outfile.open("/Users/savannasilver/Desktop/program/lab3/test2_out.txt",ios::out|ios::trunc);

    string str = init_string();
    st.push_back("#");
    st.push_back(S);

    int index=0;
    cout<<"\n------------------------------分析过程----------------------------"<<endl;
    cout<<"  分析栈          余留输入串          产生式"<<endl;
    while(!st.empty())
    {   string info1;
        for(auto i :st)
            info1+=i;
        cout<<" "<<info1;
        for(int ii = 14-info1.size();ii>=0;--ii) cout<<" ";
        cout<<str.substr(index);
        for(int ii = 18-str.substr(index).size();ii>=0;--ii) cout<<" ";
        if(index==str.size())   //说明没有分号
        {
            cout<<"\nwrong!"<<endl;
            outfile<<"wrong!"<<endl;
            break;
        }
        string top = st.back();
        string temp = str.substr(index,1);
        if(find(ter.begin(),ter.end(),top)-ter.begin()!=ter.size()) //顶是终结符号
        {
            if(top==temp)
            {   cout<<"nextsym";
                cout<<endl;
                st.pop_back();
                index++;

                continue;
            }
            else
            {
                cout<<"\nwrong!"<<endl;
                outfile<<"wrong!"<<endl;
                break;
            }
        }
        else
        {
            if(top=="#")
            {
                if(temp==";")
                {
                    cout<<"\nright!"<<endl;
                    outfile<<"right!"<<endl;
                    break;
                }
                else
                {
                    cout<<"\nwrong!"<<endl;
                    outfile<<"wrong!"<<endl;
                    break;
                }
            }
            else
            {
                if(table[top].count(temp))
                {
                    string temp2 = table[top][temp];
                    if(temp2=="null")
                    {   cout<<"null";
                        cout<<endl;
                        st.pop_back();
                        continue;
                    }
                    cout<<top<<"->"<<temp2<<endl;
                    reverse(temp2.begin(),temp2.end());
                    st.pop_back();
                    for(int i = 0 ;i<temp2.size();++i)
                    {
                        string temp3 = temp2.substr(i,1);
                        st.push_back(temp3);
                    }

                }
                else
                {
                    cout<<"wrong!"<<endl;
                    outfile<<"wrong!"<<endl;
                    break;
                }
            }
        }
    }
    infile.close();
    outfile.close();
    return 0;
}
void lab3_init()
{
    fstream in;
    in.open("/Users/savannasilver/Desktop/program/lab3/init.txt",ios::in);
    string s;
    getline(in,s);
    S = s;
    getline(in,s);
    stringstream str(s);
    string temp;
    while(str>>temp)
    {
        nter.push_back(temp);
    }
    getline(in,s);
    str.clear();
    str.str(s);
    while(str>>temp)
    {
        ter.push_back(temp);
    }
    while(getline(in,s))
    {
        str.clear();
        str.str(s);
        str>>temp;
        string begin = temp;
        vector<string> temp2;
        P[begin]=temp2;
        while(str>>temp)
        {
            P[begin].push_back(temp);
        }

    }
    in.close();
}
int first_sum()
{
    int sum = 0;
    for(auto i : first)
    {
        sum+=i.second.size();
    }
    return sum;
}
void first_init()
{
    for(auto i : ter)   //终结符的first就是终结符
    {   set<string> v;
        first[i]=v;
        first[i].insert(i);
    }
    for(auto i : P)
    {
        vector<string> right = P[i.first];
        for(auto j : right)
        {   if(j=="null")
            {
                first[i.first].insert("null");
            }
            else {
                string temp = j.substr(0, 1);
                if(find(ter.begin(),ter.end(),temp)-ter.begin()!=ter.size())    //是终结符号
                {
                    first[i.first].insert(temp);
                }
            }

        }
    }
    int size;
    do {
        size = first_sum();
        for(auto i : P)
        {
            for(auto j : i.second)  //规则对应的结果
            {
                if(j=="null")
                {
                    continue;
                }
                string temp = j.substr(0,1);
                if(find(nter.begin(),nter.end(),temp)-nter.begin()!=nter.size())    //首字母是终结符号
                {
                    for(auto k : first[temp])
                    {
                        if(k=="null")
                        {
                            continue;
                        }
                        first[i.first].insert(k);   //把第一个非终结符号的first加入当前符号中. i->(temp)ab;
                    }
                }
            }
            for(auto j : i.second)  //每个规则对应的结果 j -> string
            {
                if(j=="null") continue;
                for(int k = 1;k<j.size();++k)
                {
                    string temp = to_string(j[k-1]);
                    if(first[temp].count("null"))   //如果包含空
                    {
                        string temp2 = to_string(j[k]);
                        for(auto kk : first[temp2])     //kk -> set<string>
                        {
                            if(kk=="null") continue;
                            first[i.first].insert(kk);
                        }
                    }
                }
            }
            for(auto j : i.second)
            {
                if(j=="null") continue;
                int count=0;
                for(int k = 0;k<j.size();++k)
                {
                    string temp = to_string(j[k]);
                    if(first[temp].count("null"))   //如果包含空
                    {
                        count++;
                    }
                }
                if(count==j.size())
                {
                    first[i.first].insert("null");
                }

            }

        }
    }while(size!=first_sum());
    vector<string> v;
    for(auto i : first)
    {
        string temp = i.first;
        if(find(ter.begin(),ter.end(),temp)-ter.begin()==ter.size()&&find(nter.begin(),nter.end(),temp)-nter.begin()==nter.size())
            v.push_back(i.first);
    }
    for(auto i : v)
    {
        first.erase(i);
    }
    for(auto i : P)
    {
        for(auto j : i.second)  //j->string
        {
            if(j=="null") continue;
            string temp = j.substr(0,1);
            for(auto k : first[temp])   //k -> string
            {
                if(k=="null") continue;
                first[j].insert(k);
            }
            for(int k = 1;k<j.size();++k)
            {
                string temp = j.substr(k-1,1);
                if(first[temp].count("null"))   //如果包含空
                {
                    string temp2 = j.substr(k,1);
                    for(auto kk : first[temp2])     //kk -> set<string>
                    {
                        if(kk=="null") continue;
                        first[i.first].insert(kk);
                    }
                }
            }
        }
    }
}
int follow_sum()
{
    int sum = 0;
    for(auto i : follow)
    {
        sum+=i.second.size();
    }
    return sum;
}
void follow_init()
{
    follow[S].insert(";");
    for(auto i : P)
    {
        for(auto j : i.second)
        {
            if(j=="null") continue;
            for(auto k =0;k<j.size()-1;++k)
            {
                string temp = j.substr(k,1);
                string temp2 = j.substr(k+1,1);
                if(find(nter.begin(),nter.end(),temp)-nter.begin()!=nter.size())
                {
                    for(auto kk : first[temp2]) //kk->string
                    {
                        if(kk=="null") continue;
                        follow[temp].insert(kk);
                    }
                }
            }
        }
    }
    int size;
    do{
        size = follow_sum();
        for(auto i : P)
        {
            for(auto j : i.second)
            {
                if(j=="null") continue;
                string temp = j.substr(j.size()-1,1);
                if(find(nter.begin(),nter.end(),temp)-nter.begin()!=nter.size())    //是非终结符号
                    for(auto k : follow[i.first])
                    {
                        follow[temp].insert(k);
                    }
                for(int k = j.size()-1;k>=1;--k)
                {
                    string temp2 = j.substr(k,1);
                    if(first[temp2].count("null"))
                    {
                        string temp3 = j.substr(k-1,1);
                        for(auto kk: follow[i.first])
                        {
                            follow[temp3].insert(kk);
                        }
                    }
                }

            }

        }
    }while(size!=follow_sum());
}
void table_init()
{

    for(auto i : P)
    {
        for(auto j : i.second)
        {
            if(j=="null")
            {
                for(auto b : follow[i.first])
                {
                    if(find(ter.begin(),ter.end(),b)-ter.begin()!=ter.size()||b==";")
                    {
                        table[i.first][b]="null";
                    }
                }
            }
            else
            {
                for(auto a : first[j])  //k->string
                {
                    table[i.first][a]=j;
                }
            }
        }
    }
}
pair<int,string> split(string s)
{
    int index = s.find(',');
    string s1 = s.substr(1,index-1);
    int ans = stoi(s1);
    string s2 = s.substr(index+1,s.size()-index-2);
    return {ans,s2};
}
string init_string()
{
    string res="";
    string temp;
    while(getline(infile,temp))
    {
        pair<int,string> p = split(temp);
        if(p.first==ID)
        {
            res+="i";
        }
        else
        {
            if(p.first==EQ)
            {
                res+="=";
            }
            else if(p.first==PLUS)
            {
                res+="+";
            }
            else if(p.first==MINUS)
            {
                res+="-";
            }
            else if(p.first==MULTI)
            {
                res+="*";
            }
            else if(p.first==DIVID)
            {
                res+="/";
            }
            else if(p.first==endofline)
            {
                res+=";";
            }
            else if(p.first==LX)
            {
                res+="(";
            }
            else if(p.first==RX)
            {
                res+=")";
            }
        }
    }
    return res;
}
