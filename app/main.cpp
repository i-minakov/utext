#include "src/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/resources/text.svg"));
    w.setWindowTitle("Лучше чем VS Code");
    w.show();

    int flag = a.exec();
    system("leaks -q utext");
    return flag;
}
