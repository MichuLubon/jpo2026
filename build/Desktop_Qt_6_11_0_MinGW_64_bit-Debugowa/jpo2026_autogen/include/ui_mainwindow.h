/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTextEdit *textEdit;
    QLabel *label_2;
    QComboBox *comboBoxCzas;
    QPushButton *pushButton;
    QLabel *labelCPU;
    QProgressBar *progressBar;
    QLabel *labelRAM;
    QProgressBar *progressBar_2;
    QLabel *labelDisk;
    QProgressBar *progressBar_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(937, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(281, 17, 381, 458));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(widget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        textEdit = new QTextEdit(widget);
        textEdit->setObjectName("textEdit");

        verticalLayout->addWidget(textEdit);

        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        comboBoxCzas = new QComboBox(widget);
        comboBoxCzas->addItem(QString());
        comboBoxCzas->addItem(QString());
        comboBoxCzas->addItem(QString());
        comboBoxCzas->setObjectName("comboBoxCzas");
        comboBoxCzas->setEditable(false);

        verticalLayout->addWidget(comboBoxCzas);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);


        verticalLayout_2->addLayout(verticalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);

        labelCPU = new QLabel(widget);
        labelCPU->setObjectName("labelCPU");

        verticalLayout_3->addWidget(labelCPU);

        progressBar = new QProgressBar(widget);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(24);

        verticalLayout_3->addWidget(progressBar);

        labelRAM = new QLabel(widget);
        labelRAM->setObjectName("labelRAM");

        verticalLayout_3->addWidget(labelRAM);

        progressBar_2 = new QProgressBar(widget);
        progressBar_2->setObjectName("progressBar_2");
        progressBar_2->setValue(24);

        verticalLayout_3->addWidget(progressBar_2);

        labelDisk = new QLabel(widget);
        labelDisk->setObjectName("labelDisk");

        verticalLayout_3->addWidget(labelDisk);

        progressBar_3 = new QProgressBar(widget);
        progressBar_3->setObjectName("progressBar_3");
        progressBar_3->setValue(24);

        verticalLayout_3->addWidget(progressBar_3);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 937, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "System Monitor", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "Czas skanowania (s):", nullptr));
        comboBoxCzas->setItemText(0, QCoreApplication::translate("MainWindow", "15", nullptr));
        comboBoxCzas->setItemText(1, QCoreApplication::translate("MainWindow", "30", nullptr));
        comboBoxCzas->setItemText(2, QCoreApplication::translate("MainWindow", "60", nullptr));

        pushButton->setText(QCoreApplication::translate("MainWindow", "START", nullptr));
        labelCPU->setText(QCoreApplication::translate("MainWindow", "CPU", nullptr));
        labelRAM->setText(QCoreApplication::translate("MainWindow", "RAM", nullptr));
        labelDisk->setText(QCoreApplication::translate("MainWindow", "Dysk", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
