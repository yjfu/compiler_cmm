//
// Created by 傅永健 on 16/12/1.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

using namespace std;
#include <sstream>
#include "WrongProcesser.h"

WrongProcesser::WrongProcesser(Ui::compiler_finalClass*aui) {
	this->ui = aui;
}
void WrongProcesser::process(Errorno errorno, node*n) {
	string es = this->creat_wrong_report(errorno, n->symbol.lineNum);
	this->write(es);
}
void WrongProcesser::process_run(Errorno_run errorno_run, int line) {
	string es = this->creat_wrong_report(errorno_run + 13, line);
	this->write(es);
}

string WrongProcesser::creat_wrong_report(int err, int line) {
	string s;
	stringstream ss;
	string num;
	ss << "错误代码:" << err << "," << "位于第" << line << "行";
	ss >> num;

	s = num + "\t" + error_decl[err] + "\n";
	return s;
}
void WrongProcesser::write(string s) {
	QString error = QTextCodec::codecForName("GBK")->toUnicode(s.c_str());
	this->ui->output->append(error);
	ui->output->moveCursor(QTextCursor::End);
}
void WrongProcesser::process_syn1(int line, string should, string but) {
	stringstream ss;
	ss << "语法错误:位于第" << line << "行,符号应为" << should.c_str() << "而不应为" << but.c_str();
	string s;
	ss >> s;
	s = s + "\n";
	write(s);
}
void WrongProcesser::process_syn2(int line, string n, string get, vector<string>&can_be) {
	stringstream ss;
	ss << "语法错误:位于第" << line << "行," << n.c_str() << "开头不接受" << get.c_str();
	string s;
	ss >> s;
	s = s + "\n" + "可以考虑将其更换为:";
	for (vector<string>::iterator it = can_be.begin(); it != can_be.end(); it++) {
		s = s + *it + " ";
	}
	s = s + "\n";
	write(s);
}