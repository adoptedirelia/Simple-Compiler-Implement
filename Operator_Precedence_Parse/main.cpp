#include <iostream>
#include<map>
#include<vector>
#include<set>
#include<fstream>
#include<sstream>
#include<algorithm>
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
map<string,set<string>> firstvt;
map<string,set<string>> lastvt;
map<string,map<string,string>> table;
set<string> error_set{"(+","(-","(*","(/","#+","#*","#-","#/","+#","-#","*#","/#","+)","-)","*)","/)"};
void lab4_init();
void firstvt_init();
void lastvt_init();
void table_init();
void show_table();
void delete_stack(vector<string>&,int,int);
pair<int,string> split(string s);
string init_string();
int main() {

    lab4_init();
    firstvt_init();
    lastvt_init();
    table_init();
    //show_table(); //测试

    /*
     * right:
     * test1 a * (a + b) - c;
     * test2 c * (a * (a + b) - c);
     * wrong:
     * test3 c * ( a + ;
     * test4 * a + c;
     */
    infile.open("/Users/savannasilver/Desktop/program/lab4/test4.txt",ios::in);
    outfile.open("/Users/savannasilver/Desktop/program/lab4/test4_out.txt",ios::out|ios::trunc);

    string test = init_string();

    vector<string> st;
    st.push_back("#");
    int i=0,j=0,index=0;
    string R,Q;
    do{

        R = test.substr(index,1);
        index++;
        string s = st.back()+R;
        if(error_set.count(s)) {

            break; }
        if(find(ter.begin(),ter.end(),st[i])-ter.begin()!=ter.size()){
            j=i;
        }
        else{
            j=i-1;
        }
        while(table[st[j]][R]==">")
        {
            string q;
            do{
                q = st[j];
                if(find(ter.begin(),ter.end(),st[j-1])-ter.begin()!=ter.size())
                {
                    j--;
                }
                else{
                    j-=2;
                }
            }while(table[st[j]][q]!="<");
            string str = "";
            delete_stack(st, j + 1, i + 1);
            i = j + 1;
            st.push_back("N");
        }
        if(table[st[j]][R]=="<"||table[st[j]][R]=="="){
            i++;
            st.push_back(R);
        }
        else{
            cout<<"wrong!"<<endl;
            break;
        }
    }while(R!="#");
    string ans= "";
    for(int i = 0;i<st.size();++i)
    {
        string temp = st[i];
        ans+=temp;
    }
    for(auto i : st) cout<<i<<" ";
    cout<<endl;
    if(ans=="#N#") {
        cout << "right!" << endl;
        outfile<<"right!"<<endl;
    }
    else {
        cout << "wrong!" << endl;
        outfile<<"wrong!"<<endl;
    }
    return 0;
}
void lab4_init()
{
    fstream in;
    in.open("/Users/savannasilver/Desktop/program/lab4/init.txt",ios::in);
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
void firstvt_init()
{
    vector<pair<string,string>> st;
    for(auto i : P)
    {
        for(auto j : i.second)
        {
            string temp = j.substr(0,1);
            string temp2 = j.substr(1,1);
            if(find(ter.begin(),ter.end(),temp)-ter.begin()!=ter.size())
            {
                firstvt[i.first].insert(temp);
                pair<string,string> p{i.first,temp};
                st.push_back(p);
            }
            else if(find(ter.begin(),ter.end(),temp2)-ter.begin()!=ter.size())
            {
                firstvt[i.first].insert(temp2);
                pair<string,string> p{i.first,temp2};
                st.push_back(p);
            }
        }
    }
    while(!st.empty())
    {
        pair<string,string> p = st.back();
        st.pop_back();
        for(auto i : P)
        {
            for(auto j : i.second)
            {
                string temp = j.substr(0,1);
                if(temp==p.first){
                    if(firstvt[i.first].count(p.second))       //如果已经包含
                    {
                        continue;
                    }
                    else
                    {
                        firstvt[i.first].insert(p.second);
                        st.push_back({i.first,p.second});
                    }
                }
                else continue;
            }
        }
    }
}
void lastvt_init()
{
    vector<pair<string,string>> st;
    for(auto i : P)
    {
        for(auto j : i.second)
        {
            string temp = j.substr(j.size()-1,1);


            if(find(ter.begin(),ter.end(),temp)-ter.begin()!=ter.size())
            {
                lastvt[i.first].insert(temp);
                pair<string,string> p{i.first,temp};
                st.push_back(p);
            }

            if(j.size()>=2)
            {
                string temp2 = j.substr(j.size()-2,1);
                if(find(ter.begin(),ter.end(),temp2)-ter.begin()!=ter.size())
                {
                    lastvt[i.first].insert(temp2);
                    pair<string,string> p{i.first,temp2};
                    st.push_back(p);
                }
            }
        }
    }
    while(!st.empty())
    {
        pair<string,string> p = st.back();
        st.pop_back();
        for(auto i : P)
        {
            for(auto j : i.second)
            {
                string temp = j.substr(j.size()-1,1);
                if(temp==p.first){
                    if(lastvt[i.first].count(p.second))       //如果已经包含
                    {
                        continue;
                    }
                    else
                    {
                        lastvt[i.first].insert(p.second);
                        st.push_back({i.first,p.second});
                    }
                }
                else continue;
            }
        }
    }
}
void table_init()
{
    for(auto i : ter)
    {
        for(auto j : ter)
        {
            table[i][j]="ER";
        }
    }
    table["#"]["#"]="acc";
    for(auto i : P)
    {
        for(auto j : i.second)
        {   if(j.size()==1) continue;
            for(int k = 0;k<j.size()-1;++k)
            {
                string temp=j.substr(k,1);
                string temp2 = j.substr(k+1,1);
                //...ab...
                if(find(ter.begin(),ter.end(),temp)-ter.begin()!=ter.size()&&find(ter.begin(),ter.end(),temp2)-ter.begin()!=ter.size())
                {
                    table[temp][temp2]="=";
                }
                //...aU...
                else if(find(ter.begin(),ter.end(),temp)-ter.begin()!=ter.size()&&find(nter.begin(),nter.end(),temp2)-nter.begin()!=nter.size())
                {
                    for(auto kk:firstvt[temp2])
                    {
                        table[temp][kk]="<";
                    }
                }
                //...Ub...
                else if(find(nter.begin(),nter.end(),temp)-nter.begin()!=nter.size()&&find(ter.begin(),ter.end(),temp2)-ter.begin()!=ter.size())
                {
                    for(auto kk : lastvt[temp])
                    {
                        table[kk][temp2]=">";
                    }
                }

            }
            for(int k =0;k<j.size()-2;++k)
            {   string temp = j.substr(k,1);
                string temp2 = j.substr(k+1,1);
                string temp3 = j.substr(k+2,1);
                if(find(ter.begin(),ter.end(),temp)-ter.begin()!=ter.size()&&find(nter.begin(),nter.end(),temp2)-nter.begin()!=nter.size()
                &&find(ter.begin(),ter.end(),temp3)-ter.begin()!=ter.size())
                {
                    table[temp][temp3]="=";
                }
            }
        }
    }
}
void show_table()
{
    cout<<"\t";
    for(auto i : table)
        cout<<i.first<<"\t";
    cout<<endl;
    for(auto i : table)
    {   cout<<i.first<<"\t";
        for(auto j : i.second)
        {
            cout<<j.second<<"\t";
        }
        cout<<endl;
    }
}
void delete_stack(vector<string> &v,int begin,int end)
{
    v.erase(v.begin()+begin,v.begin()+end);
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
                res+="";
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
    res+="#";
    return res;
}
