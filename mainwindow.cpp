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

    imagescaling = new QFutureWatcher<QImage>(this);
    connect(imagescaling, SIGNAL(resultReadyAt(int)),this,SLOT(resized_image(int)));
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
    case QEvent::Resize :
        OnTableResize();
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
    contentlist = dir.entryInfoList(QStringList() << "*.jpg" << "*.png" << "*.jpeg" << "*.gif",QDir::Files);

    View();
}

void MainWindow::on_treeView_activated(QModelIndex index){

    ui->label->setText(((QDirModel*)ui->treeView->model())->filePath(index));
    this->OpenDir(((QDirModel*)ui->treeView->model())->filePath(index));
}

void MainWindow::View() {

    if (imagescaling->isRunning()) {
        imagescaling->cancel();
        imagescaling->waitForFinished();
    }

    qDeleteAll(labels);
    labels.clear();

    Update();

    QStringList files;

    int q = 0;
    foreach (QFileInfo file,contentlist) {

        files.append(file.filePath());

        QProLabel * imagelabel = new QProLabel(0,q);
        connect(imagelabel,SIGNAL(clicked(int)),this,SLOT(label_changed(int)));

        ui->gridLayout->addWidget(imagelabel,qRound(q/columncount),q%columncount);
        labels.append(imagelabel);
        q++;
    }

    imagescaling->setFuture(QtConcurrent::mapped(files, &MainWindow::Scaled));
}

void MainWindow::on_actionQuit_triggered() {

    this->close();
}

void MainWindow::Update() {

    if (previewsize > ui->scrollArea->size().width()) {
        previewsize = ui->scrollArea->size().width();
    }

    columncount = ui->scrollArea->size().width()/previewsize;

    if (contentlist.size() % columncount == 0) {
        rowcount = contentlist.size() / columncount;
    } else {
        rowcount = contentlist.size() / columncount + 1;
    }
}

void MainWindow::label_changed(int id) {

    OpenPhoto(id);
}

void MainWindow::OpenPhoto(int id) {

    viewwindow = new ViewWindow(contentlist,id);
    viewwindow->show();
}

void MainWindow::OnTableResize() {

    Update();

    for (int i = 0; i < labels.size(); i++) {
        ui->gridLayout->addWidget(labels[i],qRound(i/columncount),i%columncount);
    }
}


void MainWindow::on_largeButton_clicked()
{
    if ( previewsize + 25 < ui->scrollArea->size().width()) {
        previewsize += 25;        
    }
    View();
}

void MainWindow::on_smallButton_clicked()
{
    if ( previewsize - 25 > 25 ) {
        previewsize -= 25;
    }
    View();
}


void MainWindow::resized_image(int q) {

    ShowPreview(q);
}

void MainWindow::ShowPreview(int q) {

    labels[q]->setPixmap(QPixmap::fromImage(imagescaling->resultAt(q)));
    labels[q]->setFixedSize(previewsize,previewsize);
}

QImage MainWindow::Scaled(const QString &file) {

    QImage * image = new QImage(file);

    if (image->width() > image->height()) {
        *image = image->scaledToWidth(previewsize,Qt::SmoothTransformation);
    } else {
        *image = image->scaledToHeight(previewsize,Qt::SmoothTransformation);
    }

    return *image;
}



#endif // QT_NO_CONCURRENT
