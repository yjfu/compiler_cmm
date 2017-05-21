//
// Created by ������ on 16/12/1.
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
	"����",//s_null = 0,
	"���������",//s_end = 1,
	"�Զ������",//s_id = 2,
	"����",//s_inum = 3,
	"ʵ��",//s_rnum = 4,
	"����",//s_minus = 5,
	"�Ӻ�",//s_plus = 6,
	"�˺�",//s_mult = 7,
	"����",//s_dev = 8,
	"�������",//s_lbp = 9,
	"�Ҵ�����",//s_rbp = 10,
	"��������",//s_lmp = 11,
	"��������",//s_rmp = 12,
	"��С����",//s_lsp = 13,
	"��С����",//s_rsp = 14,
	"�Ⱥ�",//s_assign = 15,
	"��ȷ���",//s_equal = 16,
	"���ں�",//s_bigger = 17,
	"С�ں�",//s_smaller = 18,
	"���Ⱥ�",//s_notequal = 19,
	"����",//s_comma = 20,
	"�ֺ�",//s_semi = 21,
	"int�ؼ���",//s_int = 22,
	"real�ؼ���",//s_real = 23,
	"void�ؼ���",//s_void = 24,
	"while�ؼ���",//s_while = 25,
	"if�ؼ���",//s_if = 26,
	"else�ؼ���",//s_else = 27,
	"read�ؼ���",//s_read = 28,
	"write�ؼ���",//s_write = 29,
	"return�ؼ���",//s_return = 30,
	"call�ؼ���",//s_call = 31,
	"��������",//n_program = 32,
	"��������",//n_fun_decl_list = 33,
	"��������",//n_fun_decl = 34,
	"�������Ͷ���",//n_type_f = 35,
	"�����б���",//n_para_list = 36,
	"�����б���",//n_para_list_q = 37,
	"��������",//n_para_dec = 38,
	"��������",//n_type_v = 39,
	"���",//n_stmt_list = 40,
	"���",//n_stmt = 41,
	"��ֵ���",//n_exp_stmt = 42,
	"���ʽ",//n_e = 43,
	"����",//n_e_q = 44,
	"���ʽ",//n_t = 45,
	"����",//n_t_q = 46,
	"���ʽ",//n_f = 47,
	"�������",//n_obj = 48,
	"�����������",//n_call_stmt = 49,
	"ʵ���б�",//n_real_plist = 50,
	"ʵ���б�",//n_real_plist_q = 51,
	"�������",//n_ret_stmt = 52,
	"�����������",//n_decl_stmt = 53,
	"����ȡֵ",//n_dfollow = 54,
	"��ȡ���",//n_read_stmt = 55,
	"��ӡ���",//n_write_stmt = 56,
	"if���",//n_if_stmt = 57,
	"else���",//n_else_stmt = 58,
	"while���",//n_while_stmt = 59,
	"�Ƚϱ��ʽ",//n_e_exp = 60,
	"�ȽϷ���",//n_compare = 61,
	"���鳤�ȶ���"//n_id_f = 62
	"�����б�"//n_decl_q=63
};
static string error_decl[] = {
	"",
	"���������ظ�",//decleared_function
	"���������ظ�",//have_same_para
	"���������ظ�",//decleared_var
	"����ķ���ֵ",//wrong_return_value
	"δ�����ĺ��������",//not_decleared_func_or_var
	"�����±겻��Ϊreal����",//list_dice_should_not_be_double
	"�������ʹ���",//type_is_not_correct
	"�������������һ��������������",//use_a_list_as_value
	"����Ϊ�������±�",//use_a_value_as_list
	"����ʹ�ú�������Ϊ����������",//use_function_like_a_value_or_list
	"�����޸ĵ���ֵ",//cannot_be_reform
	"�������ܲ��ܷ���",//function_may_cannot_return
	"����������������",//too_many_or_to_less_function_para
	//���д�
	"û��������",//no_main_function
	"����δ��ʼ��",// the_value_is_not_assign
	"����Խ��",//out_of_list
	"������"//devide_zero

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
