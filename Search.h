#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>

namespace Ui {
class Search;
}

class Search : public QWidget {
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

    void hideAction();
    void addNewTag(QWidget *newTab);
    void deleteTab(QWidget *tab);
    QVector<QWidget *> &getList();
    bool getState();
    bool getHeight();
    QString getReplaceText();

private:
    Ui::Search *ui;
    QVector<QWidget *> m_tabList;
};

#endif
