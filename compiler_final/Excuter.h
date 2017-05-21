//
// Created by ������ on 16/12/8.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#ifndef COMPILER_EXCUTER_H
#define COMPILER_EXCUTER_H


#include "SemanticAnalyser.h"
struct Frame {
	Frame() { top = 0; }
	int top;
	vector<double>data;
};
struct s_ret {
	double v = 0;
	bool is_int = 0;
};
class Excuter {
public:
	Excuter(SemanticAnalyser *analyser, Ui::compiler_finalClass*aui);
	void excute();//ִ�н������result��
private:
	vector<double> result;
	SemanticAnalyser *analyser;
	WrongProcesser *wp;
	stack<Frame>my_stack;
	int error = 0;
	vector<string> quater_str;
	Ui::compiler_finalClass*ui;
private:
	s_ret excute_func(int point, bool is_main, string name, int return_addr);
	void process_jif(quaternion*q, int&point);
	void process_jump(quaternion*q, int&point);
	void process_call(quaternion*q);
	void process_read(quaternion*q);
	void process_write(quaternion*q);
	void process_assign(quaternion*q);
	void process_return(quaternion*q);
	void process_op(quaternion*q);
	void print_quater();
	void process_quater_str();
	double do_op(OP op, double a, double b);
	double get_obj_val(Obj o, int line);
	void myprint(double value, bool is_int);
	double myread();
};


#endif //COMPILER_EXCUTER_H
