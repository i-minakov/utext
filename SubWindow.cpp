#include "SubWindow.h"
#include "MainWindow.h"
#include "ui_SubWindow.h"

SubWindow::SubWindow(MainWindow *window, QWidget *parent) :
    QWidget(parent), ui(new Ui::SubWindow), m_parent(window) {
    ui->setupUi(this);
    m_search->hide();
    connect(ui->FileList, &QTabWidget::tabCloseRequested, this, &SubWindow::closeTab);
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

    if (m_search->getState())
        newTab->setContentsMargins(0, m_search->getHeight() ? 100 : 50, 0, 0);
    m_search->addNewTag(newTab);
    txtDoc->setDocumentLayout(new QPlainTextDocumentLayout(txtDoc));
    txtDoc->setPlainText(in.readAll());
    textArea->setDocument(txtDoc);
    m_files.insert(file->fileName(), textArea);
    m_parent->setSignals(this, textArea);
}

void SubWindow::closeTab(const int& index) {
    if (index == -1)
        return ;
    QWidget* tabItem = ui->FileList->widget(index);
    QMap<QString, QPlainTextEdit *>::iterator itr = m_files.begin();

    for (int i = 0; i < index; i++)
        itr += 1;
    m_files.erase(itr);
    ui->FileList->removeTab(index);
    delete(tabItem);
    tabItem = nullptr;
    if (m_files.size() == 0)
        m_parent->deleteScreen(this);
}

void SubWindow::resetPosition() {
    if (!m_search->getState())
        return ;
    m_search->setMinimumWidth(this->width());
    m_search->setMaximumWidth(this->width());
    m_search->move(0, 25);
    m_search->show();
    m_search->raise();
}

void SubWindow::setFocusTab(int index) {
    ui->FileList->setCurrentIndex(index);
}

Search *SubWindow::getSerach() {
    return m_search;
}

QMap<QString, QPlainTextEdit *> &SubWindow::getFiles() {
    return m_files;
}

void SubWindow::keyPressEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_F)  && QApplication::keyboardModifiers()
                && Qt::ControlModifier)
        showSearch();
}

void SubWindow::showSearch() {
    m_search->setMinimumWidth(this->width());
    m_search->setMaximumWidth(this->width());
    m_search->move(0, 25);
    m_search->show();
    m_search->raise();
    m_search->hideAction();
    for (auto &i : m_search->getList())
        i->setContentsMargins(0, m_search->getHeight() ? 100 : 50, 0, 0);
}

void SubWindow::dropEvent(QDropEvent *event) {
    if (event->proposedAction() == Qt::CopyAction && event->mimeData()->hasUrls()) {
        foreach (QUrl url, event->mimeData()->urls()) {
            QFile file(url.toLocalFile());

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                Message error("Invalid file" + url.toLocalFile());
                return ;
            }
            else if (!m_parent->getScreen().empty() && m_parent->checkFile(file.fileName()) != nullptr)
                return ;
            addNewFile(&file);
            ui->FileList->setCurrentIndex(ui->FileList->count() - 1);
        }
    }
}

void SubWindow::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

QPlainTextEdit *SubWindow::getActivArea() {
    for (auto &i : m_files) {
        if (i->isEnabled())
            return i;
    }
    return nullptr;
}

void SubWindow::replaceBut() {
    QString repl = m_search->getReplaceText();
    if (repl.isEmpty())
        return;
    m_searchIt->insertText(m_search->getReplaceText());
    downSearch();
}

void SubWindow::upSearch() {
    if (m_searchIt == m_match.begin())
        m_searchIt = m_match.end() - 1;
    else
        m_searchIt--;
    getActivArea()->setTextCursor(*m_searchIt);
}

void SubWindow::downSearch() {
    if (m_searchIt == m_match.end() - 1)
        m_searchIt = m_match.begin();
    else
        m_searchIt++;
    getActivArea()->setTextCursor(*m_searchIt);
}

void SubWindow::textRecieve(QString text) {
    m_match.clear();
    if (text.isEmpty())
        return;
    QRegularExpression regex(text, QRegularExpression::DotMatchesEverythingOption);
    QPlainTextEdit *area = getActivArea();
    QRegularExpressionMatchIterator match = regex.globalMatch(area->document()->toPlainText());
    if (!match.next().hasMatch())
        return;
    QTextCursor cursor = area->document()->find(match.next().captured(0));
    m_match.push_back(cursor);
    while(match.hasNext()) {
        QRegularExpressionMatch i = match.next();
        if (i.hasMatch()) {
            cursor = area->document()->find(i.captured(0), cursor);
            m_match.push_back(cursor);
        }
    }
    area->setTextCursor(m_match.first());
    m_searchIt = m_match.begin();
}
