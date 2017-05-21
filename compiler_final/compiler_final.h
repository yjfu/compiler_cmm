#ifndef COMPILER_FINAL_H
#define COMPILER_FINAL_H

#include <QtWidgets/QMainWindow>
#include "ui_compiler_final.h"
#include "RunThread.h"

class RunThread;
class compiler_final : public QMainWindow
{
	Q_OBJECT

public:
	compiler_final(QWidget *parent = 0);
	~compiler_final();
	RunThread *rt;
private slots:
	void on_actionOpen_triggered();

	void on_actionSave_triggered();

	void on_actionRun_triggered();

	void on_actionExit_triggered();

	
private:
	Ui::compiler_finalClass ui;

};


#endif // COMPILER_FINAL_H
