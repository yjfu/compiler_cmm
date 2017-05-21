#pragma once
#include<qthread.h>
#include<compiler_final.h>
class RunThread :
	public QThread
{
public:
	RunThread(Ui::compiler_finalClass *ui);
	~RunThread();
	void stop();
protected:
	void run();
private:
	bool flag;
	Ui::compiler_finalClass *myui;
};

