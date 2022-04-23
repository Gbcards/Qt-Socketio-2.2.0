/********************************************************************************
** Form generated from reading UI file 'paintwebsocket.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAINTWEBSOCKET_H
#define UI_PAINTWEBSOCKET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PaintWebSocket
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QWidget *parapinta;
    QPushButton *btnRojo;
    QPushButton *btnVerde;
    QPushButton *btnAmarillo;
    QPushButton *btnNegro;
    QPushButton *btnAzul;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PaintWebSocket)
    {
        if (PaintWebSocket->objectName().isEmpty())
            PaintWebSocket->setObjectName(QStringLiteral("PaintWebSocket"));
        PaintWebSocket->resize(800, 600);
        centralwidget = new QWidget(PaintWebSocket);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        parapinta = new QWidget(centralwidget);
        parapinta->setObjectName(QStringLiteral("parapinta"));

        gridLayout->addWidget(parapinta, 1, 0, 1, 5);

        btnRojo = new QPushButton(centralwidget);
        btnRojo->setObjectName(QStringLiteral("btnRojo"));

        gridLayout->addWidget(btnRojo, 0, 1, 1, 1);

        btnVerde = new QPushButton(centralwidget);
        btnVerde->setObjectName(QStringLiteral("btnVerde"));

        gridLayout->addWidget(btnVerde, 0, 2, 1, 1);

        btnAmarillo = new QPushButton(centralwidget);
        btnAmarillo->setObjectName(QStringLiteral("btnAmarillo"));

        gridLayout->addWidget(btnAmarillo, 0, 4, 1, 1);

        btnNegro = new QPushButton(centralwidget);
        btnNegro->setObjectName(QStringLiteral("btnNegro"));

        gridLayout->addWidget(btnNegro, 0, 0, 1, 1);

        btnAzul = new QPushButton(centralwidget);
        btnAzul->setObjectName(QStringLiteral("btnAzul"));

        gridLayout->addWidget(btnAzul, 0, 3, 1, 1);

        PaintWebSocket->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PaintWebSocket);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        PaintWebSocket->setMenuBar(menubar);
        statusbar = new QStatusBar(PaintWebSocket);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        PaintWebSocket->setStatusBar(statusbar);

        retranslateUi(PaintWebSocket);

        QMetaObject::connectSlotsByName(PaintWebSocket);
    } // setupUi

    void retranslateUi(QMainWindow *PaintWebSocket)
    {
        PaintWebSocket->setWindowTitle(QApplication::translate("PaintWebSocket", "MainWindow", Q_NULLPTR));
        btnRojo->setText(QApplication::translate("PaintWebSocket", "Rojo", Q_NULLPTR));
        btnVerde->setText(QApplication::translate("PaintWebSocket", "Verde", Q_NULLPTR));
        btnAmarillo->setText(QApplication::translate("PaintWebSocket", "Amarillo", Q_NULLPTR));
        btnNegro->setText(QApplication::translate("PaintWebSocket", "Negro", Q_NULLPTR));
        btnAzul->setText(QApplication::translate("PaintWebSocket", "Azul", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PaintWebSocket: public Ui_PaintWebSocket {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAINTWEBSOCKET_H
