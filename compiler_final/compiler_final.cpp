#include "compiler_final.h"
#include <qfile.h>
#include <qtextstream.h>
#include <QFileDialog>
#include <qpalette.h>
#include <qpainter>
#include <QPaintEvent>
#include <QTextBlock>
#include <qtextcodec.h>
#include"SyntacticAnalyser.h"
#include "SemanticAnalyser.h"
#include"Excuter.h"
#include <sstream>

compiler_final::compiler_final(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

compiler_final::~compiler_final()
{

}
void compiler_final::on_actionOpen_triggered()
{
	QString name = QFileDialog::getOpenFileName(0, QTextCodec::codecForName("GBK")->toUnicode("选择cmm文件"),
		QTextCodec::codecForName("GBK")->toUnicode("./testPro"), 
		QTextCodec::codecForName("GBK")->toUnicode("*.cmm"), 0, 0);
	QFile f(name);
	f.open(QIODevice::ReadOnly);
	QTextStream t(&f);
	ui.edit->setPlainText(t.read(5000));
}

void compiler_final::on_actionSave_triggered()
{
	QString name = QFileDialog::getSaveFileName(this,
		QTextCodec::codecForName("GBK")->toUnicode("存储"),
		"./testPro",
		QTextCodec::codecForName("GBK")->toUnicode("cmm文件 (*.cmm)"));
	ui.output->setText(name);
	QFile f(name);
	f.open(QIODevice::WriteOnly);
	QTextStream t(&f);
	QString pro = ui.edit->document()->toPlainText();
	QTextDocument *qd = ui.edit->document();
	t << pro;
	f.close();
}

void compiler_final::on_actionRun_triggered()
{
	ui.output->clear();
	
	SyntacticAnalyser sa("cmm.protocol", &this->ui);

	QString contain = this->ui.edit->document()->toPlainText();
	stringstream ss(contain.toStdString());
	LexicalAnalyser la(ss, &this->ui);
	if (la.success) {
		if (sa.analyse(la)) {
			SemanticAnalyser se(sa, &ui);
			if (!se.error) {
				Excuter ex(&se, &ui);
				ex.excute();
			}
		}
		
	}
}
void compiler_final::on_actionExit_triggered() {
	this->close();
}

