//
// Created by ������ on 16/12/1.
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
	ss << "�������:" << err << "," << "λ�ڵ�" << line << "��";
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
	ss << "�﷨����:λ�ڵ�" << line << "��,����ӦΪ" << should.c_str() << "����ӦΪ" << but.c_str();
	string s;
	ss >> s;
	s = s + "\n";
	write(s);
}
void WrongProcesser::process_syn2(int line, string n, string get, vector<string>&can_be) {
	stringstream ss;
	ss << "�﷨����:λ�ڵ�" << line << "��," << n.c_str() << "��ͷ������" << get.c_str();
	string s;
	ss >> s;
	s = s + "\n" + "���Կ��ǽ������Ϊ:";
	for (vector<string>::iterator it = can_be.begin(); it != can_be.end(); it++) {
		s = s + *it + " ";
	}
	s = s + "\n";
	write(s);
}