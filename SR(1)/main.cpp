#include <iostream>
#include<map>
#include<unordered_map>
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
string point = "`";
int idx = 0;
vector<string> nter;  //非终结符
vector<string> ter;   //终结符
map<string,vector<pair<string,int>>> P; //规则
map<int,map<string,int>> table;
map<int,pair<string,string> > rule_and_index; //index -> (P -> Q)
struct state
{
    int index;
    set<pair<string,int>> rules;
    map<string,state> next;
    bool terminal = false;
};
struct char_state
{
    string str;
    string place;
};
vector<state> all_state;
void lab5_init();
void init_state();
set<pair<string,int>> closure(set<pair<string,int>>);
pair<int,string> split(string s);
string init_string();
void init_table();
void show_stack(vector<int>,vector<char_state>);
vector<pair<int,string>> init_v();
void doit(int num,vector<int>&,vector<char_state>&);
void generate(string op,string opnum1,string opnum2,string s);
int temp_num = 0;
int main() {

    lab5_init();
    init_state();
    init_table();
    cout<<"-------------规则序号--------------"<<endl;
    for(auto i : rule_and_index)
    {
        cout<<"     "<<i.first<<"          "<<i.second.first<<"->"<<i.second.second<<endl;
    }
    cout<<endl;
    cout<<"-------------------------------------状态集合--------------------------------------"<<endl;

    for(auto i :all_state)
    {
        cout<<i.index<<"\t\t";
        for(auto j : i.rules)
        {
            cout<<j.first<<"      ";
        }
        cout<<endl;
    }
    cout<<endl;

    cout<<"-------------------------------------SLR(1)分析表--------------------------------------"<<endl;
    for(auto i : table)
    {
        cout<<i.first<<"\t\t";
        for(auto j : i.second)
        {
            cout<<j.first<<" "<<j.second<<"     ";
        }
        cout<<endl;
    }
    infile.open("/Users/savannasilver/Desktop/program/lab5/test4.txt",ios::in);
    outfile.open("/Users/savannasilver/Desktop/program/lab5/test4_out.txt",ios::out|ios::trunc);
    /*
     * test1:  a = b + (c + d)*f - m
     * test2:  f = a - c/d
     * test3:  a = b - ( c +d
     * test4:  1 = b +a
     */
    vector<pair<int,string>> s= init_v();
    int s_index = 0;
    vector<int> state_s; //记录数字
    vector<char_state> char_s;  //记录字母
    state_s.push_back(0);
    char_state cs;
    cs.str = "#";
    char_s.push_back(cs);
    cout<<"-------------------------------------分析过程--------------------------------------"<<endl;
    while(char_s.back().str!="S")
    {

        pair<int,string> p = s[s_index];
        pair<int,string> temp;
        if(p.first==NUM||p.first==ID)
        {
            temp = pair<int,string>{p.first,"i"};
        }
        else{
            if(p.first==EQ)
            {
                temp = pair<int,string>{p.first,"="};
            }
            else if(p.first==PLUS)
            {
                temp = pair<int,string>{p.first,"+"};
            }
            else if(p.first==MINUS)
            {
                temp = pair<int,string>{p.first,"-"};
            }
            else if(p.first==MULTI)
            {
                temp = pair<int,string>{p.first,"*"};
            }
            else if(p.first==DIVID)
            {
                temp = pair<int,string>{p.first,"/"};
            }
            else if(p.first==LX)
            {
                temp = pair<int,string>{p.first,"("};
            }
            else if(p.first==RX)
            {
                temp = pair<int,string>{p.first,")"};
            }
        }
        int now = state_s.back();

        if(table[now].count(temp.second))  //存在
        {
            int next = table[now][temp.second];
            state_s.push_back(next);
            if(temp.second=="i") {
                if(char_s.back().str=="#"&&temp.first==NUM)
                {
                    cout<<"赋值号左面不能出现数字"<<endl;
                    break;
                }
                char_s.push_back({temp.second, p.second});


            }
            else char_s.push_back({temp.second,""});
            show_stack(state_s,char_s);
            s_index++;
        }
        else if(table[now].count("规约"))  //是规约
        {
            int num = table[now]["规约"]; //用第num个规则规约

            doit(num,state_s,char_s);

        }
        else break;

    }
    if(char_s.back().str=="S")
        cout<<"right!";
    else cout<<"wrong!";
    return 0;
}

void lab5_init()
{
    fstream in;
    in.open("/Users/savannasilver/Desktop/program/lab5/init.txt",ios::in);
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
    int index=0;
    while(getline(in,s))
    {
        str.clear();
        str.str(s);
        str>>temp;
        string begin = temp;
        vector<pair<string,int>> temp2;
        P[begin]=temp2;
        while(str>>temp)
        {
            P[begin].push_back({temp,index++});
            rule_and_index[index-1]=pair<string,string>{begin,temp};
        }

    }
    in.close();
}
void init_state()
{
    auto begin = P[S];
    set<pair<string,int>> v;
    v.insert({point + begin[0].first,0});
    set<pair<string,int>> temp = closure(v);
    state c0;
    c0.index=idx;
    c0.rules=temp;
    all_state.push_back(c0);

    int sum = all_state.size();
    do {
        state now_c = all_state[idx];
        set<pair<string,int>> next_step;   //表示可以吸收的符号
        for(auto str : now_c.rules)
        {
            for(int k=0;k<str.first.size()-1;++k)
            {
                if(str.first[k]=='`')
                {
                    string next_ch = str.first.substr(k+1,1);
                    next_step.insert({next_ch, str.second});
                    break;
                }
            }
        }
        for(auto ch:next_step)      //寻找
        {
            string temp = ch.first;     //符号是什么
            set<pair<string,int>> temp2;    //放入next集
            for(auto str : now_c.rules)     //在现在该迭代的set中寻找
            {
                for(int k=0;k<str.first.size()-1;++k)
                {
                    if(str.first[k]=='`')
                    {
                        string next_ch = str.first.substr(k+1,1);
                        if(next_ch==temp)
                        {
                            string temp3 = str.first;
                            temp3.insert(k+2,"`");

                            temp3.erase(k,1);

                            temp2.insert({temp3, str.second});
                            break;
                        }

                    }
                }
            }
            state new_c;
            temp2 = closure(temp2);
            bool flag = true;
            for(int i=0;i<all_state.size();++i)
            {
                if(all_state[i].rules==temp2) {
                    flag = false;

                    all_state[idx].next[ch.first]=all_state[i];


                }
            }
            if(flag) {
                new_c.index = all_state.size();
                new_c.rules = temp2;
                all_state[idx].next[ch.first] = new_c;
                all_state.push_back(new_c);
            }
        }


        idx++;
    }while(sum!=all_state.size()&&idx<all_state.size());
};
set<pair<string,int>> closure(set<pair<string,int>> s)
{
    set<pair<string,int>> v;
    for(auto str : s) //i -> string
    {
        v.insert(str);
    }

    int sum = v.size();
    do {
        sum = v.size();
        for(auto str : v)
        {
            for(int k = 0;k<str.first.size()-1;++k)
            {
                if(str.first[k]=='`')
                {
                    string next_ch = str.first.substr(k+1,1);
                    if(find(nter.begin(),nter.end(),next_ch)-nter.begin()!=nter.size())//是终结符号
                    {
                        for(auto ans : P[next_ch])
                        {
                            v.insert({"`" + ans.first,ans.second});
                        }

                        break;
                    }

                }
            }
        }
    }while(sum!=v.size());

    return v;
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
vector<pair<int,string>> init_v()
{   string temp;
    vector<pair<int,string>> ans;
    while(getline(infile,temp))
    {
        ans.push_back((split(temp)));
    }
    return ans;
}
void init_table()
{
    for(int i=0;i<all_state.size();++i)
    {
        for(auto j:all_state[i].next)
        {
            table[i][j.first] = j.second.index;
        }
        for(auto j : all_state[i].rules)
        {
            if(j.first[j.first.size()-1]=='`')
            {
                table[i]["规约"]=j.second;

                all_state[i].terminal=true;
            }
        }
    }
}
void show_stack(vector<int> v1,vector<char_state> v2)
{

    for(auto i : v2)
    {
        cout<<i.str<<" ";
    }
    cout<<endl;
    for(auto i : v1)
    {
        cout<<i<<" ";
    }
    cout<<endl<<endl;
}
void doit(int num,vector<int> &state_s,vector<char_state> &char_s)
{
    switch(num)
    {
        case 0: //S -> V=E (generate(=,E.place,_,V.place))
        {
            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first
            int size = second.size();
            while(size--)
                state_s.pop_back();

            char_state temp1 = char_s.back();
            char_s.pop_back();
            char_s.pop_back();
            char_state temp2 = char_s.back();
            char_s.pop_back();
            generate("=",temp1.place,"_",temp2.place);
            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,""});
            show_stack(state_s,char_s);
            break;
        }
        case 1: //E -> E+T (E_left.place = new Temp; generate(+,E_right.place,T.place,E_left.place))
        {
            string tempname = "temp" + to_string(temp_num++);
            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first
            int size = second.size();
            while(size--)
                state_s.pop_back();

            char_state temp1 = char_s.back();
            char_s.pop_back();
            char_s.pop_back();
            char_state temp2 = char_s.back();
            char_s.pop_back();
            generate("+",temp2.place,temp1.place,tempname);
            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,tempname});
            show_stack(state_s,char_s);
            break;
        }
        case 2: //E ->E-T (E_left.place = new Temp; generate(-,E_right.place,T.place,E_left.place))
        {
            string tempname = "temp" + to_string(temp_num++);
            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first
            int size = second.size();
            while(size--)
                state_s.pop_back();

            char_state temp1 = char_s.back();
            char_s.pop_back();
            char_s.pop_back();
            char_state temp2 = char_s.back();
            char_s.pop_back();
            generate("-",temp2.place,temp1.place,tempname);

            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,tempname});
            show_stack(state_s,char_s);
            break;
        }
        case 3: //E->T (E.place = T.place)
        {
            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first

            state_s.pop_back();
            char_state temp = char_s.back();
            char_s.pop_back();

            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,temp.place});
            show_stack(state_s,char_s);
            break;
        }
        case 4://T->T*F (T_left.palce = new temp; generate(*,T_right.place,F.place,T_left.place))
        {
            string tempname = "temp" + to_string(temp_num++);
            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first
            int size = second.size();
            while(size--)
                state_s.pop_back();

            char_state temp1 = char_s.back();
            char_s.pop_back();
            char_s.pop_back();
            char_state temp2 = char_s.back();
            char_s.pop_back();
            generate("*",temp2.place,temp1.place,tempname);

            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,tempname});
            show_stack(state_s,char_s);
            break;
        }
        case 5: //T->T/F (T_left.palce = new temp; generate(/,T_right.place,F.place,T_left.place))
        {
            string tempname = "temp" + to_string(temp_num++);
            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first
            int size = second.size();
            while(size--)
                state_s.pop_back();

            char_state temp1 = char_s.back();
            char_s.pop_back();
            char_s.pop_back();
            char_state temp2 = char_s.back();
            char_s.pop_back();
            generate("/",temp2.place,temp1.place,tempname);
            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,tempname});
            show_stack(state_s,char_s);
            break;
        }
        case 6: //T->F (T.place = F.place)
        {
            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first

            state_s.pop_back();
            char_state temp = char_s.back();
            char_s.pop_back();

            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,temp.place});
            show_stack(state_s,char_s);
            break;
        }
        case 7: //F->(E) (F.place = i.place)
        {
            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first

            state_s.pop_back();
            state_s.pop_back();
            state_s.pop_back();
            char_s.pop_back();
            char_state temp = char_s.back();
            char_s.pop_back();
            char_s.pop_back();

            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,temp.place});
            show_stack(state_s,char_s);
            break;
        }
        case 8: //F -> i (F.place = i.place)
        {
            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first

            state_s.pop_back();
            char_state temp = char_s.back();
            char_s.pop_back();

            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,temp.place});
            show_stack(state_s,char_s);
            break;
        }
        case 9: // v -> i  (v.place = i.place)
        {

            string first = rule_and_index[num].first;
            string second = rule_and_index[num].second; //从second规约到first

            state_s.pop_back();
            char_state temp = char_s.back();
            char_s.pop_back();

            int num2 = table[state_s.back()][first];
            state_s.push_back(num2);
            char_s.push_back({first,temp.place});
            show_stack(state_s,char_s);
            break;
        }
    }
}
void generate(string op,string opnum1,string opnum2,string s)
{
    string str ="(" + op + ", " + opnum1 + ", " + opnum2 + ", " + s + ")";
    outfile<<str<<endl;
}