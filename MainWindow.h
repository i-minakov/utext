#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QFileSystemModel>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QTextDocument>
#include <QPlainTextDocumentLayout>
#include <QMap>
#include <iostream>

#define TOOLBARSIZE 25

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
//    QTextDocument *m_txtDoc;
    std::map<QString, QTextDocument *> m_files;

public slots:
    void resizeEvent(QResizeEvent *event);
    void elementClicked(QModelIndex modelIndex);

    void findAction();
};

#endif // MAINWINDOW_H
