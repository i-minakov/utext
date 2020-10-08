#include "Search.h"
#include "ui_Search.h"

Search::Search(QWidget *parent) : QWidget(parent), ui(new Ui::Search) {
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    connect(ui->Hide, &QPushButton::clicked, this, &Search::hideAction);
    connect(ui->Close, &QPushButton::clicked, [this]() {
        this->hideAction();
        this->hide();
        for (auto &i : m_tabList)
            i->setContentsMargins(0, 0, 0, 0);
    });
    connect(ui->Fin, SIGNAL(textEdited(QString)), parentWidget(),
                SLOT(textRecieve(QString)));
    connect(ui->Down, SIGNAL(clicked()), parentWidget(), SLOT(downSearch()));
    connect(ui->Up, SIGNAL(clicked()), parentWidget(), SLOT(upSearch()));
    connect(ui->Rep, SIGNAL(editingFinished()), parentWidget(), SLOT(replaceBut()));
}

Search::~Search() {
    delete ui;
}

void Search::hideAction() {
    delete ui->ButtonsWd->layout();
    QLayout *layout = nullptr;

    if (ui->Rep->isVisible())
        layout = new QHBoxLayout();
    else
        layout = new QVBoxLayout();
    if (!ui->Rep->isVisible())
        layout->addWidget(ui->Close);
    layout->addWidget(ui->Up);
    layout->addWidget(ui->Down);
    if (ui->Rep->isVisible())
        layout->addWidget(ui->Close);
    ui->ButtonsWd->setLayout(layout);
    ui->ButtonsWd->setMinimumWidth(ui->Rep->isVisible() ? 90 : 45);
    ui->ButtonsWd->setMaximumWidth(ui->Rep->isVisible() ? 90 : 45);
    this->setMinimumHeight(ui->Rep->isVisible() ? 50 : 100);
    this->setMaximumHeight(ui->Rep->isVisible() ? 50 : 100);
    ui->Rep->isVisible() ? ui->Rep->hide() : ui->Rep->show();
    for (auto &i : getList())
        i->setContentsMargins(0, getHeight() ? 100 : 50, 0, 0);
}

QString Search::getReplaceText() {
    return ui->Rep->text();
}

void Search::addNewTag(QWidget *newTab) {
    m_tabList.push_back(newTab);
}

QVector<QWidget *> &Search::getList() {
    return m_tabList;
}

bool Search::getState() {
    return this->isVisible();
}

bool Search::getHeight() {
    return ui->Rep->isVisible();
}


