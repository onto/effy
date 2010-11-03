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

int MainWindow::previewsize = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect with settings
    settings = new QSettings("effy","effy");

    //configure tree, toolbar, widgets
    SetWidgetsConf();
    SetTreeConf();
    SetToolBarConf();

    imagescaling = new QFutureWatcher<QImage>(this);
    connect(imagescaling, SIGNAL(resultReadyAt(int)),this,SLOT(resized_image(int)));
}

MainWindow::~MainWindow() {

    qDeleteAll(toolbarbuttons);
    delete model;
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

    //set tree model and
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

    if (settings->value("show_toolbar").toBool()) {
        ui->toolBar->setVisible(true);
    } else {
        ui->toolBar->setVisible(false);
    }

    //go home button
    QPushButton * gohomebutton = new QPushButton(QIcon("./icons/go-home.png"),"");
    connect(gohomebutton,SIGNAL(clicked()),this,SLOT(gohome_clicked()));
    ui->toolBar->addWidget(gohomebutton);
    toolbarbuttons.append(gohomebutton);

    ui->toolBar->addSeparator();

    //settings button
    QPushButton * settingsbutton = new QPushButton(QIcon("./icons/emblem-system.png"),"");
    connect(settingsbutton,SIGNAL(clicked()),this,SLOT(settings_clicked()));
    ui->toolBar->addWidget(settingsbutton);
    toolbarbuttons.append(settingsbutton);

    ui->toolBar->addSeparator();

    //zoom in button
    QPushButton * zoominbutton = new QPushButton(QIcon("./icons/zoom-in.png"),"");
    connect(zoominbutton,SIGNAL(clicked()),this,SLOT(zoomin_clicked()));
    ui->toolBar->addWidget(zoominbutton);
    toolbarbuttons.append(zoominbutton);

    //zoom out button
    QPushButton * zoomoutbutton = new QPushButton(QIcon("./icons/zoom-out.png"),"");
    connect(zoomoutbutton,SIGNAL(clicked()),this,SLOT(zoomout_clicked()));
    ui->toolBar->addWidget(zoomoutbutton);
    toolbarbuttons.append(zoomoutbutton);
}

void MainWindow::SetWidgetsConf() {

    this->setWindowTitle("Effy");

    pathlabel = new QLabel("");
    ui->statusBar->addWidget(pathlabel);

    this->restoreGeometry(settings->value("window_size").toByteArray());
    ui->splitter->restoreState(settings->value("splitter_state").toByteArray());

    previewsize = settings->value("preview_size").toInt();

    //connect with resize event
    connect(ui->splitter,SIGNAL(splitterMoved(int,int)),this,SLOT(resize()));
    connect(this,SIGNAL(widget_resize()),this,SLOT(resize()));

}

void MainWindow::SaveSettings() {

};

void MainWindow::OpenDir(QString path) {

    //open dir, save name of files in dir
    QDir dir(path);
    contentlist = dir.entryInfoList(QStringList() << "*.jpg" << "*.png" << "*.jpeg" << "*.gif",QDir::Files);

    View();
}

void MainWindow::on_treeView_activated(QModelIndex index){

    //previews folder files
    pathlabel->setText(qobject_cast<QDirModel*>(ui->treeView->model())->filePath(index));
    this->OpenDir(qobject_cast<QDirModel*>(ui->treeView->model())->filePath(index));
    settings->setValue("last_folder",pathlabel->text());
}

void MainWindow::View() {

    if (imagescaling->isRunning()) {
        imagescaling->cancel();
        imagescaling->waitForFinished();
    }

    qDeleteAll(labels);
    labels.clear();

    qDeleteAll(previewlayouts);
    previewlayouts.clear();

    qDeleteAll(namelabels);
    namelabels.clear();

    Update();

    QStringList files;
    int q = 0;

    foreach (QFileInfo file,contentlist) {

        files.append(file.filePath());

        //make prewiev label, connect with clicked slots
        QProLabel * imagelabel = new QProLabel(0,q);
        connect(imagelabel,SIGNAL(dbl_clicked(int)),this,SLOT(label_dbl_clicked(int)));
        connect(imagelabel,SIGNAL(clicked(int)),this,SLOT(label_clicked(int)));

        imagelabel->setFixedSize(settings->value("preview_size").toInt(),settings->value("preview_size").toInt());
        imagelabel->setAlignment(Qt::AlignCenter);

        QVBoxLayout * layout = new QVBoxLayout();        

        QLabel * namelabel = new QLabel(file.fileName());

        namelabel->setFixedWidth(settings->value("preview_size").toInt());

        layout->addWidget(imagelabel);
        layout->addWidget(namelabel);

        ui->gridLayout->addLayout(layout,qRound(q/columncount),q%columncount);

        labels.append(imagelabel);
        namelabels.append(namelabel);
        previewlayouts.append(layout);
        q++;
    }
    //resize in thread
    previewsize = settings->value("preview_size").toInt();
    imagescaling->setFuture(QtConcurrent::mapped(files, &MainWindow::Scaled));
}

void MainWindow::on_actionQuit_triggered() {

    this->close();
}

void MainWindow::Update() {

    //update some settings
    columncount = ui->scrollArea->size().width()/settings->value("preview_size").toInt();

    if (columncount == 0) {
        columncount = 1;
    }

    if (contentlist.size() % columncount == 0) {
        rowcount = contentlist.size() / columncount;
    } else {
        rowcount = contentlist.size() / columncount + 1;
    }

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

    //on resize event
    Update();

    qDeleteAll(previewlayouts);
    previewlayouts.clear();

    for (int i = 0; i < labels.size(); i++) {
        QVBoxLayout * layout = new QVBoxLayout();

        layout->addWidget(labels[i]);
        layout->addWidget(namelabels[i]);

        previewlayouts.append(layout);
        ui->gridLayout->addLayout(layout,qRound(i/columncount),i%columncount);
    }

    settings->setValue("window_size",this->saveGeometry());
    settings->setValue("splitter_state",ui->splitter->saveState());
}

void MainWindow::zoomin_clicked()
{
    if ( settings->value("preview_size").toInt() + settings->value("preview_step").toInt() < ui->scrollArea->size().width()) {
         settings->setValue("preview_size",settings->value("preview_size").toInt() + settings->value("preview_step").toInt());
    }
    View();
}

void MainWindow::zoomout_clicked()
{
    if ( settings->value("preview_size").toInt() - settings->value("preview_step").toInt() > settings->value("preview_step").toInt() ) {
        settings->setValue("preview_size",settings->value("preview_size").toInt() - settings->value("preview_step").toInt());
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
