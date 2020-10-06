#include "SubWindow.h"
#include "MainWindow.h"
#include "ui_SubWindow.h"

SubWindow::SubWindow(MainWindow *window, QWidget *parent) :
    QWidget(parent), ui(new Ui::SubWindow), m_parent(window) {
    ui->setupUi(this);
    m_search->hide();
}

SubWindow::~SubWindow() {
    delete ui;
}

void SubWindow::addNewFile(QFile *file) {
    QWidget *newTab = new QWidget(ui->FileList);
    QVBoxLayout *vbox = new QVBoxLayout();
    QPlainTextEdit *textArea = new QPlainTextEdit(this);
    QString fileName = Help::getFileName(file->fileName());

    ui->FileList->addTab(newTab, fileName == nullptr ? "/" : fileName);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->addWidget(textArea);
    newTab->setLayout(vbox);
    QTextStream in(file);
    QTextDocument *txtDoc = new QTextDocument(textArea);

    txtDoc->setDocumentLayout(new QPlainTextDocumentLayout(txtDoc));
    txtDoc->setPlainText(in.readAll());
    textArea->setDocument(txtDoc);
    m_files.insert(file->fileName(), txtDoc);
}

void SubWindow::resetPosition() {
    if (!m_search->getState())
        return ;
    m_search->move(this->width() - 255, 25);
    m_search->show();
    m_search->raise();
}

QMap<QString, QTextDocument *> &SubWindow::getFiles() {
    return m_files;
}

void SubWindow::keyPressEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_F)  && QApplication::keyboardModifiers() && Qt::ControlModifier) {
        m_search->move(this->width() - 255, 25);
        m_search->show();
        m_search->raise();
        m_search->hideAction();
    }
}

void SubWindow::dropEvent(QDropEvent *event) {
    if (event->proposedAction() == Qt::CopyAction && event->mimeData()->hasUrls()) {
        foreach (QUrl url, event->mimeData()->urls()) {
            QFile file(url.toLocalFile());

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return ; // error
            else if (!m_parent->getScreen().empty() && m_parent->checkFile(file.fileName()) != -1)
                return ; // set focus
            addNewFile(&file);
        }
    }
}

void SubWindow::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}
