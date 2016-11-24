
//
// Created by 傅永健 on 16/10/9.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#include "LexicalAnalyser.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool is_spec_signal(char c) {
	return !(isdigit(c) || isalpha(c) || c == '_');
//	char a[16]=",.[]{}()<>=+-*/";
//	for(int i = 0 ;i<15;i++){
//		if(c==a[i])return 1;
//	}
//	return 0;
}
int getINum(string s) {
	int base = 1;
	int res = 0;
	for (int i = s.length() - 1; i >= 0; i--) {
		res += (s[i] - '0')*base;
		base *= 10;
	}
	return res;
}
double getRNum(string s) {
	double base = 1;
	double res = 0;
	int point = 0;
	for (int i = 0; i<s.length(); i++)
		if (s[i] == '.')
			point = i;
	for (int i = point - 1; i >= 0; i--) {
		res += (s[i] - '0')*base;
		base *= 10;
	}
	base = 0.1;
	for (int i = point + 1; i<s.length(); i++) {
		res += (s[i] - '0')*base;
		base /= 10;
	}
	return res;
}
LexicalAnalyser::LexicalAnalyser(string file_name) {
	fstream fs(file_name, ios::in);
	string s;
	string line;
	int line_num = 0;
	this->success=1;
	while (getline(fs, line)) {
		line_num++;
		stringstream ss(line);
		while (ss >> s) {
			if (!preprocess(s, line_num)) {
				cout << "unexpected input \"" << s << "\"in " << line_num << " line" << "\"!\n";//parsing end\n";
				//return;
				this->success=0;
			}
		}
	}
	//将结束字符输入
	preprocess("#",line_num);
	if(this->success)
		cout << "Lexical Analysis success!"<<endl;
	else
		cout << "Lexical Analysis fail!"<<endl<<"Will not do Syntactic Analysis!"<<endl;

}

bool LexicalAnalyser::preprocess(string s, int line_num) {
	int start = 0;
	for (int i = 1; i<s.length(); i++) {
		//当某一个(除了开头)字符是特殊字符或与前一个字符不同是普通字符时进行特殊处理
		bool cannot_split=1;
		if (is_spec_signal(s[i])) {
			cannot_split = (s[i - 1] == '='&&s[i] == '=') || (s[i - 1] == '<'&&s[i] == '>') || (s[i] == '.');
		}
		else if (is_spec_signal(s[i - 1])) {
			cannot_split = (s[i - 1] == '.');
		}
		if (!cannot_split) {
			string a = s.substr(start, i - start);
			start = i;
			Symbol sym(a);
			sym.type = classify(a);
			if (sym.type == -1)
				return 0;
			else if (sym.type == s_inum)
				sym.value = getINum(a);
			else if (sym.type == s_rnum)
				sym.value = getRNum(a);
			sym.lineNum = line_num;
			this->program.push_back(sym);
		}
	}
	Symbol sym(s.substr(start));
	sym.type = classify(s.substr(start));
	if (sym.type == -1)
		return 0;
	else if (sym.type == s_inum)
		sym.value = getINum(s.substr(start));
	else if (sym.type == s_rnum)
		sym.value = getRNum(s.substr(start));
	sym.lineNum = line_num;
	this->program.push_back(sym);
	return 1;
}
bool LexicalAnalyser::is_id(string s) {
	//这个函数应该在检查完s不是while,if等关键字之后才能调用
	if ((!isalpha(s[0]))||(s[s.length()]=='_'))
		return 0;
	for (int i = 0; i<s.length(); i++)
		if ((!isalpha(s[i])) && (!isdigit(s[i])) && (s[i] != '_'))
			return 0;
	return 1;
}
bool LexicalAnalyser::is_inum(string s) {
	for (int i = 0; i<s.length(); i++) {
		if (!isdigit(s[i]))
			return 0;
	}
	return 1;
}
bool LexicalAnalyser::is_rnum(string s) {
	bool have_point = 0;
	//首尾要是数字(防止.123或123.的情况
	if (!isdigit(s[0]) || !isdigit(s[s.length() - 1]))
		return 0;
	for (int i = 0; i<s.length(); i++) {
		if (s[i] == '.') {
			if (have_point)
				return 0;
			else
				have_point = 1;
		}
		else if (!isdigit(s[i]))
			return 0;
	}
	if (have_point)
		return 1;
	else
		return 0;
}
int LexicalAnalyser::classify(string s) {
	if (this->is_inum(s))
		return s_inum;
	else if (this->is_rnum(s))
		return s_rnum;
	else if (s == "-")
		return s_minus;
	else if (s == "+")
		return s_plus;
	else if (s == "*")
		return s_mult;
	else if (s == "/")
		return s_dev;
	else if (s == "{")
		return s_lbp;
	else if (s == "}")
		return s_rbp;
	else if (s == "[")
		return s_lmp;
	else if (s == "]")
		return s_rmp;
	else if (s == "(")
		return s_lsp;
	else if (s == ")")
		return s_rsp;
	else if (s == "=")
		return s_assign;
	else if (s == "==")
		return s_equal;
	else if (s == ">")
		return s_bigger;
	else if (s == "<")
		return s_smaller;
	else if (s == "<>")
		return s_notequal;
	else if (s == ",")
		return s_comma;
	else if (s == ";")
		return s_semi;
	else if (s == "int")
		return s_int;
	else if (s == "real")
		return s_real;
	else if (s == "void")
		return s_void;
	else if (s == "while")
		return s_while;
	else if (s == "if")
		return s_if;
	else if (s == "else")
		return s_else;
	else if (s == "read")
		return s_read;
	else if (s == "write")
		return s_write;
	else if (s == "return")
		return s_return;
	else if (s == "call")
		return s_call;
	else if (s == "#")
		return s_end;
	else if (this->is_id(s))
		return s_id;
	else return -1;
}
void LexicalAnalyser::print() {
	for (vector<Symbol>::iterator i = this->program.begin(); i != this->program.end(); i++) {
		cout << i->contain << "\ttype:" << type_name[i->type] << "\tline number:" << i->lineNum;
		if (i->type == s_rnum || i->type == s_inum)
			cout << "\tvalue:" << i->value;
		else if (i->value == s_id)
			cout << "\tname:" << i->contain;
		cout << "\n";
	}
}

