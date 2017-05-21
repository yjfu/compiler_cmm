#include "compiler_final.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QString>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	compiler_final w;
	w.show();
	return a.exec();
}