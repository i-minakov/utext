#ifndef SUBWINDOW_H
#define SUBWINDOW_H

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
#include <QWidget>

class MainWindow;

namespace Ui {
class SubWindow;
}

class SubWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SubWindow(MainWindow *window, QWidget *parent = nullptr);
    ~SubWindow();

    void setText(QFile *file);

private:
    Ui::SubWindow *ui;
    MainWindow *m_parent;
};

#endif // SUBWINDOW_H
