#include<iostream>
#include<fstream>
#include<set>
#include<vector>
#include<map>
#include<unordered_map>
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
#define THEN 50
#define BEGIN 51
#define END 52
fstream infile,tempfile,outfile,finalfile;
map<string,int> lookup;
string S;
string point = "`";
int idx = 0;
vector<string> nter;  //非终结符
vector<string> ter;   //终结符
map<string,vector<pair<string,int>>> P; //规则
map<int,map<string,int>> table;
map<int,pair<string,string> > rule_and_index; //index -> (P -> Q)
int temp_num = 0;
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
bool isdigit(string);
struct var_info
{
    string name="null";
    int num=-1;
    string yinyong,huoyue;
    string reg="null";

};
struct QUAD     //记录四元式
{
    string op;
    var_info num1,num2,des;
    //初始化函数
    QUAD(string a,string b,string c,string d):op(a){
        if(b=="_")
        {
            b="0";
        }
        if(isdigit(b))  //是数字
        {
            num1.num=stoi(b);

        }
        else
        {
            num1.name = b;
            num1.yinyong = "N";
            if(b.substr(0,4)=="temp")
            {
                num1.huoyue = "N";
            }
            else num1.huoyue = "Y";
        }
        if(c=="_")
        {
            c="0";
        }
        if(isdigit(c))  //是数字
        {
            num2.num=stoi(c);

        }
        else
        {
            num2.name = c;
            num2.yinyong = "N";
            if(c.substr(0,4)=="temp")
            {
                num2.huoyue = "N";
            }
            else num2.huoyue = "Y";
        }
        if(d=="_")
        {
            d="0";
        }
        if(isdigit(d))  //是数字
        {
            des.num=stoi(d);

        }
        else
        {
            des.name = d;
            des.yinyong = "N";
            if(d.substr(0,4)=="temp")
            {
                des.huoyue = "N";
            }
            else des.huoyue = "Y";
        }
    }

};
struct Reg
{
    string name;
    var_info var;
    bool empty = true;
    Reg(string name):name(name){
        var.name = "nullptr";
    }
};
Reg Ax("AX"),Bx("BX");
vector<QUAD> q;
map<string,var_info> vars;
map<string,string> op;
void init_op();
void read_quad(string);
void init_qtable();
void exe_code();
vector<state> all_state;
void init();
void OUT(int,string);
void stringtopair(string,string);
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
int main() {


    string input = "/Users/savannasilver/Desktop/program/lab7/input2.txt";
    string temp = "/Users/savannasilver/Desktop/program/lab7/temp2.txt";
    string output = "/Users/savannasilver/Desktop/program/lab7/output2.txt";
    string final = "/Users/savannasilver/Desktop/program/lab7/final2.txt";

    stringtopair(input,temp);

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
    tempfile.open(temp,ios::in);
    outfile.open(output,ios::out|ios::trunc);
    vector<pair<int,string>> s= init_v();
    int s_index = 0;
    vector<int> state_s; //记录数字
    vector<char_state> char_s;  //记录字母
    state_s.push_back(0);
    char_state cs;
    cs.str = "#";
    char_s.push_back(cs);
    cout<<"-------------------------------------分析过程--------------------------------------"<<endl;
    while(1)
    {
        if(char_s.back().str=="S")  //一句话结束了
        {
            if(s_index==s.size()-1) //全部结束
            {
                break;
            }
            else
            {   if(s[s_index].first!=endofline)
                {
                    cout<<"没有以分号结束"<<endl;
                    break;
                }
                s_index++;
                state_s.clear();
                char_s.clear();
                state_s.push_back(0);
                char_state cs;
                cs.str = "#";
                char_s.push_back(cs);
            }
        }
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
    if(char_s.back().str=="S"&&s_index==s.size()-1)
        cout<<"right!";
    else { cout << "wrong!";

        return 0;
    }

    read_quad(output);
    finalfile.open(final,ios::out|ios::trunc);
    init_qtable();
    init_op();
    exe_code();
    return 0;
}
void read_quad(string ad)
{
    fstream file;
    file.open(ad,ios::in);
    string str;
    while(getline(file,str))
    {

        int index = find(str.begin(),str.end(),',')-str.begin();
        string s1 = str.substr(1,index-1);
        int index2 = find(str.begin()+index+1,str.end(),',')-str.begin();
        string s2 = str.substr(index+2,index2-index-2);
        int index3 = find(str.begin()+index2+1,str.end(),',')-str.begin();
        string s3 = str.substr(index2+2,index3-index2-2);
        string s4 = str.substr(index3+2,str.size()-index3-3);
        QUAD quad(s1,s2,s3,s4);

        if(quad.num1.name!="null")
        {   var_info a = quad.num1;
            vars[quad.num1.name] = a;
        }

        if(quad.num2.name!="null")
        {   var_info a = quad.num2;
            vars[quad.num2.name] = a;
        }

        if(quad.des.name!="null")
        {   var_info a = quad.des;
            vars[quad.des.name] = a;
        }
        q.push_back(quad);
    }
}
bool isdigit(string s)
{
    for(auto i : s)
    {
        if(!isnumber(i))
            return false;
    }
    return true;
}
void init_qtable()
{
    for(int i=q.size()-1;i>=0;--i)
    {
        string des = q[i].des.name;
        if(vars.count(des)) {
            var_info temp = vars[des];
            q[i].des = temp;
            temp.yinyong = "N";
            temp.huoyue = "N";
            vars[des] = temp;
        }
        //找到对应的名字,填入当前装然，然后参与运算的改成(位置,Y)，在结果的改成(N,N)回填
        string left_name = q[i].num1.name;
        if(vars.count(left_name)) {
            var_info temp = vars[left_name];
            q[i].num1 = temp;
            temp.yinyong = to_string(i);
            temp.huoyue = "Y";
            vars[left_name] = temp;
        }

        string right_name = q[i].num2.name;
        if(vars.count(right_name)) {
            var_info temp = vars[right_name];
            q[i].num2 = temp;
            temp.yinyong = to_string(i);
            temp.huoyue = "Y";
            vars[right_name] = temp;
        }
    }
}
void init_op()
{
    op["+"] = "ADD";
    op["-"] = "SUB";
    op["*"] = "MUL";
    op["/"] = "DIV";
    op["="] = "ADD";
}
void exe_code()
{
    for(int i=0;i<q.size();++i)
    {
        QUAD temp = q[i];
        string num1 = temp.num1.name;

        if (Ax.var.name == num1 || Bx.var.name == num1)    //寄存器中存在这个变量
        {
            if (Ax.var.name == num1) {
                string s1 = "";
                if(vars[temp.num2.name].name=="null")
                {
                    s1 += op[temp.op] + " " + Ax.name + " " + to_string(temp.num2.num);
                }
                else
                if (vars[temp.num2.name].reg != "null")
                    s1 += op[temp.op] + " " + Ax.name + " " + vars[temp.num2.name].reg;
                else
                    s1 += op[temp.op] + " " + Ax.name + " " + temp.num2.name;
                finalfile<<s1<<endl;
                cout << s1 << endl;
                vars[Ax.var.name].reg = "nullptr";
                Ax.var = temp.des;
                vars[temp.des.name].reg = Ax.name;
            } else {
                string s1 = "";
                if(vars[temp.num2.name].name=="null")
                    s1 += op[temp.op] + " " + Bx.name + " " + to_string(temp.num2.num);
                else
                if (vars[temp.num2.name].reg != "null")
                    s1 += op[temp.op] + " " + Bx.name + " " + vars[temp.num2.name].reg;
                else
                    s1 += op[temp.op] + " " + Bx.name + " " + temp.num2.name;
                finalfile<<s1<<endl;
                cout << s1 << endl;

                vars[Bx.var.name].reg = "nullptr";
                Bx.var = temp.des;
                vars[temp.des.name].reg = Bx.name;

            }
        } else {
            if (Ax.empty == true) {
                string s1 = "";
                if(num1=="null")
                    s1 += "MOV " + Ax.name + " " + to_string(temp.num1.num);
                else
                    s1 += "MOV " + Ax.name + " " + num1;
                finalfile<<s1<<endl;
                cout << s1 << endl;
                Ax.empty = false;
                Ax.var = temp.num1;
                string s2 = "";
                if(vars[temp.num2.name].name=="null")
                    s2 += op[temp.op] + " " + Ax.name + " " + to_string(temp.num2.num);
                else
                if (vars[temp.num2.name].reg != "null")
                    s2 += op[temp.op] + " " + Ax.name + " " + vars[temp.num2.name].reg;
                else
                    s2 += op[temp.op] + " " + Ax.name + " " + temp.num2.name;
                finalfile<<s2<<endl;
                cout << s2 << endl;
                Ax.var = temp.des;
                vars[temp.des.name].reg = Ax.name;
            } else if (Bx.empty == true){
                string s1 = "";
                if(num1=="null")
                    s1 += "MOV " + Bx.name + " " + to_string(temp.num1.num);
                else
                    s1 += "MOV " + Bx.name + " " + num1;
                finalfile<<s1<<endl;
                cout << s1 << endl;
                Bx.empty = false;
                Bx.var = temp.num1;
                string s2 = "";
                if(vars[temp.num2.name].name=="null")
                    s2 += op[temp.op] + " " + Bx.name + " " + to_string(temp.num2.num);
                else
                if (vars[temp.num2.name].reg != "null")
                    s2 += op[temp.op] + " " + Bx.name + " " + vars[temp.num2.name].reg;
                else
                    s2 += op[temp.op] + " " + Bx.name + " " + temp.num2.name;
                finalfile<<s2<<endl;
                cout << s2 << endl;
                Bx.var = temp.des;
                vars[temp.des.name].reg = Bx.name;
            } else {
                //比较距离
                string dis1 = Ax.var.yinyong;
                string dis2 = Bx.var.yinyong;
                if (dis1 != "N" && dis2 != "N") {
                    if (stoi(dis1) < stoi(dis2))      //Ax近，替换Bx
                    {
                        string s = "MOV ";

                        s += Bx.var.name + " " + Bx.name;
                        finalfile<<s<<endl;
                        cout << s << endl;

                        string s1 = "";
                        if(num1=="null")
                            s1 += "MOV " + Bx.name + " " + to_string(temp.num1.num);
                        else
                            s1 += "MOV " + Bx.name + " " + num1;
                        finalfile<<s1<<endl;
                        cout << s1 << endl;
                        Bx.empty = false;
                        Bx.var = temp.num1;
                        string s2 = "";
                        if(vars[temp.num2.name].name=="null")
                            s2 += op[temp.op] + " " + Bx.name + " " + to_string(temp.num2.num);
                        else
                        if (vars[temp.num2.name].reg != "null")
                            s2 += op[temp.op] + " " + Bx.name + " " + vars[temp.num2.name].reg;
                        else
                            s2 += op[temp.op] + " " + Bx.name + " " + temp.num2.name;
                        finalfile<<s2<<endl;
                        cout << s2 << endl;
                        vars[Bx.var.name].reg = "null";
                        Bx.var = temp.des;
                        vars[temp.des.name].reg = Bx.name;
                    } else {
                        string s = "MOV ";
                        s += Ax.var.name + " " + Ax.name;
                        finalfile<<s<<endl;
                        cout << s << endl;

                        string s1 = "";
                        if(num1=="null")
                            s1 += "MOV " + Ax.name + " " + to_string(temp.num1.num);
                        else
                            s1 += "MOV " + Ax.name + " " + num1;
                        finalfile<<s1<<endl;
                        cout << s1 << endl;
                        Ax.empty = false;
                        Ax.var = temp.num1;
                        string s2 = "";
                        if(vars[temp.num2.name].name=="null")
                            s2 += op[temp.op] + " " + Ax.name + " " + to_string(temp.num2.num);
                        else
                        if (vars[temp.num2.name].reg != "null")
                            s2 += op[temp.op] + " " + Ax.name + " " + vars[temp.num2.name].reg;
                        else
                            s2 += op[temp.op] + " " + Ax.name + " " + temp.num2.name;
                        finalfile<<s2<<endl;
                        cout << s2 << endl;
                        vars[Ax.var.name].reg = "null";
                        Ax.var = temp.des;
                        vars[temp.des.name].reg = Ax.name;
                    }
                } else if (dis1 == "N")  //替换Ax
                {
                    string s = "MOV ";
                    s += Ax.var.name + " " + Ax.name;
                    cout << s << endl;
                    finalfile<<s<<endl;

                    string s1 = "";
                    if(num1=="null")
                        s1 += "MOV " + Ax.name + " " + to_string(temp.num1.num);
                    else
                        s1 += "MOV " + Ax.name + " " + num1;
                    finalfile<<s1<<endl;
                    cout << s1 << endl;
                    Ax.empty = false;
                    Ax.var = temp.num1;
                    string s2 = "";
                    if(vars[temp.num2.name].name=="null")
                        s2 += op[temp.op] + " " + Ax.name + " " + to_string(temp.num2.num);
                    else
                    if (vars[temp.num2.name].reg != "null")
                        s2 += op[temp.op] + " " + Ax.name + " " + vars[temp.num2.name].reg;
                    else
                        s2 += op[temp.op] + " " + Ax.name + " " + temp.num2.name;
                    finalfile<<s2<<endl;
                    cout << s2 << endl;
                    vars[Ax.var.name].reg = "null";
                    Ax.var = temp.des;
                    vars[temp.des.name].reg = Ax.name;
                } else    //替换Bx
                {
                    string s = "MOV ";

                    s += Bx.var.name + " " + Bx.name;
                    cout << s << endl;
                    finalfile<<s<<endl;
                    string s1 = "";
                    if(num1=="null")
                        s1 += "MOV " + Bx.name + " " + to_string(temp.num1.num);
                    else
                        s1 += "MOV " + Bx.name + " " + num1;
                    finalfile<<s1<<endl;
                    cout << s1 << endl;
                    Bx.empty = false;
                    Bx.var = temp.num1;
                    string s2 = "";
                    if(vars[temp.num2.name].name=="null")
                        s2 += op[temp.op] + " " + Bx.name + " " + to_string(temp.num2.num);
                    else
                    if (vars[temp.num2.name].reg != "null")
                        s2 += op[temp.op] + " " + Bx.name + " " + vars[temp.num2.name].reg;
                    else
                        s2 += op[temp.op] + " " + Bx.name + " " + temp.num2.name;
                    finalfile<<s2<<endl;
                    cout << s2 << endl;
                    vars[Bx.var.name].reg = "null";
                    Bx.var = temp.des;
                    vars[temp.des.name].reg = Bx.name;
                }
            }
        }
        if (Ax.var.huoyue == "N" && Ax.var.yinyong == "N") {

            Ax.empty = true;
            var_info v;
            Ax.var = v;
        }
        if (Bx.var.huoyue == "N" && Bx.var.yinyong == "N") {

            Bx.empty = true;
            var_info v;
            Bx.var = v;
        }

    }
    if(Ax.empty==false)
    {
        string s = "MOV ";
        s += Ax.var.name + " "+ Ax.name;
        finalfile<<s<<endl;
        cout<<s<<endl;
        Ax.empty = true;
        var_info v;
        Ax.var = v;
    }
    if(Bx.empty==false)
    {
        string s = "MOV ";
        s += Bx.var.name + " "+ Bx.name;
        finalfile<<s<<endl;
        cout<<s<<endl;

        Bx.empty = true;
        var_info v;
        Bx.var = v;
    }

}
void lab5_init()
{
    fstream in;
    in.open("/Users/savannasilver/Desktop/program/lab7/init.txt",ios::in);
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
    while(getline(tempfile,temp))
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
    lookup["then"]=50;
    lookup["begin"]=51;
    lookup["end"]=52;
}
void OUT(int c,string val="null")
{   if(c==ID||c==NUM) {
        string str = "(" + to_string(c) + "," + val + ")";
        tempfile << str<<endl;
    } else{
        string str = "(" + to_string(c) + "," + val + ")";
        tempfile<< str<<endl;
    }
}
void stringtopair(string infile_,string outfile_)
{
    infile.open(infile_, ios::in);
    tempfile.open(outfile_,ios::out|ios::trunc);

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
    tempfile.close();
    infile.close();
    cout<<"complete"<<endl;
}
