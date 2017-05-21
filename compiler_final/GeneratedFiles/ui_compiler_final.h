/********************************************************************************
** Form generated from reading UI file 'compiler_final.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPILER_FINAL_H
#define UI_COMPILER_FINAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE

class Ui_compiler_finalClass
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionSave;
    QAction *actionRun;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTextBrowser *middle;
    CodeEditor *edit;
    QTextBrowser *output;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *compiler_finalClass)
    {
        if (compiler_finalClass->objectName().isEmpty())
            compiler_finalClass->setObjectName(QStringLiteral("compiler_finalClass"));
        compiler_finalClass->resize(1039, 699);
        actionOpen = new QAction(compiler_finalClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/compiler_final/Resources/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionExit = new QAction(compiler_finalClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/compiler_final/Resources/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon1);
        actionSave = new QAction(compiler_finalClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/compiler_final/Resources/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon2);
        actionRun = new QAction(compiler_finalClass);
        actionRun->setObjectName(QStringLiteral("actionRun"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/compiler_final/Resources/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon3);
        centralWidget = new QWidget(compiler_finalClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        middle = new QTextBrowser(centralWidget);
        middle->setObjectName(QStringLiteral("middle"));
        middle->setMaximumSize(QSize(16777215, 287));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(12);
        middle->setFont(font);

        gridLayout->addWidget(middle, 1, 1, 1, 1);

        edit = new CodeEditor(centralWidget);
        edit->setObjectName(QStringLiteral("edit"));
        edit->setFont(font);

        gridLayout->addWidget(edit, 0, 0, 1, 2);

        output = new QTextBrowser(centralWidget);
        output->setObjectName(QStringLiteral("output"));
        output->setMaximumSize(QSize(16777215, 287));
        output->setFont(font);

        gridLayout->addWidget(output, 1, 0, 1, 1);

        compiler_finalClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(compiler_finalClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1039, 23));
        compiler_finalClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(compiler_finalClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        compiler_finalClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(compiler_finalClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        compiler_finalClass->setStatusBar(statusBar);

        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addAction(actionRun);
        mainToolBar->addAction(actionExit);

        retranslateUi(compiler_finalClass);

        QMetaObject::connectSlotsByName(compiler_finalClass);
    } // setupUi

    void retranslateUi(QMainWindow *compiler_finalClass)
    {
        compiler_finalClass->setWindowTitle(QApplication::translate("compiler_finalClass", "compiler_final", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("compiler_finalClass", "open", Q_NULLPTR));
        actionExit->setText(QApplication::translate("compiler_finalClass", "exit", Q_NULLPTR));
        actionSave->setText(QApplication::translate("compiler_finalClass", "save", Q_NULLPTR));
        actionRun->setText(QApplication::translate("compiler_finalClass", "run", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class compiler_finalClass: public Ui_compiler_finalClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPILER_FINAL_H
