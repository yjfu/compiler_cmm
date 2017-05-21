//
// Created by 傅永健 on 16/12/8.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#include "Excuter.h"
#include <sstream>
#include<qinputdialog.h>
Excuter::Excuter(SemanticAnalyser *analyser, Ui::compiler_finalClass*aui) {
	this->analyser = analyser;
	this->wp = new WrongProcesser(aui);
	this->ui = aui;
}
void Excuter::myprint(double value, bool is_int) {
	if (error)return;
	stringstream ss;
	if (is_int) ss << (int)value;
	else ss << value;
	string s;
	ss >> s;
	wp->write(s);
}
double Excuter::myread() {
	if (error)return 0;
	bool isOK=0;
	double d;
	while (!isOK) {
		d = QInputDialog::getDouble(NULL, QTextCodec::codecForName("GBK")->toUnicode("输入"),
			QTextCodec::codecForName("GBK")->toUnicode("请输入一个值"),
			QLineEdit::Normal,
			-1000000,
			1000000,
			1,
			&isOK
		);
	}
	return d;
}

void Excuter::excute() {
	vector<quaternion>&vq = this->analyser->code;
	int pointer = this->analyser->start_point;
	if (pointer == -1) {
		//说明没有main函数
		this->wp->process_run(no_main_function, 0);
		this->error = 1;
		return;
	}
	s_ret r = this->excute_func(pointer, true, "main", 0);
	if (this->error) {
		wp->write("程序执行出错\n");
	}
	else {
		stringstream ss;
		if (r.is_int) {
			int v = r.v;
			ss << "程序正常执行,返回值为"<<v;
			string s;
			ss >> s;
			wp->write(s);
		}
		else {
			ss << "程序正常执行,返回值为" << r.v;
			string s;
			ss >> s;
			wp->write(s);
		}
	}
	if (!this->error) { 
		this->process_quater_str(); 
		this->print_quater(); 
		this->analyser->syn->print_tree();
	}
}
s_ret Excuter::excute_func(int point, bool is_main, string name, int return_addr) {
	vector<quaternion>&vq = this->analyser->code;
	mID func = this->analyser->id_table[0].table[name];
	int para_num = (int)func.para_list.size();
	Frame f;
	//处理参数
	Frame now;
	if (!is_main)now = this->my_stack.top();
	for (int i = 0; i<para_num; i++) {
		if (is_main)f.data.push_back(0);//对于main函数,默认实参都是0
		else f.data.push_back(now.data[return_addr + 1 + i]);
		f.top++;
	}
	this->my_stack.push(f);

	//执行语句
	while (vq[point].op.action != "return") {
		//只要主函数不返回,就一直执行
		if (vq[point].op.action == "jif") {
			process_jif(&vq[point], point);
		}
		else if (vq[point].op.action == "jump") {
			process_jump(&vq[point], point);
			continue;//不再执行累加操作
		}
		else if (vq[point].op.action == "call") {
			process_call(&vq[point]);
			//continue;//不再执行累加操作
		}
		else if (vq[point].op.action == "=") {
			process_assign(&vq[point]);
		}
		else if (vq[point].op.action == "+" || vq[point].op.action == "-" ||
			vq[point].op.action == "/" || vq[point].op.action == "*") {
			process_op(&vq[point]);
		}
		else if (vq[point].op.action == "read") {
			process_read(&vq[point]);
		}
		else if (vq[point].op.action == "write") {
			process_write(&vq[point]);
		}
		point++;
	}
	//处理return
	s_ret r;
	if (vq[point].op.action == "return") {
		Obj o = vq[point].a.obj;
		double to_return = get_obj_val(o, vq[point].op.line);
		if (error)return r;
		//栈帧出栈
		this->my_stack.pop();
		if (this->my_stack.empty()) {
			if ((r.is_int = o.is_int || o.ia))to_return = (int)to_return;
			r.v = to_return;
			return r;
		}//执行完了main函数}
		if (o.is_int || o.ia)to_return = (int)to_return;
		Frame& now = this->my_stack.top();
		//先查看有没有这个地址,如果没有就添加足够的地址

		if (now.data.size() <= return_addr) {
			now.data.resize((unsigned long)return_addr + 1);
			now.top = return_addr;
		}
		this->my_stack.top().data[return_addr] = to_return;

	}
	else {
		//栈帧出栈
		this->my_stack.pop();
	}
	return r;
}
double Excuter::get_obj_val(Obj o, int line) {
	if (o.is_int || o.is_real)return o.value;
	int addr = o.addr;
	int move = 0;
	if (o.have_move)move = (int)get_obj_val(*o.move_obj, line);
	if (this->error)return 0;
	//int move=(int)this->my_stack.top().data[o.addr];
	if (move + addr >= this->my_stack.top().data.size()) {
		this->wp->process_run(the_value_is_not_assign, line);
		this->error = 1;
		return 0;
	}
	if (o.start_addr!=-1) {
		if (move + addr >= o.length+o.start_addr) {
			this->wp->process_run(out_of_list, line);
			this->error = 1;
			return 0;
		}
	}
	return this->my_stack.top().data[move + addr];
}
void Excuter::process_jif(quaternion*q, int&point) {
	Inequity ie = q->a.compare;
	Frame& now = this->my_stack.top();
	double left = get_obj_val(ie.left, q->op.line);
	double right = get_obj_val(ie.right, q->op.line);
	if (left == 0 || right == 0)
		if (this->error)
			return;//计算地址时发生错误
	int il = (int)left, ir = (int)right;
	if (ie.right.is_int || ie.right.ia)right = ir;
	if (ie.left.is_int || ie.left.ia)left = il;
	//计算布尔表达式的结果
	bool result;
	if (ie.op == '>')result = (left>right);
	else if (ie.op == '<')result = (left<right);
	else if (ie.op == '=')result = ((left - right)<0.000001);
	else result = ((left - right)>0.000001);
	if (!result) {
		//跳转到b的位置
		int jumpto = (int)q->b.obj.value;
		point = jumpto - 1;
	}
}
void Excuter::process_jump(quaternion*q, int&point) {
	point = (int)q->b.obj.value;
}
void Excuter::process_call(quaternion*q) {
	string name = q->a.func_name;
	int point = this->analyser->id_table[0].table[name].addr;

	excute_func(point, 0, name, q->b.obj.addr);
}
void Excuter::process_read(quaternion*q) {
	Frame& now = this->my_stack.top();
	//先查看有没有这个地址,如果没有就添加足够的地址
	int addr = 0;
	if (q->a.obj.have_move) {
		addr += (int)get_obj_val(*q->a.obj.move_obj, q->op.line);
		if (error)return;
	}
	addr += q->a.obj.addr;
	if (now.data.size() <= addr) {
		now.data.resize((unsigned long)addr + 1);
		now.top = addr;
	}
	now = this->my_stack.top();
	now.data[addr] = myread();
	if (q->a.obj.ia)now.data[addr] = (int)now.data[addr];
}
void Excuter::process_write(quaternion*q) {
	Obj o = q->a.obj;
	double d = get_obj_val(o, q->op.line);
	myprint(d, o.ia || o.is_int);
}
void Excuter::process_assign(quaternion*q) {
	Frame& now = this->my_stack.top();
	//先查看有没有这个地址,如果没有就添加足够的地址
	int a_addr = 0;
	if (q->a.obj.have_move) {
		a_addr += (int)get_obj_val(*q->a.obj.move_obj, q->op.line);
		if (this->error)return;
	}
	a_addr += q->a.obj.addr;
	if (now.data.size() <= a_addr) {
		now.data.resize((unsigned long)a_addr + 1);
		now.top = a_addr;
	}
	if (q->a.obj.length) {
		if (a_addr >= q->a.obj.length+q->a.obj.start_addr) {
			this->wp->process_run(out_of_list, q->op.line);
			this->error = 1;
			return;
		}
	}
	now = this->my_stack.top();//防止扩张时自动更换地址
	if (q->a.obj.ia || q->a.obj.is_int)now.data[a_addr] = (int)get_obj_val(q->b.obj, q->op.line);
	else now.data[a_addr] = get_obj_val(q->b.obj, q->op.line);
}
double Excuter::do_op(OP op, double a, double b) {
	if (op.action == "+")return a + b;
	else if (op.action == "-")return a - b;
	else if (op.action == "*")return a*b;
	else {
		if (b == 0) {
			this->wp->process_run(devide_zero, op.line);
			this->error = 1;
			return 0;
		}
		return a / b;
	}
}
void Excuter::process_op(quaternion *q) {
	double a = get_obj_val(q->a.obj, q->op.line);
	double b = get_obj_val(q->b.obj, q->op.line);
	int ia = (int)a, ib = (int)b;
	if (q->a.obj.is_int || q->a.obj.ia)a = ia;
	if (q->b.obj.is_int || q->b.obj.ia)b = ib;
	if (this->error)return;
	//如果空间不足,申请空间
	if (this->my_stack.top().data.size() <= q->c.obj.addr) {
		this->my_stack.top().data.resize((unsigned long)q->c.obj.addr + 1);
		this->my_stack.top().top = q->c.obj.addr;
	}
	Frame&now = this->my_stack.top();
	if (q->c.obj.ia)now.data[q->c.obj.addr] = (int)do_op(q->op, a, b);
	else now.data[q->c.obj.addr] = do_op(q->op, a, b);
}
void Excuter::print_quater() {
	int count=0;
	this->ui->middle->clear();
	for (vector<string>::iterator it = this->quater_str.begin(); it != this->quater_str.end(); it++) {
		string s;
		stringstream ss;
		ss << count << "." << it->c_str();
		ss >> s;
		QString quat = QTextCodec::codecForName("GBK")->toUnicode(s.c_str());
		this->ui->middle->append(quat);
		ui->middle->moveCursor(QTextCursor::End);
		count++;
	}
}
string process_obj_str(Obj o) {
	stringstream ss;
	string st = "";
	if (o.ia)st = "i";
	else if (o.ra)st = "r";
	if (o.is_int)ss << "i" << (int)o.value;
	else if (o.is_real)ss << "r" << o.value;
	else if (!o.have_move)ss <<st.c_str()<< "*" << o.addr;
	else {
		ss << st.c_str() << "*" << "(" << o.addr << "+" << process_obj_str(*o.move_obj) << ")";
	}
	string s;
	ss >> s;
	return s;
}
void Excuter::process_quater_str() {
	for (vector<quaternion>::iterator it = this->analyser->code.begin(); it != this->analyser->code.end(); it++) {
		string op = it->op.action;
		string a, b, c;
		if (op == "call") {
			a = it->a.func_name;
			int ret_add = it->b.obj.addr;
			stringstream ss;
			ss << ret_add;
			ss >> b;
			c = "_";
		}
		else if (op == "jif") {
			Inequity ie = it->a.compare;
			string l = process_obj_str(it->a.compare.left);
			string r = process_obj_str(it->a.compare.right);
			stringstream ss;
			ss << l.c_str() << it->a.compare.op << r.c_str();
			ss >> a;
			ss.clear();
			ss << (int)it->b.obj.value;
			ss >> b;
			c = "_";
		}
		else if (op == "jump") {
			a = "_";
			stringstream ss;
			ss << (int)it->b.obj.value;
			ss >> b;
			c = "_";
		}
		else if (op == "call") {
			a = it->a.func_name;
			stringstream ss;
			ss << it->b.obj.addr;
			ss >> b;
			c = "_";
		}
		else if (op == "read") {
			a = process_obj_str(it->a.obj);
			b = "_";
			c = "_";
		}
		else if (op == "write") {
			a = process_obj_str(it->a.obj);
			b = "_";
			c = "_";
		}
		else if (op == "=") {
			a = process_obj_str(it->a.obj);
			b = process_obj_str(it->b.obj);
			c = "_";
		}
		else if (op == "+" || op == "-" || op == "*" || op == "/") {
			a = process_obj_str(it->a.obj);
			b = process_obj_str(it->b.obj);
			c = process_obj_str(it->c.obj);
		}
		else if (op == "return") {
			a = process_obj_str(it->a.obj);
			b = "_";
			c = "_";
		}
		stringstream ss;
		ss << "(" << op.c_str() << "," << a.c_str() << "," << b.c_str() << "," << c.c_str() << ")";
		string qu;
		ss >> qu;
		this->quater_str.push_back(qu);
	}
}