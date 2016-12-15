//
// Created by 傅永健 on 16/11/21.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#include "SemanticAnalyser.h"
#include "NodeProcessor.h"

void SemanticAnalyser::process_node(node* n){
    bool success=1;
    switch(n->symbol.type){
        case n_program:{
            this->process_program(n);
            break;
        }
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
            this->process_n_stmt_list(n);
            break;
        }
        case n_decl_stmt:{
            this->process_n_decl_stmt(n);
            break;
        }
        case n_call_stmt:{
            this->process_n_call_stmt(n);
            break;
        }
        case n_read_stmt:{
            this->process_n_read_stmt(n);
            break;
        }
        case n_write_stmt:{
            this->process_n_write_stmt(n);
            break;
        }
        case n_ret_stmt:{
            this->process_n_ret_stmt(n);
            break;
        }
        case n_if_stmt:{
            this->process_n_if_stmt(n);
            break;
        }
        case n_while_stmt:{
            this->process_n_while_stmt(n);
            break;
        }
        case n_exp_stmt:{
            this->process_n_exp_stmt(n);
            break;
        }
    }
}
void SemanticAnalyser::process_program(node *n) {
    this->process_node(n->sub_node[0]);
}
void SemanticAnalyser::process_n_fun_decl_list(node *n) {
    vector<node*>& subn=(n->sub_node);
    for(int i = 0;i<subn.size();i++){
        //分析n_fun_decl
        process_node(subn[i]);
    }
    return;
}
void SemanticAnalyser::process_n_fun_decl(node *n) {
    int type=n->sub_node[0]->sub_node[0]->symbol.type;//确定函数返回类型
    string id=n->sub_node[1]->symbol.contain;//确定函数的名字
    //将函数名加入最顶层符号表this->id_table中去
    if(this->id_table[0].table.count(id)){
        //已经声明过这个函数
        //printf("该函数 %s 已被声明!\n",id.c_str());
        this->wp->process(decleared_function,n);
        return;
    }

    this->id_table[0].table[id].name=n->sub_node[1]->symbol.contain;
    this->id_table[0].table[id].addr=this->code_point;
    if(this->id_table[0].table[id].name=="main")
        this->start_point=this->code_point;//加入起点
    this->id_table[0].table[id].type=a_function;
    this->id_table[0].table[id].return_type=type;
    this->id_table[0].order.push_back(id);
    //创建函数的符号表
    this->id_table.push_back(IDTable());
    //分析参数n_para_list
    process_node(n->sub_node[3]);
    //分析语句n_stmt_list
    process_node(n->sub_node[6]);
    //可以在这里分析中间代码,看看能否return
    if(!must_can_return(this->id_table[0].table[id].addr)){
        //不一定可以返回,报错
        this->wp->process(function_may_cannot_return,n->sub_node[1]);
        //继续分析
    }
    //分析完此函数,此函数符号表出栈
    this->id_table.pop_back();

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
    string func_name=this->id_table[0].order[this->id_table[0].order.size()-1];
    //判断是否所在函数中是否有同名参数
    bool have_same=0;
    for(int i = 0;i<this->id_table[0].table[func_name].para_list.size();i++){
        if(id.name==this->id_table[0].table[func_name].para_list[i].name) {
            have_same = 1;
            break;
        }
    }
    if(have_same){
        this->wp->process(have_same_para,n);
        return;
    }
    //如果没有,加入到函数的参数表里面去
    this->id_table[0].table[func_name].para_list.push_back(id);
    //将参数加入到函数的符号表中
    this->id_table[1].table[id.name].addr=id_table[1].number_of_id;
    id_table[1].number_of_id++;
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
void SemanticAnalyser::process_n_stmt_list(node *n) {
    if(n->sub_node[0]->symbol.type==s_null){
        //如果语句列表是空
        return;
    }
    //处理n_stmt的唯一的子节点,也就是n_decl_stmt,n_call_stmt等等
    process_node(n->sub_node[0]->sub_node[0]);
    //处理n_stmt_list
    process_node(n->sub_node[1]);
}
//done
void SemanticAnalyser::process_n_exp_stmt(node*n){
    //这里使用处理n_obj节点的函数处理n_exp_stmt节点
    //因为n_exp_stmt的前两个子节点正好就是一个n_obj节点
    Obj obj=process_n_obj(n);
    if(obj.addr==-1)return;//前面就有错,返回
    int pos = (int)this->id_table.size()-1;
    int now_addr=this->id_table[pos].number_of_id;
    //先建立最终的赋值四元式
    quaternion q_assign;
    q_assign.op="=";
    //q_assign.a.addr=obj.addr;
    q_assign.a.obj=obj;

    Obj ans = process_n_e(n->sub_node[3]);
    if(ans.addr==-1)return;//处理表达式时出错,返回
    q_assign.b.obj=ans;
    //q_assign.b.addr=ans.addr;
    //最后将q_assign加入code
    this->code.push_back(q_assign);
    this->code_point++;
    this->id_table[pos].number_of_id=now_addr;//释放之前做这些操作用的内存
}
//done
Obj SemanticAnalyser::process_n_e(node *n) {
    //处理n_t
    Obj nt=this->process_n_t(n->sub_node[0]);
    if(nt.addr==-1)return nt;//之前出错,返回一个地址为-1的obj

    node*now_neq=n->sub_node[1];//当前处理的neq
    int now_addr=nt.addr;//当前的加数的地址
    bool is_int=nt.ia,is_real=nt.ra;
    while(now_neq->sub_node[0]->symbol.type!=s_null){
        //neq还没有被递归的处理完,就是后面还有加数
        Obj next_nt=process_n_t(now_neq->sub_node[1]);//处理下一个加数
        if(next_nt.addr==-1)return nt;//之前出错,返回一个地址为-1的obj
        //生成一个新的四元式,把这两个加数加起来
        quaternion q;
        if(now_neq->sub_node[0]->symbol.type==s_plus)q.op="+";
        else q.op="-";
//        q.a.is_addr=1;
//        q.a.addr=now_addr;
//        q.a.ia=is_int;
//        q.a.ra=is_real;
//
//        q.b.is_addr=1;
//        q.b.addr=next_nt.addr;
//        q.b.ia=next_nt.ia;
//        q.b.ra=next_nt.ra;
        q.a.obj=nt;
        q.b.obj=next_nt;

        int pos=(int)this->id_table.size()-1;
        //为加的和申请一块内存
        //q.c.is_addr=1;
        q.c.obj.addr=this->id_table[pos].number_of_id;
        this->id_table[pos].number_of_id++;
        //将语句加入code
        q.c.obj.ra=q.a.obj.ra||q.b.obj.ra;
        q.c.obj.ia=!q.c.obj.ra;
//        is_int=q.c.ia;
//        is_real=q.c.ra;
        this->code.push_back(q);
        this->code_point++;
        //更新当前加数信息
        //now_addr=q.c.addr;
        nt=q.c.obj;
        now_neq=now_neq->sub_node[2];
    }
    Obj ret;
    //ret.addr=now_addr;
    ret=nt;
    return ret;
}
//done
Obj SemanticAnalyser::process_n_t(node *n) {
    //处理n_f
    Obj nf=this->process_n_f(n->sub_node[0]);
    if(nf.addr==-1)return nf;//之前出错,返回一个地址为-1的obj

    node*now_ntq=n->sub_node[1];//当前处理的ntq
    int now_addr=nf.addr;//当前的乘数的地址
    bool is_int=nf.ia,is_real=nf.ra;
    while(now_ntq->sub_node[0]->symbol.type!=s_null){
        //ntq还没有被递归的处理完,就是后面还有乘数
        Obj next_nf=process_n_f(now_ntq->sub_node[1]);//处理下一个乘数
        if(next_nf.addr==-1)return nf;//之前出错,返回一个地址为-1的obj
        //生成一个新的四元式,把这两个乘数乘起来
        quaternion q;
        if(now_ntq->sub_node[0]->symbol.type==s_mult)q.op="*";
        else q.op="/";
        q.a.obj=nf;
//        q.a.is_addr=1;
//        q.a.addr=now_addr;
//        q.a.ia=is_int;
//        q.a.ra=is_real;
        q.b.obj=next_nf;
//        q.b.is_addr=1;
//        q.b.addr=next_nf.addr;
//        q.b.ia=next_nf.ia;
//        q.b.ra=next_nf.ra;
        int pos=(int)this->id_table.size()-1;
        //为乘积申请一块内存
        //q.c.addr=this->id_table[pos].number_of_id;
        q.c.obj.addr=this->id_table[pos].number_of_id;
        this->id_table[pos].number_of_id++;

        //q.c.is_addr=1;
        q.c.obj.ra=q.a.obj.ra||q.b.obj.ra;
        q.c.obj.ia=!q.c.obj.ra;

        //将语句加入code
        this->code.push_back(q);
        this->code_point++;
        //更新当前乘数信息

//        now_addr=q.c.addr;
//        is_int=q.c.ia;
//        is_real=q.c.ra;
        nf=q.c.obj;
        now_ntq=now_ntq->sub_node[2];
    }
    Obj ret;
    //ret.addr=now_addr;
    ret=nf;
    return ret;
}
//done
Obj SemanticAnalyser::process_n_f(node *n) {
    Obj ret;
    if(n->sub_node[0]->symbol.type==s_lsp){
        //使用产生式n_f->(n_e)
        Obj ne=this->process_n_e(n->sub_node[1]);
        if(ne.addr==-1)return ne;//出错返回
//        ret.addr=ne.addr;
//        ret.ia=ne.ia;
//        ret.ra=ne.ra;
        ret=ne;
    }
    else{
        //使用产生式n_f->n_obj
        Obj obj=this->process_n_obj(n->sub_node[0]);
        if(obj.addr==-1)return obj;//出错返回
        if(obj.is_int||obj.is_real){
            //obj是一个值而不是地址
            //为这个值单独开辟一个地址
            int pos = (int)this->id_table.size()-1;
            quaternion make_value;
            make_value.op="=";
            make_value.a.obj.addr=this->id_table[pos].number_of_id;
            make_value.a.obj.ia=obj.is_int;
            make_value.a.obj.ra=obj.is_real;

            this->id_table[pos].number_of_id++;
            make_value.b.obj.is_int=obj.is_int;
            make_value.b.obj.is_real=obj.is_real;
            make_value.b.obj.value=obj.value;
            this->code.push_back(make_value);
            this->code_point++;
            //ret.addr=make_value.a.obj.addr;
            ret=make_value.a.obj;
        }
        else{
            //就是一个地址
            ret=obj;
//            ret.addr=obj.addr;
//            ret.ra=obj.ra;
//            ret.ia=obj.ia;
        }
    }
    return ret;
}
//done
void SemanticAnalyser::process_n_call_stmt(node*n){
    //转化为四元式(call,function,_,paramater_num)
    //检查是否有这个函数
    string func_name = n->sub_node[1]->symbol.contain;
    if(this->id_table[0].table.count(func_name)==0){
        this->wp->process(not_decleared_func_or_var,n);
        return;
    }
    //该语句所在作用域的符号表
    int pos = (int)this->id_table.size()-1;
    //为函数返回值预留一个位置
    this->id_table[pos].number_of_id++;
    quaternion q;
    q.op="call";
    //函数名
    q.a.is_func=1;
    q.a.func_name=func_name;
    q.b.obj.addr=this->id_table[pos].number_of_id-1;
    //参数个数
    node* nt=n->sub_node[3];
    int count_para=0;
    if(nt->sub_node.size()==2){
        nt=nt->sub_node[1];
        count_para++;
        while(nt->sub_node.size()==3) {
            count_para++;
            nt=nt->sub_node[2];
        }
    }
    q.c.obj.value=count_para;
    //判断参数个数是否匹配
    if(this->id_table[0].table[func_name].para_list.size()!=count_para){
        this->wp->process(too_many_or_to_less_function_para,n->sub_node[1]);
        return;
    }
    //分析参数,加入参数的四元式
    process_n_real_plist(n->sub_node[3],n->sub_node[1]);
    //加入call四元式
    this->code.push_back(q);
    this->code_point++;
    //退回参数占用的空间
    this->id_table[pos].number_of_id-=(int)q.c.obj.value;
}
//done
void SemanticAnalyser::process_n_real_plist(node *n,node*fun_id) {
    //是空字,返回
    if(n->sub_node[0]->symbol.type==s_null)
        return;

    //为这个参数分配空间
    int pos = (int)this->id_table.size()-1;
    this->id_table[pos].number_of_id++;

    Obj obj=process_n_obj(n->sub_node[0]);

    if(obj.addr==-1)return;//前面出了错,直接返回
    //检查是否有类型不匹配的错误
    //obj是一个值而不是变量
    int type=this->id_table[0].table[fun_id->symbol.contain].para_list[0].type;
    if((obj.is_int||obj.is_real)&&!(obj.is_int&&type==a_int_var)&& !(obj.is_real&&type==a_real_var)){
        this->wp->process(type_is_not_correct,fun_id);
        return;
    }
    //obj是一个变量
    bool is_real=this->id_type(obj.id)==a_real_var||this->id_type(obj.id)==a_real_list;
    bool is_int=this->id_type(obj.id)==a_int_var||this->id_type(obj.id)==a_int_list;
    if((!obj.is_int&&!obj.is_real)&&((is_real&&type==a_int_var)||
            (is_int&&type==a_real_var))){
        this->wp->process(type_is_not_correct,fun_id);
        return;
    }

    //产生四元式
    quaternion q;
    q.op="=";

    q.a.obj.addr=this->id_table[pos].number_of_id-1;;
//    q.a.is_addr=1;
//    q.a.addr=this->id_table[pos].number_of_id-1;
    q.b.obj=obj;
//    if((obj.is_int||obj.is_real)){
//        q.b.value=obj.value;
//    }
//    else{
//        q.b.is_real=obj.is_real;
//        q.b.is_int=obj.is_int;
//        q.b.addr=obj.addr;
//    }

    //将四元式加入code
    this->code.push_back(q);
    this->code_point++;
    //分析子节点(n_real_plist_q):
    process_n_real_plist_q(n->sub_node[n->sub_node.size()-1],fun_id,1);
}
//done
void SemanticAnalyser::process_n_real_plist_q(node*n,node*fun_id,int para_num){
    //是空字,返回
    if(n->sub_node[0]->symbol.type==s_null)
        return;

    //为这个参数分配空间
    int pos = (int)this->id_table.size()-1;
    this->id_table[pos].number_of_id++;

    Obj obj=process_n_obj(n->sub_node[1]);

    if(obj.addr==-1)return;//前面出了错,直接返回
    //检查是否有类型不匹配的错误
    //obj是一个值而不是变量
    int type=this->id_table[0].table[fun_id->symbol.contain].para_list[para_num].type;
    if((obj.is_int||obj.is_real)&&!(obj.is_int&&type==a_int_var)&& !(obj.is_real&&type==a_real_var)){
        this->wp->process(type_is_not_correct,fun_id);
        return;
    }
    //obj是一个变量
    bool is_real=this->id_type(obj.id)==a_real_var||this->id_type(obj.id)==a_real_list;
    bool is_int=this->id_type(obj.id)==a_int_var||this->id_type(obj.id)==a_int_list;
    if((!obj.is_int&&!obj.is_real)&&((is_real&&type==a_int_var)||
            (is_int&&type==a_real_var))){
        this->wp->process(type_is_not_correct,fun_id);
        return;
    }

    //产生四元式
    quaternion q;
    q.op="=";
    //q.a.is_addr=1;
    q.a.obj.addr=this->id_table[pos].number_of_id-1;
//    if((obj.is_int||obj.is_real)){
//        q.b.value=obj.value;
//    }
//    else{
//        q.b.is_real=obj.is_real;
//        q.b.is_int=obj.is_int;
//        q.b.addr=obj.addr;
//    }
    q.b.obj=obj;
    //将四元式加入code
    this->code.push_back(q);
    this->code_point++;
    //分析子节点(n_real_plist_q):
    process_n_real_plist_q(n->sub_node[n->sub_node.size()-1],fun_id,para_num+1);
}
//usless by now
Obj SemanticAnalyser::process_a_param(node *n){
    Symbol s = n->symbol;
    int type = s.type;
    Obj obj;
    //id是整数
    if(type==s_inum){
        obj.is_int=1;
        obj.value=s.value;
    }
    else if(type==s_rnum){
        obj.is_real=1;
        obj.value=s.value;
    }
    else {
        //是变量或者数组中的值
        //如果这个id是一个函数,显然是错的
        if(id_type(s.contain)==a_function){
            this->wp->process(use_function_like_a_value_or_list,n);
            obj.addr=-1;
            return obj;
        }
        int addr;
        obj.id=s.contain;
        if((addr=have_id(s.contain))==-1){
            this->wp->process(not_decleared_func_or_var,n);
            obj.addr=-1;
            return obj;
        }
        if(n->sub_node[1]->sub_node[0]->symbol.type==s_null){
            //以变量的形式使用数组,这里要报错
            if(id_type(s.contain)==a_real_list||id_type(s.contain)==a_int_list){
                this->wp->process(use_a_list_as_value,n);
                obj.addr=-1;
                return obj;
            }
            //确实是变量不是数组
            obj.addr=addr;
        }
        else{
            //以数组的形式使用变量,这里要报错
            if(id_type(s.contain)==a_real_var||id_type(s.contain)==a_int_var){
                this->wp->process(use_a_value_as_list,n);
                obj.addr=-1;
                return obj;
            }
            Obj o = process_n_obj(n->sub_node[1]->sub_node[1]);
            if(o.addr==-1)return o;
            if(o.is_real){
                //[a]中的a是个实数,显然出错
                this->wp->process(list_dice_should_not_be_double,n);
                obj.addr=-1;
                return obj;
            }
            else if(o.is_int){
                //是个整数,直接使用
                obj.addr=addr+(int)o.value;
            }
            else {
                //是个地址
                //这个地址对应的id不是整形的id
                if(id_type(o.id)!=a_int_var&&id_type(o.id)!=a_int_list){
                    //先查看这个地址是不是来源于一个函数,如果是则检查其返回值
                    bool valid=0;
                    if(id_type(o.id)==a_function){
                        int return_type=this->id_table[0].table[o.id].return_type;
                        if(return_type==s_int)valid=1;//返回值与想要的类型相符
                    }
                    if(!valid){
                        this->wp->process(wrong_var_type,n);
                        obj.addr=-1;
                        return obj;
                    }
                }
                obj.addr=o.addr;
            }
        }
    }
    return obj;
}
//done
Obj SemanticAnalyser::process_n_obj(node *n) {
    Symbol s = n->sub_node[0]->symbol;
    int type = s.type;
    Obj obj;
    //id是整数
    if(type==s_inum){
        obj.is_int=1;
        obj.value=s.value;
    }
    else if(type==s_rnum){
        obj.is_real=1;
        obj.value=s.value;
    }
    else if(type==s_id){
        //是变量或者数组中的值
        int addr;
        obj.id=s.contain;
        if((addr=have_id(s.contain))==-1){
            this->wp->process(not_decleared_func_or_var,n);
            obj.addr=-1;
            return obj;
        }
        //如果这个id是一个函数,显然是错的
        if(id_type(s.contain)==a_function){
            this->wp->process(use_function_like_a_value_or_list,n);
            obj.addr=-1;
            return obj;
        }
        obj.id=s.contain;
        if(n->sub_node[1]->sub_node[0]->symbol.type==s_null){
            //以变量的形式使用数组,这里要报错
            if(id_type(s.contain)==a_real_list||id_type(s.contain)==a_int_list){
                this->wp->process(use_a_list_as_value,n);
                obj.addr=-1;
                return obj;
            }
            //确实是变量不是数组
            obj.addr=addr;
            if(id_type(s.contain)==a_real_var)obj.ra=1;
            else obj.ia=1;
        }
        else{
            //以数组的形式使用变量,这里要报错
            if(id_type(s.contain)==a_real_var||id_type(s.contain)==a_int_var){
                this->wp->process(use_a_value_as_list,n);
                obj.addr=-1;
                return obj;
            }
            Obj o = process_n_obj(n->sub_node[1]->sub_node[1]);
            if(o.is_real){
                //[a]中的a是个实数,显然出错
                this->wp->process(list_dice_should_not_be_double,n);
                obj.addr=-1;
                return obj;
            }
            else if(o.is_int){
                //是个整数,直接使用
                obj.addr=addr+(int)o.value;
            }
            else {
                //是个地址
                //这个地址对应的id不是整型的id
                if(id_type(o.id)!=a_int_var&&id_type(o.id)!=a_int_list){
                    //先查看这个地址是不是来源于一个函数,如果是则检查其返回值
                    bool valid=0;
                    if(id_type(o.id)==a_function){
                        int return_type=this->id_table[0].table[o.id].return_type;
                        if(return_type==s_int)valid=1;//返回值与想要的类型相符

                    }
                    if(!valid){
                        this->wp->process(wrong_var_type,n);
                        obj.addr=-1;
                        return obj;
                    }
                }
                obj.addr=addr;
                obj.move_obj=new Obj;
                *obj.move_obj=o;
                obj.have_move=1;
            }
            if(id_type(s.contain)==a_real_list)obj.ra=1;
            else obj.ia=1;
        }
    }
    else{
        //是一个函数调用
        process_node(n->sub_node[0]);
        int pos = (int)this->id_table.size()-1;
        obj.addr=this->id_table[pos].number_of_id-1;
        obj.id=n->sub_node[0]->sub_node[1]->symbol.contain;

        int return_type=this->id_table[0].table[obj.id].return_type;
        if(return_type==s_int)obj.ia=1;
        else obj.ra=1;
    }
    return obj;
}
//如果存在,返回地址;否则返回-1
int SemanticAnalyser::have_id(string id) {
    int last_table=(int)this->id_table.size()-1;
    for(int i = last_table;i>=0;i--){
        if(this->id_table[i].table.count(id)){
            return this->id_table[i].table[id].addr;
        }
    }
    return -1;
}
//返回id的类型,
int SemanticAnalyser::id_type(string id){
    int last_table=(int)this->id_table.size()-1;
    for(int i = last_table;i>=0;i--){
        if(this->id_table[i].table.count(id)){
            return this->id_table[i].table[id].type;
        }
    }
    return -1;
}
//done
void SemanticAnalyser::process_n_ret_stmt(node*n){
    string id_name;
    int type=0;//这里的type是IDType枚举类型,但是只用来区分是int还是real,不用来区分是id,函数,还是常数
    int addr=-1;//如果是常数,那么addr==-1
    Obj obj=this->process_n_obj(n->sub_node[1]);
    if(obj.addr==-1)return;//分析obj时已经出错,不必再继续了
    if(obj.is_real){type=a_real_var;}
    else if(obj.is_int){type=a_int_var;}
    else{
        //是个变量,数组,或者函数
        if(id_type(obj.id)==a_function){
            //先查看这个地址是不是来源于一个函数,如果是则检查其返回值
            if(id_type(obj.id)==a_function){
                int return_type=this->id_table[0].table[obj.id].return_type;
                if(return_type==s_int){type=a_int_var;}
                else {type=a_real_var;}
            }
            else if(id_type(obj.id)==a_real_var||id_type(obj.id)==a_real_list){
                type=a_real_var;
            }
            else if(id_type(obj.id)==a_int_var||id_type(obj.id)==a_int_list){
                type=a_int_var;
            }
        }
        addr=obj.addr;
    }
    //检查返回类型是不是正确的
    vector<string>&all_func_name=this->id_table[0].order;
    string func_name=all_func_name[all_func_name.size()-1];
    int func_return_type=this->id_table[0].table[func_name].return_type;
    if((func_return_type==s_int&&type==a_real_var)||
            (func_return_type==s_real&&type==a_int_var)){
        //类型不匹配
        this->wp->process(wrong_return_value,n);
        return;
    }
    //生成四元式
    quaternion q;
    q.op="return";
//    if(addr==-1){
//        //返回值是个常数
//        if(obj.is_int)q.a.is_int=1;
//        else q.a.is_real=1;
//        q.a.value=obj.value;
//    }
//    else{
//        q.a.is_addr=1;
//        q.a.addr=addr;
//    }
    q.a.obj=obj;
    this->code.push_back(q);
    this->code_point++;
}
//done
void SemanticAnalyser::process_n_decl_stmt(node*n){
    mID id;
    id.name = n->sub_node[1]->symbol.contain;
    if(n->sub_node[0]->sub_node[0]->symbol.type==s_int)
        id.type = a_int_var;
    else
        id.type = a_real_var;
    //获取所在的符号表是属于哪一个等级的
    int pos = (int)this->id_table.size()-1;
    if(have_id(id.name)!=-1){
        //如果此作用域内已有该变量名
        wp->process(decleared_var,n);
        return;
    }
    this->id_table[pos].table[id.name].addr=this->id_table[pos].number_of_id;
    this->id_table[pos].number_of_id++;
    //看第三个子节点dfollow是否为空字,是则为变量,不是则为数组
    if(n->sub_node[2]->sub_node[0]->symbol.type!=s_null){
        //是数组
        node*follow=n->sub_node[2];
        int length = (int)follow->sub_node[1]->symbol.value;
        this->id_table[pos].number_of_id+=(length-1);
        if(id.type==a_real_var)id.type=a_real_list;
        else id.type=a_int_list;
        this->id_table[pos].table[id.name].length=length;
    }
    this->id_table[pos].table[id.name].type=id.type;
    this->id_table[pos].table[id.name].name=id.name;
}
//done
void SemanticAnalyser::process_n_read_stmt(node*n){
//    string id=n->sub_node[1]->symbol.contain;
//    int addr;
//    //看看要读取的id是不是声明过
//    if((addr=have_id(id))==-1){
//        this->wp->process(not_decleared_func_or_var,n);
//        return;
//    }
//    //要读取的对象是不是函数或者数组
//    if(id_type(id)!=a_real_var&&id_type(id)!=a_int_var){
//        this->wp->process(cannot_read_or_write_a_list_or_function,n);
//        return;
//    }
    int type=n->sub_node[1]->sub_node[1]->symbol.type;
    if(type==s_call||type==s_inum||type==s_rnum){
        //不可修改的左值,read一个函数或常数
        this->wp->process(cannot_be_reform,n);
        return;
    }

    Obj o = process_n_obj(n->sub_node[1]);
    if(o.addr==-1)return;
    quaternion q;
    q.op="read";
    q.a.obj=o;
    this->code.push_back(q);
    this->code_point++;
}
//done
void SemanticAnalyser::process_n_write_stmt(node*n){
//    string id=n->sub_node[1]->symbol.contain;
//    int addr;
//    int add_addr=0;
//    //看看要读取的id是不是声明过
//    if((addr=have_id(id))==-1){
//        this->wp->process(not_decleared_func_or_var,n);
//        return;
//    }
//    //要写入的对象是不是函数或者数组
//    if(id_type(id)!=a_real_var&&id_type(id)!=a_int_var){
//        if(id_type(id)==a_real_list||id_type(id)==a_int_list){
//            if(n->sub_node[2]->sub_node[0]->symbol.type!=s_null){
//                Obj o=process_n_obj(n->sub_node[2]->sub_node[1]);
//                if(o.addr==-1)return;
//                else if(o.)
//            }
//            else{
//                this->wp->process(cannot_read_or_write_a_list_or_function, n);
//                return;
//            }
//        }
//        else {
//            this->wp->process(cannot_read_or_write_a_list_or_function, n);
//            return;
//        }
//    }
    Obj o=process_n_obj(n->sub_node[1]);
    if(o.addr==-1)return;
    quaternion q;
    q.op="write";
//    q.a.is_addr=1;
//    q.a.addr=addr;
//    if(id_type(id)==a_real_var)
    q.a.obj=o;
    this->code.push_back(q);
    this->code_point++;
}
//done
void SemanticAnalyser::process_n_if_stmt(node*n){
    //为了分析生成四元式的过程中不同的if,else,while的作用域不产生冲突
    //用不同的id_table来存储同一函数中不同的if,else,while的变量
    //但是id_table的number_of_id字段记录的是处理四元式时函数栈帧的数据的顶部的地址
    //所以这个地址不应该是从0开始的,而应该是从之前开始的

    //创建一个新的id_table
    IDTable if_table;
    //改变地址偏移量
    int pos = (int)this->id_table.size()-1;
    if_table.number_of_id=this->id_table[pos].number_of_id;
    this->id_table.push_back(if_table);

    //分析n_e_exp
    Inequity eq=process_n_e_exp(n->sub_node[2]);
    //if(eq.op==0)return;//处理出现了问题,但不一定要返回,可以再分析分析
    //占位
    quaternion q;
    this->code.push_back(q);
    this->code_point++;
    //计算总共经历了多少语句
    int code_length_before=this->code_point;
    process_node(n->sub_node[5]);

    //生成无条件跳转四元式并将其加入code,占位
    quaternion qj;
    this->code.push_back(qj);
    this->code_point++;
    //回填条件跳转四元式的内容
    //这里有个很有意思的地方,就是stl容器因为要不断地扩张,所以内容的地址可能是会变的
    quaternion* q_if=&this->code[code_length_before-1];
    q_if->op="jif";
    q_if->a.compare=eq;
    q_if->b.obj.value=this->code_point;
    //处理else语句
    //如果有else
    if(n->sub_node[7]->sub_node[0]->symbol.type!=s_null) {
        int code_else = this->code_point;
        process_node(n->sub_node[7]->sub_node[2]);

        quaternion *q_else = &this->code[code_else - 1];
        q_else->op = "jump";
        q_else->b.obj.value = this->code_point;
    }
    //弹出id_table
    this->id_table.pop_back();
    //分析完成后不用同时更新上一层的地址,因为这里的数据都是局部变量,不能被外界调用
}
//usless by now
void process_eq_and_obj(double *eq_v,int *eq_a,Obj *obj){
    if(obj->is_int||obj->is_real){
        //是一个值
        *eq_v=obj->value;
    }
    else{
        *eq_a=obj->addr;
    }
}
//done
Inequity SemanticAnalyser::process_n_e_exp(node *n) {
    Obj left=this->process_n_obj(n->sub_node[0]);
    Obj right=this->process_n_obj(n->sub_node[2]);
    Inequity eq;
    if(left.addr==-1||right.addr==-1){
        //分析左部或右部时出错
        eq.op=0;
        return eq;
    }
    //处理左部和右部
//    process_eq_and_obj(&(eq.left_value),&(eq.left_addr),&left);
//    process_eq_and_obj(&(eq.right_value),&(eq.right_addr),&right);
    eq.left=left;
    eq.right=right;
    //查看比较运算符是什么
    int type=n->sub_node[1]->sub_node[0]->symbol.type;
    if(type==s_bigger)eq.op='>';
    else if(type==s_smaller)eq.op='<';
    else if(type==s_notequal)eq.op='!';
    else if(type==s_equal)eq.op='=';
    return eq;
}
//done
void SemanticAnalyser::process_n_while_stmt(node*n){
    //为了分析生成四元式的过程中不同的if,else,while的作用域不产生冲突
    //用不同的id_table来存储同一函数中不同的if,else,while的变量
    //但是id_table的number_of_id字段记录的是处理四元式时函数栈帧的数据的顶部的地址
    //所以这个地址不应该是从0开始的,而应该是从之前开始的

    //创建一个新的id_table
    IDTable while_table;
    //改变地址偏移量
    int pos = (int)this->id_table.size()-1;
    while_table.number_of_id=this->id_table[pos].number_of_id;
    this->id_table.push_back(while_table);

    //分析n_e_exp
    Inequity eq=process_n_e_exp(n->sub_node[2]);
    //if(eq.op==0)return;//处理出现了问题,但不一定要返回,可以再分析分析
    //获取jif四元式的位置
    int cbefore=this->code_point;
    //jif四元式占位
    quaternion q;
    this->code.push_back(q);
    this->code_point++;
    //处理语句
    this->process_node(n->sub_node[5]);
    //回填jif
    this->code[cbefore].op="jif";
    this->code[cbefore].a.compare=eq;
    this->code[cbefore].b.obj.value=this->code_point+1;

    //填入jump
    quaternion qj;
    qj.op="jump";
    qj.b.obj.value=cbefore;//跳回到条件跳转那里再做判断
    this->code.push_back(qj);
    this->code_point++;
    //弹出id_table
    this->id_table.pop_back();
}
SemanticAnalyser::SemanticAnalyser(SyntacticAnalyser &sa) {
    this->syn=&sa;
    this->code_point=0;
    this->id_table.push_back(IDTable());
    this->wp=new WrongProcesser;
    this->read_tree();
}
void SemanticAnalyser::read_tree() {
    node* node = this->syn->root;
    for(int i = 0;i<node->sub_node.size();i++){
        process_node(node);
    }
}
//done
bool SemanticAnalyser::must_can_return(int codep) {
    while(codep<this->code_point){
        quaternion&q=this->code[codep];
        if(q.op=="return")return 1;
        else if(q.op=="jif"){
            bool can=this->jump_return(codep);
            if(can)return 1;
            else continue;//不在加一了,在函数中已经指到下一个点了
        }
        codep++;
    }
    return 0;
}
//done
bool SemanticAnalyser::jump_return(int &codep) {
    quaternion&q=this->code[codep];
    int jump=(int)(q.b.obj.value)-1;//jump语句(如果有)的地址
    if(this->code[jump].op=="jump"){
        //有jump语句,需分析是while情况还是if情况
        if((int)this->code[jump].b.obj.value==codep){
            //while情况,并不一定可以return
            //跳过这部分代码
            codep=jump+1;
            return 0;
        }
        else{
            //有else的if情况
            //逐句检查,先检查if下面的,再检查else
            bool if_can_ret=0;
            bool else_can_ret=0;
            codep++;
            while(codep<jump){
                quaternion&q=this->code[codep];
                if(q.op=="return"){
                    if_can_ret=1;
                    break;
                }
                else if(q.op=="jif"){
                    bool can=this->jump_return(codep);
                    if((if_can_ret=can))break;
                }
                codep++;
            }
            if(if_can_ret){
                //if可以返回,接下来看else是否可以返回
                codep++;
                while(codep<(int)code[jump].b.obj.value){
                    quaternion&q=this->code[codep];
                    if(q.op=="return"){
                        else_can_ret=1;
                        break;
                    }
                    else if(q.op=="jif"){
                        bool can=this->jump_return(codep);
                        if((else_can_ret=can))break;
                    }
                    codep++;
                }
            }
            codep=(int)code[jump].b.obj.value;//保证跳过代码
            return (if_can_ret&&else_can_ret);
        }
    }
    //没有else的if,不能一定返回
    codep=jump+1;
    return 0;
}