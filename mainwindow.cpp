#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString * start_folder = new QString;
    *start_folder = "/home/";

    OpenStartFolder(start_folder);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::OpenStartFolder(QString *folder) {

    QDirModel model;
    QModelIndex index = model.index(*folder);
    ui->treeView->setModel(&model);
    ui->treeView->setRootIndex(index);

}
