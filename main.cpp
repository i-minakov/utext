#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int flag = a.exec();
//    system("leaks -q utext");
    return flag;
}
