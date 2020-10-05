#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SubWindow.h"

#define TOOLBARSIZE 25

class SubWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void chooseDir();
    void changeToolBarSize();
    void setTreeView(QString path);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_DirList {new QFileSystemModel(this)};
    std::map<QString, SubWindow *> m_files;

public slots:
    void resizeEvent(QResizeEvent *event);
    void elementClicked(QModelIndex modelIndex);
    void findAction();
};

#endif // MAINWINDOW_H
