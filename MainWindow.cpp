#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->FileTree->setCurrentIndex(0);
    ui->TreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->actionOpen_Folder, &QAction::triggered, this, &MainWindow::chooseDir);
    connect(ui->actionSave_as_2, &QAction::triggered,  [this]() {
        for (auto &i : m_screen) {
            QMap<QString, QPlainTextEdit *> files = i->getFiles();
            for (auto &j : i->getFiles()) {
                if (j->isEnabled() && j->hasFocus()) {
                    QString file = QFileDialog::getSaveFileName(this, "Save file", "", nullptr, nullptr);
                    QFile saveFile(file);
                    if (!saveFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
                        return;
                    QTextStream st(&saveFile);
                    st << j->toPlainText();
                }
            }
        }
    });
    connect(ui->actionSave_2, &QAction::triggered, [this]() {
        for (auto &i : m_screen) {
            QMap<QString, QPlainTextEdit *> files = i->getFiles();
            for (auto &j : i->getFiles()) {
                if (j->isEnabled() && j->hasFocus()) {
                    QFile saveFile(files.key(j));
                    if (!saveFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
                        return;
                    QTextStream st(&saveFile);
                    st << j->toPlainText();
                }
            }
        }
    });

    connect(ui->ChooseDir, &QPushButton::clicked, this, &MainWindow::chooseDir);
    connect(ui->ChNewDir, &QPushButton::clicked, [this]() {
        ui->CloseDir->animateClick();
        chooseDir();
    });
    connect(ui->mainToolBar, &QToolBar::topLevelChanged, this, &MainWindow::changeToolBarSize);
    connect(ui->TreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(elementClicked(QModelIndex)));
    connect(ui->MainWindowSP, &QSplitter::splitterMoved, [this]() {
        for (auto &i : m_screen)
            i->resetPosition();
    });
    connect(ui->actionHide, &QAction::triggered, [this]() {
        ui->FileTree->isVisible() ? ui->FileTree->hide() : ui->FileTree->show();
    });
    connect(ui->actionCut, &QAction::triggered, [this]() {
        for (auto &i : m_screen)
            for (auto &j : i->getFiles())
                if (j->isEnabled() && j->hasFocus())
                    j->cut();
    });
    connect(ui->CloseDir, &QPushButton::clicked, [this]() {
        for (auto &i : m_screen)
            for (int j = i->getFiles().size() - 1; j >= 0; j--)
                i->closeTab(j);
        ui->FileTree->setCurrentIndex(0);
    });
    connect(ui->Create, &QPushButton::clicked, this, &MainWindow::createFile);
    connect(ui->TreeView, SIGNAL(customContextMenuRequested(QPoint)),
        this, SLOT(treeCustomMenu(QPoint)));
    connect(ui->Remove, SIGNAL(clicked()), this, SLOT(removeItem()));
    setSignals();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::removeItem() {
    QString path = m_DirList->filePath(ui->TreeView->currentIndex());
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "", "Are you sure you want to remove '" + path + "'?");
    if (reply == QMessageBox::No)
        return;
    if (QDir(path).exists())
        QDir(path).removeRecursively();
    else
        QFile(path).remove();
}

void MainWindow::renameItem() {
    QString path = m_DirList->filePath(ui->TreeView->currentIndex());
    QString name = QInputDialog::getText(this, "", "Name");
    if (name.isEmpty())
        return;
    if (QDir(path).exists())
        QDir(path).rename(QDir(path).dirName(), name);
    else
        QFile(path).rename(name);
}

void MainWindow::treeCustomMenu(QPoint point) {
    QString path = m_DirList->filePath(ui->TreeView->indexAt(point));
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionRemove("Remove", this);
    connect(&actionRemove, &QAction::triggered, this, &MainWindow::removeItem);
    QAction actionRename("Rename", this);
    connect(&actionRename, &QAction::triggered, this, &MainWindow::renameItem);
    contextMenu.addAction(&actionRename);
    contextMenu.addAction(&actionRemove);
    if (!path.isEmpty() && !QDir(path).exists()) {
        contextMenu.exec(mapToGlobal(point));
        return;
    }
    QAction actionNewFile("New File", this);
    connect(&actionNewFile, &QAction::triggered, this, &MainWindow::createFile);
    QAction actionNewDir("New Folder", this);
    connect(&actionNewDir, &QAction::triggered, [=](){
        QDir(path).mkdir(QInputDialog::getText(this, "", "New Folder name"));
    });
    contextMenu.addAction(&actionNewDir);
    contextMenu.addAction(&actionNewFile);
    contextMenu.exec(mapToGlobal(point));
}

void MainWindow::createFile() {
    QString parentDir;
    if (QDir(m_DirList->filePath(ui->TreeView->currentIndex())).exists())
        parentDir = m_DirList->filePath(ui->TreeView->currentIndex());
    else
        parentDir = m_DirList->filePath(m_DirList->
                                            parent(ui->TreeView->currentIndex()));
    QString file = (parentDir.isEmpty() ? m_DirList->rootPath() : parentDir) +
                "/" + QInputDialog::getText(this, "", "Name of file");
    QFile newFile(file);
    newFile.open(QIODevice::ReadWrite);
    newFile.close();
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

void MainWindow::setSignals() {
    connect(ui->actionPaste, &QAction::triggered, [this]() {
        for (auto &i : m_screen)
            for (auto &j : i->getFiles())
                if (j->isEnabled() && j->hasFocus())
                    j->paste();
    });
    connect(ui->actionCopy, &QAction::triggered, [this]() {
        for (auto &i : m_screen)
            for (auto &j : i->getFiles())
                if (j->isEnabled() && j->hasFocus())
                    j->copy();
    });
    connect(ui->actionCut, &QAction::triggered, [this]() {
        for (auto &i : m_screen)
            for (auto &j : i->getFiles())
                if (j->isEnabled() && j->hasFocus())
                    j->cut();
    });
    connect(ui->actionUndo, &QAction::triggered, [this]() {
        for (auto &i : m_screen)
            for (auto &j : i->getFiles())
                if (j->isEnabled() && j->hasFocus())
                    j->undo();
    });
    connect(ui->actionRedo, &QAction::triggered, [this]() {
        for (auto &i : m_screen)
            for (auto &j : i->getFiles())
                if (j->isEnabled() && j->hasFocus())
                    j->redo();
    });
    connect(ui->actionFind, &QAction::triggered, [this]() {
        for (auto &i : m_screen)
            i->showSearch();
    });
    connect(ui->actionSave_as, &QAction::triggered, [this]() {
        for (auto &i : m_screen) {
            QMap<QString, QPlainTextEdit *> files = i->getFiles();
            for (auto &j : i->getFiles()) {
                if (j->isEnabled() && j->hasFocus()) {
                    QString file = QFileDialog::getSaveFileName(this, "Save file", "", nullptr, nullptr);
                    QFile saveFile(file);
                    if (!saveFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
                        return;
                    QTextStream st(&saveFile);
                    st << j->toPlainText();
                }
            }
        }
    });
    connect(ui->actionReplace, &QAction::triggered, [this]() {
        for (auto &i : m_screen) {
            i->showSearch();
            i->getSerach()->hideAction();
        }
    });
    connect(ui->actionSave, &QAction::triggered, [this]() {
        for (auto &i : m_screen) {
            QMap<QString, QPlainTextEdit *> files = i->getFiles();
            for (auto &j : i->getFiles()) {
                if (j->isEnabled() && j->hasFocus()) {
                    QFile saveFile(files.key(j));
                    if (!saveFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
                        return;
                    QTextStream st(&saveFile);
                    st << j->toPlainText();
                }
            }
        }
    });
    connect(ui->actionFont, &QAction::triggered, [this]() {
        for (auto &i : m_screen)
            for (auto &j : i->getFiles())
                if (j->isEnabled() && j->hasFocus()) {
                    bool ok;
                    QFont font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
                    if (ok)
                        j->setFont(font);
                }
    });
}

void MainWindow::saveToFile(QString path, QString text) {
    QFile saveFile(path);
    if (!saveFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
        return;
    QTextStream st(&saveFile);
    st << text;
    saveFile.close();
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
