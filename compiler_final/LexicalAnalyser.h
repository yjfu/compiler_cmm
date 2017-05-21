
//
// Created by 傅永健 on 16/10/9.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#ifndef COMPILER_LEXICALANALYSER_H
#define COMPILER_LEXICALANALYSER_H
#include <cstdlib>
#include <string>
#include <vector>
#include"compiler_final.h"
#define SMALLEST_NONTERNMENAL 32//最小的非终结符,更小的就是终结符了
#define LARGEST_NONTERNMENAL 64//最大非终结符+1
using namespace std;
struct Symbol {
	Symbol(string s) {
		contain = s;
	}
	Symbol(int t) {
		type = t;
	}
	Symbol() {}
	int type;
	int lineNum;
	double value;
	string contain;
};
static int type_num = 64;
static string type_name[] = {
	"$",
	"#",
	"id",
	"inum",
	"rnum",
	"s_minus",
	"s_plus",
	"s_mult",
	"s_dev",
	"s_lbp",
	"s_rbp",
	"s_lmp",
	"s_rmp",
	"s_lsp",
	"s_rsp",
	"s_assign",
	"s_equal",
	"s_bigger",
	"s_smaller",
	"s_notequal",
	"s_comma",
	"s_semi",
	"s_int",
	"s_real",
	"s_void",
	"s_while",
	"s_if",
	"s_else",
	"s_read",
	"s_write",
	"s_return",
	"s_call",
	//非终结符
	"n_program",
	"n_fun_decl_list",
	"n_fun_decl",
	"n_type_f",
	"n_para_list",
	"n_para_list_q",
	"n_para_dec",
	"n_type_v",
	"n_stmt_list",
	"n_stmt",
	"n_exp_stmt",
	"n_e",
	"n_e_q",
	"n_t",
	"n_t_q",
	"n_f",
	"n_obj",
	"n_call_stmt",
	"n_real_plist",
	"n_real_plist_q",
	"n_ret_stmt",
	"n_decl_stmt",
	"n_dfollow",
	"n_read_stmt",
	"n_write_stmt",
	"n_if_stmt",
	"n_else_stmt",
	"n_while_stmt",
	"n_e_exp",
	"n_compare",
	"n_id_f",
	"n_decl_q"
};
enum type_id {
	s_null = 0,
	s_end = 1,         //#,程序结束
	s_id = 2,		   // id
	s_inum = 3,		   //整数
	s_rnum = 4,		   //实数
	s_minus = 5,	   //减号
	s_plus = 6,		   //加号
	s_mult = 7,		   //乘号
	s_dev = 8,		   //除号
	//
	s_lbp = 9,		   //{
	s_rbp = 10,		   //}
	s_lmp = 11,		   //[
	s_rmp = 12,		   //]
	s_lsp = 13,		   //(
	s_rsp = 14,		   //)
	//
	s_assign = 15,	   //=
	s_equal = 16,	   //==
	s_bigger = 17,	   //>
	s_smaller = 18,	   //<
	s_notequal = 19,   //<>
	s_comma = 20,	   //,
	s_semi = 21,	   //;
	//
	s_int = 22,		   //关键字int
	s_real = 23,	   //关键字real
	s_void = 24,	   //关键字void
	s_while = 25,	   //关键字while
	s_if = 26,		   //关键字if
	s_else = 27,
	s_read = 28,	   //关键字read
	s_write = 29,	   //关键字write
	s_return = 30,	   //关键字return
	s_call = 31,	   //关键字call

	//以下为非终结符
	n_program = 32,
	n_fun_decl_list = 33,
	n_fun_decl = 34,
	n_type_f = 35,
	n_para_list = 36,
	n_para_list_q = 37,
	n_para_dec = 38,
	n_type_v = 39,
	n_stmt_list = 40,
	n_stmt = 41,
	n_exp_stmt = 42,
	n_e = 43,
	n_e_q = 44,
	n_t = 45,
	n_t_q = 46,
	n_f = 47,
	n_obj = 48,
	n_call_stmt = 49,
	n_real_plist = 50,
	n_real_plist_q = 51,
	n_ret_stmt = 52,
	n_decl_stmt = 53,
	n_dfollow = 54,
	n_read_stmt = 55,
	n_write_stmt = 56,
	n_if_stmt = 57,
	n_else_stmt = 58,
	n_while_stmt = 59,
	n_e_exp = 60,
	n_compare = 61,
	n_id_f = 62,
	n_decl_q=63
};
class LexicalAnalyser {
public:
	LexicalAnalyser(string file_name);
	LexicalAnalyser(stringstream &ss, Ui::compiler_finalClass *aui);
	vector<Symbol> program;
	bool success;
	Ui::compiler_finalClass *ui;
	string name;
private:
	bool preprocess(string s, int line_num);//用来给符号附近加空格(比如main()的(与main之间),并加入vector
	int classify(string s);//用来给一个符号分好类
	bool is_id(string s);
	bool is_inum(string s);//整数
	bool is_rnum(string s);//实数
	void write(string s);
public:
	void print();//打印结果
};


#endif //COMPILER_LEXICALANALYSER_H

