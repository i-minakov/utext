#include "Message.h"
#include "ui_Message.h"

Message::Message(QString message, QWidget *parent) : QDialog(parent), ui(new Ui::Message) {
    ui->setupUi(this);
    ui->Text->setText(message);
    connect(ui->Ok, &QPushButton::clicked, this, &QDialog::close);
    this->setModal(true);
    this->exec();
}

Message::~Message(){
    delete ui;
}
