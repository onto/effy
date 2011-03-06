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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    this->setWindowIcon(QIcon("./icons/icon.png"));

    //connect with settings
    settings = new QSettings("effy","effy");

    //configure tree, toolbar, widgets
    setWidgetsConf();
    setTreeConf();
    setToolBarConf();

    imagescaling = new QFutureWatcher<QImage>(this);
    connect(imagescaling, SIGNAL(resultReadyAt(int)),this,SLOT(add_preview(int)));
}

MainWindow::~MainWindow() {

    qDeleteAll(toolbarbuttons);
    delete scrollarea;
    delete progressbar;
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

void MainWindow::closeEvent(QCloseEvent *) {
    //save geometry
    settings = new QSettings("effy","effy");
    settings->setValue("window_size",this->saveGeometry());
    settings->setValue("splitter_state",ui->splitter->saveState());
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

    if (event->modifiers() & Qt::ControlModifier) {

        switch (event->key()) {

        case (Qt::Key_Q) : {
            this->close();
            break;
        }
        case (Qt::Key_P) : {
            view_settings();
            break;
        }
        case (Qt::Key_Equal) : {
            zoomin();
            break;
        }
        case (Qt::Key_Minus) : {
            zoomout();
            break;
        }
        }
    }
}

void MainWindow::setTreeConf() {

    QString rootfolder = settings->value("root_folder").toString();
    QString lastfolder = settings->value("last_folder").toString();

    //set tree model
    model = new QFileSystemModel();
    model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    model->setRootPath(rootfolder);

    ui->treeView->setModel(model);    
    //ui->treeView->setRootIndex(model->index(rootfolder));
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);

    if (settings->value("save_last_folder").toBool()) {
        ui->treeView->scrollTo(model->index(lastfolder));
    } else {
        ui->treeView->scrollTo(model->index(rootfolder));
    }

    currentpath = "";
}

void MainWindow::setToolBarConf() {

    ui->toolBar->clear();
    qDeleteAll(toolbarbuttons);
    toolbarbuttons.clear();

    ui->toolBar->setVisible(settings->value("show_mainwindow_toolbar").toBool());

    //settings button
    QPushButton * settingsbutton = new QPushButton(QIcon::fromTheme("emblem-system",QIcon("./icons/emblem-system.png")),"");
    connect(settingsbutton,SIGNAL(clicked()),this,SLOT(view_settings()));
    ui->toolBar->addWidget(settingsbutton);
    toolbarbuttons.append(settingsbutton);

    ui->toolBar->addSeparator();

    //zoom in button
    QPushButton * zoominbutton = new QPushButton(QIcon::fromTheme("zoom-in",QIcon("./icons/zoom-in.png")),"");
    connect(zoominbutton,SIGNAL(clicked()),this,SLOT(zoomin()));
    ui->toolBar->addWidget(zoominbutton);
    toolbarbuttons.append(zoominbutton);

    //zoom out button
    QPushButton * zoomoutbutton = new QPushButton(QIcon::fromTheme("zoom-out",QIcon("./icons/zoom-out.png")),"");
    connect(zoomoutbutton,SIGNAL(clicked()),this,SLOT(zoomout()));
    ui->toolBar->addWidget(zoomoutbutton);
    toolbarbuttons.append(zoomoutbutton);

    int size = settings->value("icon_size").toInt();
    int toolbarbuttons_size = toolbarbuttons.size();

    for (int i = 0; i < toolbarbuttons_size; i++) {

        toolbarbuttons.at(i)->setFlat(true);
        toolbarbuttons.at(i)->setFixedSize(size+8,size+8);
        toolbarbuttons.at(i)->setIconSize(QSize(size,size));
    }
}

void MainWindow::setWidgetsConf() {

    this->setWindowTitle("Effy");

    this->restoreGeometry(settings->value("window_size").toByteArray());
    ui->splitter->restoreState(settings->value("splitter_state").toByteArray());

    scrollarea = new QPreviewScrollArea();
    ui->verticalLayout->addWidget(scrollarea);
    connect(scrollarea,SIGNAL(dbl_clicked(int)),this,SLOT(open_photo(int)));

    progressbar = new QProgressBar();
    ui->statusBar->addWidget(progressbar);
    progressbar->setFixedWidth(100);

    //connect with resize event
    connect(ui->splitter,SIGNAL(splitterMoved(int,int)),this,SLOT(resize()));
    connect(this,SIGNAL(widget_resize()),this,SLOT(resize()));

}

void MainWindow::openDir(QString path) {

    //open dir, save name of files in dir
    QDir dir(path);

    contentlist = dir.entryInfoList(settings->value("image_formats").toStringList(),QDir::Files);

    this->setWindowTitle(path+" - effy");
    settings->setValue("last_folder",path);
    currentpath = path;

    ui->treeView->scrollTo(model->index(path));

    view();
}

void MainWindow::on_treeView_activated(QModelIndex index){

    QString path = (qobject_cast<QFileSystemModel*>(ui->treeView->model()))->filePath(index);

    if (currentpath != path) {
        this->openDir(path);
    }
}

void MainWindow::view() {

    previewsize = settings->value("preview_size").toInt();

    //stop resize threads
    if (imagescaling->isRunning()) {
        imagescaling->cancel();
        imagescaling->waitForFinished();
    }

    //update area
    scrollarea->clear();
    scrollarea->setPreviewSize(previewsize);
    scrollarea->update();

    //update progressbar
    progressbar->reset();
    progressbar->setMinimum(0);
    progressbar->setMaximum(contentlist.count() - 1);


    QStringList files;

    //make stringlist with paths of files
    foreach (QFileInfo file,contentlist) {

        files.append(file.filePath());
    }

    //resize in thread
    imagescaling->setFuture(QtConcurrent::mapped(files, &MainWindow::scaled));
}

void MainWindow::on_actionQuit_triggered() {

    this->close();
}

void MainWindow::open_photo(int id) {

    viewwindow = new ViewWindow(contentlist,id);
    viewwindow->show();
}

void MainWindow::resize() {

    //on resize event
    scrollarea->update();
}

void MainWindow::zoomin()
{
    int size = settings->value("preview_size").toInt();
    int step = settings->value("preview_step").toInt();

    if ( size + step + scrollarea->borderSize() < scrollarea->size().width() ) {
         settings->setValue("preview_size", size + step);
         view();
    }
}

void MainWindow::zoomout()
{
    int size = settings->value("preview_size").toInt();
    int step = settings->value("preview_step").toInt();

    if ( size - step > step ) {
        settings->setValue("preview_size",size - step);
        view();
    }
}

void MainWindow::view_settings() {

    settingswindow = new SettingsWindow();
    settingswindow->show();
    connect(settingswindow,SIGNAL(close_window()),this,SLOT(update_settings()));
}

void MainWindow::update_settings() {

    setTreeConf();
    setToolBarConf();
}

void MainWindow::add_preview(int q) {

    scrollarea->addImage(q,QPixmap::fromImage(imagescaling->resultAt(q)),contentlist.at(q).fileName());
    progressbar->setValue(progressbar->value() + 1);
}

QImage MainWindow::scaled(const QString &file) {

    QImage image(file);

    if ((image.width() < previewsize) && (image.height() < previewsize)) return image;

    if (image.width() > image.height()) {
        image = image.scaledToWidth(previewsize,Qt::SmoothTransformation);
    } else {
        image = image.scaledToHeight(previewsize,Qt::SmoothTransformation);
    }

    return image;
}

void MainWindow::on_actionAbout_Qt_triggered() {

    QMessageBox about;
    about.aboutQt(0,tr("About Qt"));
}

void MainWindow::on_actionAbout_Effy_triggered() {

    aboutwindow = new Aboutwindow();
    aboutwindow->show();
}

#endif // QT_NO_CONCURRENT
