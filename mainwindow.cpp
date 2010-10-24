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

int MainWindow::previewsize = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new QSettings("effy","effy");

    SetWidgetsConf();
    SetTreeConf();
    SetToolBarConf();

    connect(ui->splitter,SIGNAL(splitterMoved(int,int)),this,SLOT(resize()));
    connect(this,SIGNAL(widget_resize()),this,SLOT(resize()));

    imagescaling = new QFutureWatcher<QImage>(this);
    connect(imagescaling, SIGNAL(resultReadyAt(int)),this,SLOT(resized_image(int)));
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

void MainWindow::resizeEvent(QResizeEvent *e) {

    QMainWindow::resizeEvent(e);
    switch (e->type()) {
    case QResizeEvent::Resize:
        emit widget_resize();
        break;
    default: break;
    }
}

void MainWindow::SetTreeConf() {

    model = new QDirModel;
    model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(settings->value("root_folder").toString()));
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);

    if (settings->value("save_last_folder").toBool()) {
        ui->treeView->scrollTo(model->index(settings->value("last_folder").toString()));
    }
}


void MainWindow::SetToolBarConf() {

    QPushButton * gohomebutton = new QPushButton(QIcon::fromTheme("go-home"),"");
    connect(gohomebutton,SIGNAL(clicked()),this,SLOT(gohome_clicked()));
    ui->toolBar->addWidget(gohomebutton);

    ui->toolBar->addSeparator();

    QPushButton * settingsbutton = new QPushButton(QIcon::fromTheme("emblem-system"),"");
    connect(settingsbutton,SIGNAL(clicked()),this,SLOT(settings_clicked()));
    ui->toolBar->addWidget(settingsbutton);

    ui->toolBar->addSeparator();

    QPushButton * zoominbutton = new QPushButton(QIcon::fromTheme("zoom-in"),"");
    connect(zoominbutton,SIGNAL(clicked()),this,SLOT(zoomin_clicked()));
    ui->toolBar->addWidget(zoominbutton);

    QPushButton * zoomoutbutton = new QPushButton(QIcon::fromTheme("zoom-out"),"");
    connect(zoomoutbutton,SIGNAL(clicked()),this,SLOT(zoomout_clicked()));
    ui->toolBar->addWidget(zoomoutbutton);
}

void MainWindow::SetWidgetsConf() {

    pathlabel = new QLabel("");
    ui->statusBar->addWidget(pathlabel);

    this->restoreGeometry(settings->value("window_size").toByteArray());
    ui->splitter->restoreState(settings->value("splitter_state").toByteArray());

    if (settings->contains("preview_size")) {
        previewsize = settings->value("preview_size").toInt();
    } else {
        previewsize = 100;
    }
}

void MainWindow::SaveSettings() {

};

void MainWindow::OpenDir(QString path) {

    QDir dir(path);
    contentlist = dir.entryInfoList(QStringList() << "*.jpg" << "*.png" << "*.jpeg" << "*.gif",QDir::Files);

    View();
}

void MainWindow::on_treeView_activated(QModelIndex index){


    pathlabel->setText(((QDirModel*)ui->treeView->model())->filePath(index));
    this->OpenDir(((QDirModel*)ui->treeView->model())->filePath(index));
    settings->setValue("last_folder",pathlabel->text());
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
        connect(imagelabel,SIGNAL(dbl_clicked(int)),this,SLOT(label_dbl_clicked(int)));
        connect(imagelabel,SIGNAL(clicked(int)),this,SLOT(label_clicked(int)));

        imagelabel->setFixedSize(previewsize,previewsize);

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

    columncount = ui->scrollArea->size().width()/previewsize;

    if (columncount == 0) {
        columncount = 1;
    }

    if (contentlist.size() % columncount == 0) {
        rowcount = contentlist.size() / columncount;
    } else {
        rowcount = contentlist.size() / columncount + 1;
    }

    settings->setValue("preview_size",previewsize);
}

void MainWindow::label_dbl_clicked(int id) {

    OpenPhoto(id);
}

void MainWindow::label_clicked(int id) {

    for (int i = 0; i < labels.size(); i++) {
        labels[i]->setUnHighlight();
    }
    labels[id]->setHighlight();
}

void MainWindow::OpenPhoto(int id) {

    viewwindow = new ViewWindow(contentlist,id);
    viewwindow->show();
}

void MainWindow::resize() {

    Update();

    for (int i = 0; i < labels.size(); i++) {
        ui->gridLayout->addWidget(labels[i],qRound(i/columncount),i%columncount);
    }

    settings->setValue("window_size",this->saveGeometry());
    settings->setValue("splitter_state",ui->splitter->saveState());
}

void MainWindow::zoomin_clicked()
{
    if ( previewsize + 25 < ui->scrollArea->size().width()) {
        previewsize += 25;        
    }
    View();
}

void MainWindow::zoomout_clicked()
{
    if ( previewsize - 25 > 25 ) {
        previewsize -= 25;
    }
    View();
}

void MainWindow::settings_clicked() {

    settingswindow = new SettingsWindow();
    settingswindow->show();
}

void MainWindow::gohome_clicked() {

    ui->treeView->scrollTo(model->index(settings->value("root_folder").toString()));
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
