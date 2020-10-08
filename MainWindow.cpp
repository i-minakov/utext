#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->FileTree->setCurrentIndex(0);

    connect(ui->ChooseDir, &QPushButton::clicked, this, &MainWindow::chooseDir);
    connect(ui->mainToolBar, &QToolBar::topLevelChanged, this, &MainWindow::changeToolBarSize);
    connect(ui->TreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(elementClicked(QModelIndex)));
    connect(ui->MainWindowSP, &QSplitter::splitterMoved, [this]() {
        for (auto &i : m_screen)
            i->resetPosition();
    });
    connect(ui->actionHide, &QAction::triggered, [this]() {
        ui->FileTree->isVisible() ? ui->FileTree->hide() : ui->FileTree->show();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::chooseDir() {
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory", ".",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isNull() && QDir(dir).exists() && QDir(dir).isReadable()) {
        setTreeView(dir);
        ui->FileTree->setCurrentIndex(1);
    }
}

void MainWindow::changeToolBarSize() {
    Qt::ToolBarArea position = QMainWindow::toolBarArea(ui->mainToolBar);

    if (position == Qt::ToolBarArea::LeftToolBarArea
        || position == Qt::ToolBarArea::RightToolBarArea) {
        ui->mainToolBar->setMinimumSize(QSize(TOOLBARSIZE, 0));
        ui->mainToolBar->setMaximumSize(QSize(TOOLBARSIZE, this->size().height()));
    }
    else if (position == Qt::ToolBarArea::TopToolBarArea
             || position == Qt::ToolBarArea::BottomToolBarArea) {
        ui->mainToolBar->setMinimumSize(QSize(0, TOOLBARSIZE));
        ui->mainToolBar->setMaximumSize(QSize(this->size().width(), TOOLBARSIZE));
    }
}

void MainWindow::setTreeView(QString path) {
    QString dirName = Help::getFileName(path);

    ui->DirName->setText(dirName == nullptr ? "/" : dirName);
    m_DirList->setRootPath(path);
    ui->TreeView->setModel(m_DirList);
    ui->TreeView->setRootIndex(m_DirList->index(path));
    for (int i = 1; i < m_DirList->columnCount(); i++)
        ui->TreeView->hideColumn(i);
}

void MainWindow::deleteScreen(SubWindow *screen) {
    screen->hide();
    m_screen.remove(m_screen.indexOf(screen));
    this->setMinimumWidth(this->size().width() / 4 + (m_screen.size() + 1) * 250);
    if (m_screen.size() == 0)
        ui->TextArea->show();
}

void MainWindow::setSignals(QPlainTextEdit *textArea) {
    connect(ui->actionPaste, &QAction::triggered, [this, textArea]() {
        if (m_screen.empty() || !textArea->hasFocus())
            return ;
        textArea->paste();
    });
    connect(ui->actionCopy, &QAction::triggered, [this, textArea]() {
        if (m_screen.empty() || !textArea->hasFocus())
            return ;
        textArea->copy();
    });
    connect(ui->actionCut, &QAction::triggered, [this, textArea]() {
        if (m_screen.empty() || !textArea->hasFocus())
            return ;
        textArea->cut();
    });
    connect(ui->actionUndo, &QAction::triggered, [this, textArea]() {
        if (m_screen.empty() || !textArea->hasFocus())
            return ;
        textArea->undo();
    });
    connect(ui->actionRedo, &QAction::triggered, [this, textArea]() {
        if (m_screen.empty() || !textArea->hasFocus())
            return ;
        textArea->redo();
    });

    connect(ui->actionCut, &QAction::triggered, [this, textArea]() {
        if (m_screen.empty() || !textArea->hasFocus())
            return ;
        textArea->cut();
    });
    connect(ui->actionCut, &QAction::triggered, [this, textArea]() {
        if (m_screen.empty() || !textArea->hasFocus())
            return ;
        textArea->cut();
    });
    connect(ui->actionCut, &QAction::triggered, [this, textArea]() {
        if (m_screen.empty() || !textArea->hasFocus())
            return ;
        textArea->cut();
    });
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    for (auto &i : m_screen)
        i->resetPosition();
    ui->FileTree->setMinimumWidth(this->size().width() / 6);
    ui->FileTree->setMaximumWidth(this->size().width() / 4);
}

void MainWindow::elementClicked(QModelIndex modelIndex) {
    QString path = m_DirList->filePath(modelIndex);
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Message error("Invalid file" + path);
        return ;
    }
    else if (!m_screen.empty() && checkFile(file.fileName()) != nullptr)
        return ;
    else if (this->size().width() / 4 + (m_screen.size() + 1) * 250 > this->size().width())
        return ;
    SubWindow *screen = new SubWindow(this);

    ui->TextArea->hide();
    ui->MainWindowSP->addWidget(screen);
    m_screen.push_back(screen);
    screen->addNewFile(&file);
    this->setMinimumWidth(this->size().width() / 4 + (m_screen.size() + 1) * 250);
    for (auto &i : m_screen)
        i->resetPosition();
}

QWidget *MainWindow::checkFile(QString path) {
    for (auto &i : m_screen)
        if (i->getFiles().find(path) != i->getFiles().end()) {
            int index = int(std::distance(i->getFiles().begin(), i->getFiles().find(path)));
            i->setFocusTab(index);
            return i->getSerach()->getList().at(index);
        }
    return nullptr;
}

QVector<SubWindow *> &MainWindow::getScreen() {
    return m_screen;
}

void MainWindow::dropEvent(QDropEvent *event) {
    if (event->proposedAction() == Qt::CopyAction) {
        if (QDir(event->mimeData()->urls().begin()->toLocalFile()).exists()
            && QDir(event->mimeData()->urls().begin()->toLocalFile()).isReadable()) {
            ui->FileTree->setCurrentIndex(1);
            setTreeView(event->mimeData()->urls().begin()->toLocalFile());
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}
