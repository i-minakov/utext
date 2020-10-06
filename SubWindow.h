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
#include <QUrl>
#include <QWidget>
#include <QMimeData>

#include "Search.h"
#include "Help.h"

class MainWindow;

namespace Ui {
class SubWindow;
}

class SubWindow : public QWidget {
    Q_OBJECT

public:
    explicit SubWindow(MainWindow *window, QWidget *parent = nullptr);
    ~SubWindow() override;

    void addNewFile(QFile *file);
    void resetPosition();
    QMap<QString, QTextDocument *> &getFiles();

private:
    Ui::SubWindow *ui;
    MainWindow *m_parent;
    QMap<QString, QTextDocument *> m_files;
    Search *m_search {new Search(this)};

public slots:
    void keyPressEvent(QKeyEvent *event) override;

protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
};

#endif // SUBWINDOW_H
