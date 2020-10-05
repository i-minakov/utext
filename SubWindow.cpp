#include "SubWindow.h"
#include "ui_SubWindow.h"

SubWindow::SubWindow(MainWindow *window, QWidget *parent) :
    QWidget(parent), ui(new Ui::SubWindow), m_parent(window)
{
    ui->setupUi(this);
}

SubWindow::~SubWindow()
{
    delete ui;
}

void SubWindow::setText(QFile *file)
{
    QTextStream in(file);
    QTextDocument *txtDoc = new QTextDocument(ui->TextArea);

    txtDoc->setDocumentLayout(new QPlainTextDocumentLayout(txtDoc));
    txtDoc->setPlainText(in.readAll());
    ui->TextArea->setDocument(txtDoc);
}


