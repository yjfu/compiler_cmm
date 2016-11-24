//
// Created by 傅永健 on 16/10/31.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#include "SyntacticAnalyser.h"
#include "LexicalAnalyser.h"
#include <fstream>
#include <sstream>

int match_signal_and_id(string s){
    for(int i = 0;i<type_num;i++){
        if(s == type_name[i])
            return i;
    }
    return -1;
}
int print_node(node *n,int layer){
    printf("|--%15s",type_name[n->symbol.type].c_str());
    if(n->symbol.type>=SMALLEST_NONTERNMENAL)
        printf("--");
    vector<node*> vn = n->sub_node;
    int line_num=1;//这个节点展开有多长
    for(int i = 0;i<vn.size();i++){
        line_num += print_node(vn[i],layer+1);
        if(i+1<vn.size()) {
            printf("\n");
            //打印出前面的空格格式
            printf("         ");//为最前面'program--'留出9个空格
            for (int j = 0; j <= layer; j++) {
                printf("|");
                for (int k = 0; k < 19; k++)printf(" ");
            }
        }
    }
    return line_num;
}
bool SyntacticAnalyser::can_be_null(int non_term_num) {
    set<production>& sp = this->production_cmm[non_term_num];
    for(set<production>::iterator it = sp.begin();it != sp.end();it++){
        if(it->follow_list[0]==0)
            return 1;
    }
    return 0;
}
SyntacticAnalyser::SyntacticAnalyser(string file){
    fstream fin(file,ios::in);
    string line;
    while(getline(fin,line)){
        if(line.length() == 0)
            continue;
        stringstream ss(line);
        string left_s,right_s;
        int left_i,right_i;
        ss>>left_s;
        left_i = match_signal_and_id(left_s);
        production p(left_i);
        while(ss>>right_s){
            right_i = match_signal_and_id(right_s);
            p.follow_list.push_back(right_i);
        }
        if(this->production_cmm.count(left_i))
            this->production_cmm[left_i].insert(p);
        else{
            set<production> sp;
            sp.insert(p);
            this->production_cmm[left_i]=sp;
        }
        this->all_production.push_back(p);
    }
    make_first();
    make_follow();
    make_select();
    make_predict_table();
    fin.close();
    //print test
//    for(map<int,set<production>>::iterator i = this->production_cmm.begin();i!=this->production_cmm.end();i++){
//        for(set<production>::iterator j = i->second.begin();j!=i->second.end();j++){
//            printf("%d\t",j->belong);
//            for(int k = 0;k<j->follow_list.size();k++)
//                printf("%d\t",j->follow_list[k]);
//            printf("\n");
//        }
//    }
}
set<int> SyntacticAnalyser::get_first(int non_term_num){
    set<int> si;
    set<production>&sp = this->production_cmm[non_term_num];
    for(set<production>::iterator it = sp.begin();it!=sp.end();it++){
        const vector<int>&vi = it->follow_list;
        if(vi[0]<SMALLEST_NONTERNMENAL)
            si.insert(vi[0]);
        else{
            //产生式开头不是终结符,递归计算
            //实际上应该考虑右部第一个非终结符能为空的情况
            //但是文法中并没有这种情况,所以没有写
            set<int>sub_si = get_first(vi[0]);
            set<int>result;
            set_union(si.begin(),si.end(),sub_si.begin(),
                    sub_si.end(),inserter(result,result.begin()));
            si = result;
        }
    }
    return si;
}
void SyntacticAnalyser::make_first() {
    for(int i = SMALLEST_NONTERNMENAL;i<LARGEST_NONTERNMENAL;i++)
        this->first[i] = get_first(i);
    //test
//    for(int i = SMALLEST_NONTERNMENAL;i<LARGEST_NONTERNMENAL;i++) {
//        printf("%d\t",i);
//        for(set<int>::iterator it=first[i].begin();it!=first[i].end();it++)
//            printf("%d,",*it);
//        printf("\n");
//    }
}
void SyntacticAnalyser::make_follow() {
    vector<production>&vp = this->all_production;
    //第一种后跟符号集合的产生情况,a->bC,则C的开始符号集合就是b的后跟符号集合
    for(int i = 0;i<vp.size();i++){
        vector<int>&vi = vp[i].follow_list;
        for(int j = 0;j<vi.size()-1;j++){
            if(vi[j]==0)continue;
            if(vi[j+1]>=SMALLEST_NONTERNMENAL) {
                //后跟符号是一个非终结符
                set<int> si, temp;
                if (this->follow.count(vi[j]))
                    temp = follow[vi[j]];
                set_union(temp.begin(), temp.end(),
                        this->first[vi[j+1]].begin(),
                        this->first[vi[j+1]].end(),
                        inserter(si,si.begin()));
                this->follow[vi[j]]=si;
            }else {
                //后跟符号是一个终结符
                if(this->follow.count(vi[j]))
                    this->follow[vi[j]].insert(vi[j+1]);
                else{
                    set<int>si;
                    si.insert(vi[j+1]);
                    this->follow[vi[j]] = si;
                }
            }
        }
    }
    //第二种,a->cb,则a的后跟符号被b包含;或b能推出null,则b的后跟符号被c包含
    bool have_changed=1;
    while(have_changed) {
        have_changed=0;
        for (int i = 0; i < vp.size(); i++) {
            vector<int> &vi = vp[i].follow_list;
            for (int j = 0; j < vi.size(); j++) {
                if(vi[j]==0)continue;
                set<int>si,temp;
                //前一种情况
                if(j==vi.size()-1){
                    if(this->follow.count(vp[i].belong))
                        temp = this->follow[vp[i].belong];
                }//后一种情况
                else if(can_be_null(vi[j+1])){
                    if(this->follow.count(vi[j+1]))
                        temp = this->follow[vi[j+1]];
                }
                set_union(temp.begin(),temp.end(),
                        this->follow[vi[j]].begin(),
                        this->follow[vi[j]].end(),
                        inserter(si,si.begin()) );
                if(si.size()>this->follow[vi[j]].size()){
                    have_changed = 1;
                    this->follow[vi[j]]=si;
                }

            }
        }
    }
    //test
//    for(map<int,set<int>>::iterator itm = this->follow.begin();itm!=this->follow.end();itm++){
//        printf("%d\t",itm->first);
//        for(set<int>::iterator its = itm->second.begin();its!=itm->second.end();its++){
//            printf("%d ",*its);
//        }
//        printf("\n");
//    }
}
void SyntacticAnalyser::make_select() {
    for(vector<production>::iterator it=this->all_production.begin();it!=this->all_production.end();it++){
        //这里select集计算比较特殊
        //本来应该判断A->alpha中alpha是否可能为空字
        //但是文法中发现除非alpha本身是空字,否则alpha不可能推出空字
        //(除了第一个产生式,但是这可以直接当做特殊情况,因为此时相当于没有输入)
        //同时,发现产生式右部开头不会能导出空字
        //而以下两个操作也是因为这个特殊性得到了简化
        if(it->follow_list[0]==0)
            it->select=this->follow[it->belong];
        else if(it->follow_list[0]<SMALLEST_NONTERNMENAL)
            it->select.insert(it->follow_list[0]);
        else
            it->select=this->first[it->follow_list[0]];
    }
}
void SyntacticAnalyser::make_predict_table() {
    //对预测分析表的每一行(代表一个非终结符,32行)
    int base = SMALLEST_NONTERNMENAL;
    memset(this->predict_table,-1,sizeof(int)*32*32);
    for(int i = 0;i<LARGEST_NONTERNMENAL-SMALLEST_NONTERNMENAL;i++){
        vector<production> &vp=this->all_production;
        //对每一个产生式vp[j]
        for(int j = 0;j<vp.size();j++){
            if(i+base==vp[j].belong){
                for(set<int>::iterator sit=vp[j].select.begin();sit!=vp[j].select.end();sit++){
                    if(this->predict_table[i][*sit]==-1)
                        this->predict_table[i][*sit]=j;
                    else
                        printf("\nWRONG!at row %d col %d first is %d next is %d\n",
                                i,*sit,this->predict_table[i][*sit],j);
                }
            }
        }
    }
    //test
//    printf("type\t");
//    for(int j = 0;j<32;j++)
//        printf("%s\t",type_name[j].c_str());
//    printf("\n");
//    for(int i = 0;i<31;i++){
//        printf("%s\t",type_name[i+SMALLEST_NONTERNMENAL].c_str());
//        for(int j = 0;j<32;j++)
//            printf("%d\t",this->predict_table[i][j]);
//        printf("\n");
//    }
}
bool SyntacticAnalyser::analyse(LexicalAnalyser &la) {
    if(la.program.size()==0){
        printf("no program input!\n");
        return 0;
    }
    node*n=new node;
    n->symbol.type=SMALLEST_NONTERNMENAL;
    bool success = 1;
    stack<int>symbol_stack;//总控程序的符号栈
    int input_pointer=0;//指向现在已经分析到的位置
    vector<Symbol>& vs = la.program;
    symbol_stack.push(SMALLEST_NONTERNMENAL);
    while(!symbol_stack.empty()){
        int symbol_num = symbol_stack.top();
        symbol_stack.pop();
        //当程序段被分析完
        if(input_pointer==vs.size()&&symbol_stack.size()!=0){
            printf("the program is not done!\n");
            success = 0;
            break;
        }
        //如果栈顶是-1,那么说明一个非终结符对应的产生式已经被分析完了
        if(symbol_num == -1){
            n=n->father;
            if(n->father&&n->father->sub_node.size()>n->node_num+1) {
                n=n->father->sub_node[n->node_num+1];
            }
            continue;
        }
        //如果是终结符
        if(symbol_num<SMALLEST_NONTERNMENAL){
            if(symbol_num == 0) {
                if(n->father->sub_node.size()>n->node_num+1)
                    n=n->father->sub_node[n->node_num+1];
                continue;
            }
            if(vs[input_pointer].type==symbol_num) {
                n->symbol=vs[input_pointer];
                if(n->father->sub_node.size()>n->node_num+1)
                    n=n->father->sub_node[n->node_num+1];
                input_pointer++;
            }
            else{
                printf("Syntactic Error in line %d :need a \"%s\" but get a \"%s\"\n",
                        vs[input_pointer].lineNum,
                        type_name[symbol_num].c_str(),
                        type_name[vs[input_pointer].type].c_str());
                success = 0;
                break;
            }
        }//如果是非终结符
        else{
            int row = symbol_num-SMALLEST_NONTERNMENAL;
            int col = vs[input_pointer].type;
            int predict_num = this->predict_table[row][col];
            if(predict_num == -1){
                printf("Syntactic Error in line %d :symbol after %s should not be a \"%s\"\n",
                        vs[input_pointer].lineNum,
                        type_name[symbol_num].c_str(),
                        type_name[vs[input_pointer].type].c_str());
                printf("It can be a");
                for(int i = 0;i<32;i++){
                    if(predict_table[row][i]!=-1)
                        printf(" %s",type_name[i].c_str());
                }
                printf("\n");
                success = 0;
                break;
            }
            vector<int> vi = this->all_production[predict_num].follow_list;
            //构建子节点
            for(int i = 0;i<(int)vi.size();i++){
                node *sub_n=new node;
                sub_n->father=n;
                sub_n->symbol.type=vi[i];
                sub_n->node_num=i;
                n->sub_node.push_back(sub_n);
            }
            n=n->sub_node[0];
            //压入符号栈
            //插入-1,表示这里是由一个非终结符分裂出的
            symbol_stack.push(-1);
            for(int i = (int)vi.size()-1;i>-1;i--){
                symbol_stack.push(vi[i]);
            }
        }
    }
    if(symbol_stack.empty()&&success) {
        printf("Synatactic Analysis success\n");
        this->root=n;
    }
    else
        success = 0;
    return success;
}
void SyntacticAnalyser::print_tree() {
    printf("program--");
    for(int i = 0;i<root->sub_node.size();i++){
        node*n=root->sub_node[i];
        print_node(root->sub_node[i],0);
        printf("\n");
        //打印出前面的空格格式
        printf("         ");//为最前面'program--'留出9个空格
    }
}

