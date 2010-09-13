#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetTreeConf();
}

MainWindow::~MainWindow() {

    delete ui;

}

void MainWindow::changeEvent(QEvent *e) {

    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }

}

void MainWindow::SetTreeConf() {

    QDirModel * model = new QDirModel;
    QModelIndex index = model->index("");
    model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(index);
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
}

void MainWindow::OpenDirToTab(QString path) {

}

void MainWindow::on_tabWidget_tabCloseRequested(int index){

    ui->tabWidget->removeTab(index);
}

void MainWindow::on_treeView_activated(QModelIndex index)
{
    QTableView * tab = new QTableView;
    QString name = ((QDirModel*)ui->treeView->model())->fileName(index);
    QString path = ((QDirModel*)ui->treeView->model())->filePath(index);
    ui->tabWidget->addTab(tab,name);
    OpenDirToTab(path);
}
