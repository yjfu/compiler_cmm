//
// Created by 傅永健 on 16/12/1.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#ifndef COMPILER_WRONGPROCESSER_H
#define COMPILER_WRONGPROCESSER_H

#include "SyntacticAnalyser.h"
#include "compiler_final.h"
#include "qtextcodec.h"


enum Errorno {
	decleared_function = 1,//done
	have_same_para = 2,//done
	decleared_var = 3,//done
	wrong_return_value = 4,//done
	not_decleared_func_or_var = 5,//done
	list_dice_should_not_be_double = 6,//done
	type_is_not_correct = 7,//done
	use_a_list_as_value = 8,//done
	use_a_value_as_list = 9,//done
	use_function_like_a_value_or_list = 10,//done
	cannot_be_reform = 11,//done
	function_may_cannot_return = 12,//done
	too_many_or_to_less_function_para = 13//done
};
static string symbol_chinese[] = {
	"空字",//s_null = 0,
	"程序完结字",//s_end = 1,
	"自定义符号",//s_id = 2,
	"整数",//s_inum = 3,
	"实数",//s_rnum = 4,
	"减号",//s_minus = 5,
	"加号",//s_plus = 6,
	"乘号",//s_mult = 7,
	"除号",//s_dev = 8,
	"左大括号",//s_lbp = 9,
	"右大括号",//s_rbp = 10,
	"左中括号",//s_lmp = 11,
	"右中括号",//s_rmp = 12,
	"左小括号",//s_lsp = 13,
	"右小括号",//s_rsp = 14,
	"等号",//s_assign = 15,
	"相等符号",//s_equal = 16,
	"大于号",//s_bigger = 17,
	"小于号",//s_smaller = 18,
	"不等号",//s_notequal = 19,
	"逗号",//s_comma = 20,
	"分号",//s_semi = 21,
	"int关键字",//s_int = 22,
	"real关键字",//s_real = 23,
	"void关键字",//s_void = 24,
	"while关键字",//s_while = 25,
	"if关键字",//s_if = 26,
	"else关键字",//s_else = 27,
	"read关键字",//s_read = 28,
	"write关键字",//s_write = 29,
	"return关键字",//s_return = 30,
	"call关键字",//s_call = 31,
	"整个程序",//n_program = 32,
	"函数声明",//n_fun_decl_list = 33,
	"函数声明",//n_fun_decl = 34,
	"返回类型定义",//n_type_f = 35,
	"参数列表定义",//n_para_list = 36,
	"参数列表定义",//n_para_list_q = 37,
	"参数声明",//n_para_dec = 38,
	"变量类型",//n_type_v = 39,
	"语句",//n_stmt_list = 40,
	"语句",//n_stmt = 41,
	"赋值语句",//n_exp_stmt = 42,
	"表达式",//n_e = 43,
	"加数",//n_e_q = 44,
	"表达式",//n_t = 45,
	"乘数",//n_t_q = 46,
	"表达式",//n_f = 47,
	"计算对象",//n_obj = 48,
	"函数调用语句",//n_call_stmt = 49,
	"实参列表",//n_real_plist = 50,
	"实参列表",//n_real_plist_q = 51,
	"返回语句",//n_ret_stmt = 52,
	"变量声明语句",//n_decl_stmt = 53,
	"数组取值",//n_dfollow = 54,
	"读取语句",//n_read_stmt = 55,
	"打印语句",//n_write_stmt = 56,
	"if语句",//n_if_stmt = 57,
	"else语句",//n_else_stmt = 58,
	"while语句",//n_while_stmt = 59,
	"比较表达式",//n_e_exp = 60,
	"比较符号",//n_compare = 61,
	"数组长度定义"//n_id_f = 62
	"声明列表"//n_decl_q=63
};
static string error_decl[] = {
	"",
	"函数声明重复",//decleared_function
	"参数声明重复",//have_same_para
	"变量声明重复",//decleared_var
	"错误的返回值",//wrong_return_value
	"未声明的函数或变量",//not_decleared_func_or_var
	"数组下标不能为real类型",//list_dice_should_not_be_double
	"参数类型错误",//type_is_not_correct
	"不能像操作变量一样操作整个数组",//use_a_list_as_value
	"不能为变量加下标",//use_a_value_as_list
	"不能使用函数名作为变量或数组",//use_function_like_a_value_or_list
	"不可修改的左值",//cannot_be_reform
	"函数可能不能返回",//function_may_cannot_return
	"函数参数个数错误",//too_many_or_to_less_function_para
	//运行错
	"没有主函数",//no_main_function
	"变量未初始化",// the_value_is_not_assign
	"数组越界",//out_of_list
	"除以零"//devide_zero

};
enum Errorno_run {
	no_main_function = 1,
	the_value_is_not_assign = 2,
	out_of_list = 3,
	devide_zero = 4,
};
class WrongProcesser {
public:
	WrongProcesser(Ui::compiler_finalClass*aui); 
	void process(Errorno errorno, struct node*n);
	void process_run(Errorno_run errorno_run, int line);
	void process_syn1(int line, string should, string but);
	void process_syn2(int line, string n, string get, vector<string>&can_be);
	void write(string s);
	
private:
	string creat_wrong_report(int err, int line);
	Ui::compiler_finalClass*ui;
};


#endif //COMPILER_WRONGPROCESSER_H
