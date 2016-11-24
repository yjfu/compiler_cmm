//
// Created by 傅永健 on 16/11/21.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#include "SemanticAnalyser.h"

void SemanticAnalyser::process_node(node* n){
    switch(n->symbol.type){
        case n_fun_decl_list:{
            this->process_n_fun_decl_list(n);
            break;
        }
        case n_fun_decl:{
            this->process_n_fun_decl(n);
            break;
        }
        case n_para_list:{
            this->process_n_para_list(n);
            break;
        }
        case n_para_dec:{
            this->process_n_para_dec(n);
            break;
        }
        case n_para_list_q:{
            this->process_n_para_list_q(n);
            break;
        }
        case n_stmt_list:{

            break;
        }
    }
}
void SemanticAnalyser::process_n_fun_decl_list(node *n) {
    vector<node*>& subn=(n->sub_node);
    for(int i = 0;i<subn.size();i++){
        process_node(subn[i]);
    }
    return;
}
void SemanticAnalyser::process_n_fun_decl(node *n) {
    int type=n->sub_node[0]->sub_node[0]->symbol.type;//确定函数返回类型
    string id=n->sub_node[1]->symbol.contain;//确定函数的名字
    //将函数名加入最顶层符号表this->id_table中去
    if(this->id_table.table.count(id)){
        //已经声明过这个函数
        printf("该函数 %s 已被声明!\n",id.c_str());
        return;
    }
    else{
        this->id_table.table[id].name=n->sub_node[1]->symbol.contain;
        this->id_table.table[id].addr=this->code_point;
        this->id_table.table[id].type=a_function;
        this->id_table.order.push_back(id);
    }
    //分析参数
    process_node(n->sub_node[3]);
    //分析语句
    process_node(n->sub_node[6]);
}
void SemanticAnalyser::process_n_para_list(node *n) {
    if(n->sub_node[0]->symbol.type==s_null){
        //如果参数列表为空
        return;
    }
    //处理n_para_dec
    process_node(n->sub_node[0]);
    //处理n_para_list_q
    process_node(n->sub_node[1]);
}
void SemanticAnalyser::process_n_para_dec(node *n) {
    mID id;
    //查看参数类型
    if(n->sub_node[0]->sub_node[0]->symbol.type==s_int)
        id.type=a_int_var;
    else
        id.type=a_real_var;
    id.name=n->sub_node[1]->symbol.contain;
    id.value=0;
    //所在函数名
    string func_name=this->id_table.order[this->id_table.order.size()-1];
    //判断是否所在函数中是否有同名参数
    bool have_same=0;
    for(int i = 0;i<this->id_table.table[func_name].para_list.size();i++){
        if(id.name==this->id_table.table[func_name].para_list[i]) {
            have_same = 1;
            break;
        }
    }
    if(have_same)return;
    //如果没有,加入到函数的参数表里面去
    this->id_table.table[func_name].para_list.push_back(id);
}
void SemanticAnalyser::process_n_para_list_q(node *n) {
    if(n->sub_node[0]->symbol.type==s_null){
        //如果参数列表为空
        return;
    }
    //处理n_para_dec,这里sub_node[0]是逗号
    process_node(n->sub_node[1]);
    //处理n_para_list_q
    process_node(n->sub_node[2]);
}
SemanticAnalyser::SemanticAnalyser(SyntacticAnalyser &sa) {
    this->syn=&sa;
    this->code_point=0;
}
void SemanticAnalyser::read_tree() {
    node* node = this->syn->root;
    for(int i = 0;i<node->sub_node.size();i++){
        process_node(node);
    }
}