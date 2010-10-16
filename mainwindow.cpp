#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetTreeConf();
    previewsize = 200;
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

void MainWindow::OpenDir(QString path) {

    QDir dir(path);
    QFileInfoList content = dir.entryInfoList(QStringList() << "*.jpg" << "*.png" << "*.jpeg" << "*.gif",QDir::Files);

    ViewInTable(content,previewsize,ui->tableWidget->width()/previewsize);
}

void MainWindow::on_treeView_activated(QModelIndex index){

    ui->label->setText(((QDirModel*)ui->treeView->model())->filePath(index));
    this->OpenDir(((QDirModel*)ui->treeView->model())->filePath(index));
}

void MainWindow::ViewInTable(QFileInfoList content,int size,int col) {

    QLabel * imagelabel;

    ui->tableWidget->setRowCount(content.size()/col);
    ui->tableWidget->setColumnCount(col);

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->setRowHeight(i,size);
    }
    for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
        ui->tableWidget->setColumnWidth(j,size);
    }

    int q = 0;
    foreach(QFileInfo file,content) {

        imagelabel = new QLabel("");
        imagelabel->setPixmap(QPixmap(file.filePath()).scaled(size,size));

        ui->tableWidget->setCellWidget(q/col,q%col,imagelabel);
        q++;
    }
}
