//
// Created by 傅永健 on 16/11/21.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#ifndef COMPILER_SEMANTICANALYSER_H
#define COMPILER_SEMANTICANALYSER_H
#include "SyntacticAnalyser.h"

struct mID{
    int type;
    string name;
    //函数属性
    vector<mID> para_list;
    int addr;
    //变量属性
    double value;
};
struct IDTable{
    IDTable(){}
    bool canbreak;
    map<string,mID> table;
    //标记的是被加入table的先后顺序
    vector<string> order;
};
struct quaternion{
    //(op,a,b,c)
    string op;
    string a;
    string b;
    string c;
};
enum IDType{
    a_function=1,
    a_real_var=2,
    a_int_var=3
};
class SemanticAnalyser {
public:
    SemanticAnalyser(SyntacticAnalyser& sa);
    IDTable id_table;
    void excute();
private:
    SyntacticAnalyser* syn;
    vector<quaternion> code;
    int code_point;//==0 at first
    void read_tree();
    void output_quaternion();
private://节点处理函数
    void process_node(node*n);
    void process_n_fun_decl_list(node*n);
    void process_n_fun_decl(node*n);
    void process_n_para_list(node*n);
    void process_n_para_dec(node*n);
    void process_n_para_list_q(node*n);

};


#endif //COMPILER_SEMANTICANALYSER_H
