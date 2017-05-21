//
// Created by ������ on 16/11/21.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#ifndef COMPILER_SEMANTICANALYSER_H
#define COMPILER_SEMANTICANALYSER_H
#include "SyntacticAnalyser.h"
#include "WrongProcesser.h"
enum IDType {
	a_function = 1,
	a_real_var = 2,
	a_int_var = 3,
	a_int_list = 4,
	a_real_list = 5
};
struct mID {
	IDType type;
	string name;
	//��������
	vector<mID> para_list;
	int return_type;
	//��������
	double value;
	//��������
	//������ʾ����Ӻδ���ʼ
	//������ʾ�����ں��������ݶ��е�λ��
	int addr;
	//��������
	int length;
};

struct Obj {
	Obj() {
		is_real = 0;
		is_int = 0;
		ia = 0;
		ra = 0;
		addr = 0;
		have_move = 0;
		line = 0;
		length = 0;
		start_addr = -1;
	}
	int length;//���鳤��
	int line;
	bool have_move;
	bool is_real;
	bool is_int;
	int addr;
	double value;
	//����ר��
	int start_addr;
	//��ַ����һ��ʵ����������
	bool ra;
	bool ia;
	string id;
	Obj* move_obj;
};
struct IDTable {
	IDTable() { number_of_id = 0; }
	map<string, mID> table;
	//��ǵ��Ǳ�����table���Ⱥ�˳��
	vector<string> order;
	int number_of_id;
};
struct Inequity {
	char op;//>,<,=,!
	Obj left;
	Obj right;
};
struct q_contain {
	//    //��־�Ƿ��Ǻ���
	bool is_func;
	//    //��������
	string func_name;
	Obj obj;
	//�Ƚ�����
	Inequity compare;
};
struct OP {
	OP() {};
	int line;
	string action;
};
struct quaternion {
	//(op,a,b,c)
	OP op;
	q_contain a;
	q_contain b;
	q_contain c;
};

class SemanticAnalyser {
public:
	SemanticAnalyser(SyntacticAnalyser& sa, Ui::compiler_finalClass*aui);
	vector<IDTable> id_table;
	vector<quaternion> code;
	int code_point;//==0
	int start_point;//==-1,Ϊmain������һ�����ĵ�ַ
	bool error = 0;
	SyntacticAnalyser* syn;
private:
	Ui::compiler_finalClass*ui;
	WrongProcesser *wp;
	void read_tree();
	void output_quaternion();
private://�ڵ㴦����
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
	void process_n_real_plist(node*n, node*fun_id);
	void process_n_real_plist_q(node*n, node*fun_id, int para_num);
	Obj process_n_e(node*n, int line);
	Obj process_n_f(node*n);
	Obj process_n_t(node*n, int line);
	Obj process_a_param(node*n);
	Obj process_n_obj(node*n);
	Inequity process_n_e_exp(node*n);
	int id_type(string id);
	int have_id(string id);
	bool must_can_return(int codep);
	bool jump_return(int &codep);
};


#endif //COMPILER_SEMANTICANALYSER_H
