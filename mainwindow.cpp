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

#ifndef QT_NO_CONCURRENT

int MainWindow::previewsize = 100;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetTreeConf();
    SetTableConf();

    imagescaling = new QFutureWatcher<QImage>(this);
    connect(imagescaling, SIGNAL(resultReadyAt(int)),this,SLOT(on_resize_image(int)));
}

MainWindow::~MainWindow() {

    imagescaling->cancel();
    imagescaling->waitForFinished();

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

void MainWindow::setPreviewSize(int size) {

    previewsize = size;
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

    if (imagescaling->isRunning()) {
        imagescaling->cancel();
        imagescaling->waitForFinished();
    }

    QDir dir(path);
    contentlist = dir.entryInfoList(QStringList() << "*.jpg" << "*.png" << "*.jpeg" << "*.gif",QDir::Files);

    columncount = ui->tableWidget->width()/previewsize;

    ViewInTable();
}

void MainWindow::on_treeView_activated(QModelIndex index){

    ui->label->setText(((QDirModel*)ui->treeView->model())->filePath(index));
    this->OpenDir(((QDirModel*)ui->treeView->model())->filePath(index));
}

void MainWindow::ViewInTable() {

    qDeleteAll(labels);
    labels.clear();

    ui->tableWidget->setRowCount(contentlist.size()/columncount);
    ui->tableWidget->setColumnCount(columncount);

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->setRowHeight(i,previewsize);
    }
    for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
        ui->tableWidget->setColumnWidth(j,previewsize);
    }

    QStringList files;

    int q = 0;
    foreach (QFileInfo file,contentlist) {

        files.append(file.filePath());

        QLabel * imagelabel = new QLabel;
        imagelabel->setFixedSize(previewsize,previewsize);
        ui->tableWidget->setCellWidget(q/columncount,q%columncount,imagelabel);
        labels.append(imagelabel);
        q++;
    }

    imagescaling->setFuture(QtConcurrent::mapped(files, &MainWindow::Scaled));
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

    ViewInTable();
}


void MainWindow::on_largeButton_clicked()
{
    if ( previewsize + 25 < ui->tableWidget->width() ) {
        previewsize += 25;
        columncount = ui->tableWidget->width() / previewsize;
    }
    ViewInTable();
}

void MainWindow::on_smallButton_clicked()
{
    if ( previewsize - 25 > 25 ) {
        previewsize -= 25;
        columncount = ui->tableWidget->width() / previewsize;
    }
    ViewInTable();
}


void MainWindow::on_resize_image(int q) {

    ShowPreview(q);
}

void MainWindow::ShowPreview(int q) {

    labels[q]->setPixmap(QPixmap::fromImage(imagescaling->resultAt(q)));
    labels[q]->setFixedSize(previewsize,previewsize);
}

QImage MainWindow::Scaled(const QString &file) {

    QImage * image = new QImage(file);

    if (image->width() > image->height()) {
        *image = image->scaledToWidth(previewsize,Qt::FastTransformation);
    } else {
        *image = image->scaledToHeight(previewsize,Qt::FastTransformation);
    }

    return *image;
}

#endif // QT_NO_CONCURRENT
