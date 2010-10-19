/**************************************************************************
    Copyright (C) 2010 Lashkov Anton

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    ui->tableWidget->clear();

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

    int q = 0;
    foreach(QFileInfo file,content) {

        ResizeThread * resize = new ResizeThread(file,size,col,q);
        connect(resize,SIGNAL(finished(QImage*,int,int)),this,SLOT(on_resize_image(QImage *,int, int)));

        resize->start();
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

    viewwindow = new ViewWindow(contentlist,id);
    viewwindow->show();
}

void MainWindow::on_resize() {

    ViewInTable(contentlist,previewsize,columncount);
}


void MainWindow::on_largeButton_clicked()
{
    if ( previewsize + 25 < ui->tableWidget->width() ) {
        previewsize += 25;
        columncount = ui->tableWidget->width() / previewsize;
    }
    ViewInTable(contentlist,previewsize,columncount);
}

void MainWindow::on_smallButton_clicked()
{
    if ( previewsize - 25 > 25 ) {
        previewsize -= 25;
        columncount = ui->tableWidget->width() / previewsize;
    }
    ViewInTable(contentlist,previewsize,columncount);
}


void MainWindow::on_resize_image(QImage *image, int col, int q) {

    QLabel * imagelabel = new QLabel("");
    imagelabel->setPixmap(QPixmap::fromImage(*image));

    ui->tableWidget->setCellWidget(q/col,q%col,imagelabel);
}
