#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetTreeConf();
    previewsize = 200;
    SetTableConf();
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

    ui->tableWidget->setRowCount(content.size()/col);
    ui->tableWidget->setColumnCount(col);

    columncount = col;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->setRowHeight(i,size);
    }
    for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
        ui->tableWidget->setColumnWidth(j,size);
    }

    contentlist = content;

    QLabel * imagelabel;
    QPixmap * image;


    int q = 0;
    foreach(QFileInfo file,content) {

        image = new QPixmap(file.filePath());

        if (image->width() > image->height()) {
            *image = image->scaledToWidth(size,Qt::FastTransformation);
        } else {
            *image = image->scaledToHeight(size,Qt::FastTransformation);
        }

        imagelabel = new QLabel("");
        imagelabel->setPixmap(*image);

        ui->tableWidget->setCellWidget(q/col,q%col,imagelabel);

        q++;
    }
}

void MainWindow::on_actionQuit_triggered() {

    this->close();
}

void MainWindow::SetTableConf() {


}

void MainWindow::on_tableWidget_cellActivated(int row, int column) {

    OpenPhoto(row * columncount + column);
}

void MainWindow::OpenPhoto(int id) {


    ViewWindow * viewwindow = new ViewWindow(contentlist,id);

    viewwindow->show();
}

