#include "Message.h"
#include "ui_Message.h"

Message::Message(QString message, QWidget *parent) : QDialog(parent), ui(new Ui::Message) {
    ui->setupUi(this);
    ui->Text->setText(message);
    ui->Icon->setPixmap(QPixmap(":/resources/error.svg").scaled(90, 90));
    connect(ui->Ok, &QPushButton::clicked, this, &QDialog::close);
    this->setWindowTitle("");
    this->setModal(true);
    this->exec();
}

Message::~Message(){
    delete ui;
}
