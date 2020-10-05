#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->ChooseDir, &QPushButton::clicked, this, &MainWindow::chooseDir);
    connect(ui->mainToolBar, &QToolBar::topLevelChanged, this, &MainWindow::changeToolBarSize);
    connect(ui->TreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(elementClicked(QModelIndex)));

//    connect(ui->actionCopy, SIGNAL(triggered()), ui->TextEdit, SLOT(copy()));
//    connect(ui->actionPaste, SIGNAL(triggered()), ui->TextEdit, SLOT(paste()));
//    connect(ui->actionUndo, SIGNAL(triggered()), ui->TextEdit, SLOT(undo()));
//    connect(ui->actionRedo, SIGNAL(triggered()), ui->TextEdit, SLOT(redo()));
//    connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(findAction()));
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
    QStringList splitList = path.split("/");

    ui->DirName->setText(path != "/" ? splitList.at(splitList.size() - 1) : path);
    m_DirList->setRootPath(path);
    ui->TreeView->setModel(m_DirList);
    ui->TreeView->setRootIndex(m_DirList->index(path));
    for (int i = 1; i < m_DirList->columnCount(); i++)
        ui->TreeView->hideColumn(i);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    ui->FileTree->setMinimumWidth(this->size().width() / 4);
    ui->FileTree->setMaximumWidth(this->size().width() / 2);
}

void MainWindow::elementClicked(QModelIndex modelIndex) {
    QString path = m_DirList->filePath(modelIndex);
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ; // error
    else if (m_files[path])
        return ; // set focus
    SubWindow *screen = new SubWindow(this);

    ui->TextArea->hide();
    ui->MainWindowSP->addWidget(screen);
    m_files.insert({path, screen});
    screen->setText(&file);
}

void MainWindow::findAction() {

}
