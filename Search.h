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
    void setFinFocus();
    bool getState();
    bool getHeight();
    QVector<QWidget *> &getList();
    QString getReplaceText();
    QString getFindText();

private:
    Ui::Search *ui;
    QVector<QWidget *> m_tabList;
};

#endif
