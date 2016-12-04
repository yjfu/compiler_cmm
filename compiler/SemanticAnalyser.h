//
// Created by 傅永健 on 16/11/21.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#ifndef COMPILER_SEMANTICANALYSER_H
#define COMPILER_SEMANTICANALYSER_H
#include "SyntacticAnalyser.h"
#include "WrongProcesser.h"
struct mID{
    int type;
    string name;
    //函数属性
    vector<mID> para_list;
    int return_type;
    //变量属性
    double value;
    //共有属性
    //函数表示代码从何处开始
    //变量表示在所在函数的数据段中的位置
    int addr;
};
struct IDTable{
    IDTable(){number_of_id=0;}
    map<string,mID> table;
    //标记的是被加入table的先后顺序
    vector<string> order;
    int number_of_id;
};
struct q_contain{
    //标志是address还是值
    bool is_addr=0;
    //标志是否是函数
    bool is_func=0;
    bool is_real=0;
    bool is_int=0;
    string func_name;
    int addr;
    double value;
};
struct quaternion{
    //(op,a,b,c)
    string op;
    q_contain a;
    q_contain b;
    q_contain c;
};

enum IDType{
    a_function=1,
    a_real_var=2,
    a_int_var=3,
    a_int_list=4,
    a_real_list=5
};
struct Obj{
    bool is_real=0;
    bool is_int=0;
    int addr;
    double value;
    string id;
};
class SemanticAnalyser {
public:
    SemanticAnalyser(SyntacticAnalyser& sa);
    vector<IDTable> id_table;
    void excute();
private:
    WrongProcesser *wp;
    SyntacticAnalyser* syn;
    vector<quaternion> code;
    int code_point;//==0 at first
    void read_tree();
    void output_quaternion();
private://节点处理函数
    void process_node(node*n);
    void process_program(node*n);
    void process_n_fun_decl_list(node*n);
    void process_n_fun_decl(node*n);
    void process_n_para_list(node*n);
    void process_n_para_dec(node*n);
    void process_n_para_list_q(node*n);
    void process_n_stmt_list(node*n);
    void process_n_exp_stmt(node*n);
    void process_n_call_stmt(node*n);
    void process_n_ret_stmt(node*n);
    void process_n_decl_stmt(node*n);
    void process_n_read_stmt(node*n);
    void process_n_write_stmt(node*n);
    void process_n_if_stmt(node*n);
    void process_n_while_stmt(node*n);
    void process_n_real_plist(node*n,node*fun_id);
    void process_n_real_plist_q(node*n,node*fun_id,int para_num);
    int id_type(string id);
    Obj process_a_param(node*n);
    int have_id(string id);
    Obj process_n_obj(node*n);
};


#endif //COMPILER_SEMANTICANALYSER_H
