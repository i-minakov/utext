#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SubWindow.h"
#include <QMessageBox>

#define TOOLBARSIZE 46

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void chooseDir();
    void changeToolBarSize();
    void setTreeView(QString path);
    void deleteScreen(SubWindow *screen);
    void setSignals();
    QWidget *checkFile(QString path);
    QVector<SubWindow *> &getScreen();
    void saveToFile(QString path, QString text);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_DirList {new QFileSystemModel(this)};
    QVector<SubWindow *> m_screen;

public slots:
    void resizeEvent(QResizeEvent *event) override;
    void elementClicked(QModelIndex modelIndex);
    void createFile();
    void treeCustomMenu(QPoint point);
    void removeItem();
    void renameItem();

protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
};

#endif
