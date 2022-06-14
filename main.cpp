#include "mainwindow.h"
#include <iostream>
#include <conio.h>
#include <QApplication> //for " QApplication a(argc, argv); "

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
