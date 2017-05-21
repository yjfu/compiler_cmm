#include "RunThread.h"
#include"SyntacticAnalyser.h"
#include <sstream>

RunThread::RunThread(Ui::compiler_finalClass *ui){
	this->myui = ui;
	flag = 0;
}


RunThread::~RunThread()
{
}
void RunThread::run() {
	SyntacticAnalyser sa("cmm.protocol",this->myui);
	
	QString contain = this->myui->edit->document()->toPlainText();
	stringstream ss(contain.toStdString());
	LexicalAnalyser la(ss, this->myui);
	if (la.success) {
		sa.analyse(la);
	}
	quit();
}
void RunThread::stop() {
	this->flag = 1;
}