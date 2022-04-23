#include "mainwindow.h"
#include <QApplication>

#include <paintwebsocket.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    PaintWebSocket w;
    w.show();

    return a.exec();
}
