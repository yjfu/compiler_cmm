//
// Created by 傅永健 on 16/12/8.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#include "Excuter.h"
Excuter::Excuter(SemanticAnalyser *analyser) {
    this->analyser=analyser;
    this->wp=new WrongProcesser();
    excute();
}
void myprint(double value,bool is_int){
    if(is_int)printf("%d\n",(int)value);
    else printf("%lf\n",value);
}
double myread(){
    double d;
    scanf("%lf",&d);
    return d;
}
void Excuter::excute() {
    vector<quaternion>&vq=this->analyser->code;
    int pointer=this->analyser->start_point;
    if(pointer==-1){
        //说明没有main函数
        this->wp->process_run(no_main_function);
        return;
    }
    this->excute_func(pointer,true,"main",0);

}
void Excuter::excute_func(int point,bool is_main,string name,int return_addr) {
    vector<quaternion>&vq=this->analyser->code;
    mID func = this->analyser->id_table[0].table[name];
    int para_num=(int)func.para_list.size();
    Frame f;
    //处理参数
    Frame now;
    if(!is_main)now=this->my_stack.top();
    for(int i=0;i<para_num;i++){
        if(is_main)f.data.push_back(0);//对于main函数,默认实参都是0
        else f.data.push_back(now.data[return_addr+1+i]);
        f.top++;
    }
    this->my_stack.push(f);

    //执行语句
    while(vq[point].op!="return"){
        //只要主函数不返回,就一直执行
        if(vq[point].op=="jif"){
            process_jif(&vq[point],point);
        }
        else if(vq[point].op=="jump"){
            process_jump(&vq[point],point);
            continue;//不再执行累加操作
        }
        else if(vq[point].op=="call"){
            process_call(&vq[point]);
            //continue;//不再执行累加操作
        }
        else if (vq[point].op=="="){
            process_assign(&vq[point]);
        }
        else if(vq[point].op=="+"||vq[point].op=="-"||
                vq[point].op=="/"||vq[point].op=="*"){
            process_op(&vq[point]);
        }
        else if(vq[point].op=="read"){
            process_read(&vq[point]);
        }
        else if(vq[point].op=="write"){
            process_write(&vq[point]);
        }
        point++;
    }
    //处理return
    if(vq[point].op=="return"){
        Obj o = vq[point].a.obj;
        double to_return=get_obj_val(o);
        if(error)return;
        //栈帧出栈
        this->my_stack.pop();
        if(this->my_stack.empty())return;//执行完了main函数
        if(o.is_int||o.ia)to_return=(int)to_return;
        this->my_stack.top().data[return_addr]=to_return;
    }
    else {
        //栈帧出栈
        this->my_stack.pop();
    }
}
double Excuter::get_obj_val(Obj o){
    if(o.is_int||o.is_real)return o.value;
    int addr=o.addr;
    int move=0;
    if(o.have_move)move=(int)get_obj_val(*o.move_obj);
    if(this->error)return 0;
    //int move=(int)this->my_stack.top().data[o.addr];
    if(move+addr>=this->my_stack.top().data.size()){
        this->wp->process_run(wrong_addr_access_or_the_value_is_not_assign);
        return 0;
    }
    return this->my_stack.top().data[move+addr];
}
void Excuter::process_jif(quaternion*q,int&point){
    Inequity ie=q->a.compare;
    Frame& now=this->my_stack.top();
    double left=get_obj_val(ie.left);
    double right=get_obj_val(ie.right);
    if(left==0||right==0)
        if(this->error)
            return;//计算地址时发生错误
    int il=(int)left,ir=(int)right;
    if(ie.right.is_int||ie.right.ia)right=ir;
    if(ie.left.is_int||ie.left.ia)left=il;
    //计算布尔表达式的结果
    bool result;
    if(ie.op=='>')result=(left>right);
    else if(ie.op=='<')result=(left<right);
    else if(ie.op=='=')result=((left-right)<0.000001);
    else result=((left-right)>0.000001);
    if(!result){
        //跳转到b的位置
        int jumpto=(int)q->b.obj.value;
        point=jumpto-1;
    }
}
void Excuter::process_jump(quaternion*q,int&point){
    point=(int)q->b.obj.value;
}
void Excuter::process_call(quaternion*q){
    string name = q->a.func_name;
    int point = this->analyser->id_table[0].table[name].addr;
    excute_func(point,0,name,q->b.obj.addr);
}
void Excuter::process_read(quaternion*q){
    Frame& now=this->my_stack.top();
    //先查看有没有这个地址,如果没有就添加足够的地址
    int addr=0;
    if(q->a.obj.have_move){
        addr+=(int)get_obj_val(*q->a.obj.move_obj);
        if(error)return;
    }
    addr+=q->a.obj.addr;
    if(now.data.size()<=addr){
        now.data.resize((unsigned long) addr+1);
        now.top=addr;
    }
    now=this->my_stack.top();
    now.data[addr]=myread();
    if(q->a.obj.ia)now.data[addr]=(int)now.data[addr];
}
void Excuter::process_write(quaternion*q){
    Obj o=q->a.obj;
    myprint(get_obj_val(o),o.ia||o.is_int);
}
void Excuter::process_assign(quaternion*q){
    Frame& now = this->my_stack.top();
    //先查看有没有这个地址,如果没有就添加足够的地址
    int a_addr=0;
    if(q->a.obj.have_move){
        a_addr+=(int)get_obj_val(*q->a.obj.move_obj);
        if(this->error)return;
    }
    a_addr+=q->a.obj.addr;
    if(now.data.size()<=a_addr){
        now.data.resize((unsigned long) a_addr+1);
        now.top=a_addr;
    }
    now=this->my_stack.top();//防止扩张时自动更换地址
    if(q->a.obj.ia||q->a.obj.is_int)now.data[a_addr]=(int)get_obj_val(q->b.obj);
    else now.data[a_addr]=get_obj_val(q->b.obj);
}
double Excuter::do_op(string op,double a,double b){
    if(op=="+")return a+b;
    else if(op=="-")return a-b;
    else if(op=="*")return a*b;
    else {
        if(b==0){
            this->wp->process_run(devide_zero);
            return 0;
        }
        return a/b;
    }
}
void Excuter::process_op(quaternion *q) {
    double a = get_obj_val(q->a.obj);
    double b = get_obj_val(q->b.obj);
    int ia=(int)a,ib=(int)b;
    if(q->a.obj.is_int||q->a.obj.ia)a=ia;
    if(q->b.obj.is_int||q->b.obj.ia)b=ib;
    if(this->error)return;
    //如果空间不足,申请空间
    if(this->my_stack.top().data.size()<=q->c.obj.addr){
        this->my_stack.top().data.resize((unsigned long)q->c.obj.addr+1);
        this->my_stack.top().top=q->c.obj.addr;
    }
    Frame&now=this->my_stack.top();
    if(q->c.obj.ia)now.data[q->c.obj.addr]=(int)do_op(q->op,a,b);
    else now.data[q->c.obj.addr]=do_op(q->op,a,b);
}