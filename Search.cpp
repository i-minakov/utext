#include "Search.h"
#include "ui_Search.h"

#include <QDebug>

Search::Search(QWidget *parent) : QWidget(parent), ui(new Ui::Search) {
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    connect(ui->Hide, &QPushButton::clicked, this, &Search::hideAction);
    connect(ui->Close, &QPushButton::clicked, [this]() {
        this->hideAction();
        this->hide();
    });

    qDebug() << parent->layout()->itemAt(1)->minimumSize().width();
}

Search::~Search() {
    delete ui;
}

void Search::hideAction() {
    delete ui->Buttons->layout();
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
    ui->Buttons->setLayout(layout);
    ui->Buttons->setMinimumWidth(ui->Rep->isVisible() ? 90 : 45);
    ui->Buttons->setMaximumWidth(ui->Rep->isVisible() ? 90 : 45);
    this->setMinimumHeight(ui->Rep->isVisible() ? 50 : 100);
    this->setMaximumHeight(ui->Rep->isVisible() ? 50 : 100);
    ui->Rep->isVisible() ? ui->Rep->hide() : ui->Rep->show();
}

bool Search::getState() {
    return this->isVisible();
}
