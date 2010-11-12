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

#include "viewwindow.h"
#include "ui_viewwindow.h"

ViewWindow::ViewWindow(QFileInfoList content, int id, QWidget *parent): QMainWindow(parent), ui(new Ui::ViewWindow) {

    ui->setupUi(this);
    this->move(qApp->desktop()->availableGeometry(this).center()-rect().center());

    contentlist = content;
    photoid = id;
    scale = 100;
    fit = 1;

    settings = new QSettings("effy","effy");

    zoomlabel = new QLabel(tr("Zoom: 100%"));
    ui->statusbar->addWidget(zoomlabel);

    this->restoreGeometry(settings->value("viewwindow_size").toByteArray());

    InitToolBar();

    OpenPhoto();
    zoom_fit();
}

ViewWindow::~ViewWindow() {

    delete image;
    qDeleteAll(toolbarbuttons);
    delete zoomlabel;
    delete ui;
}

void ViewWindow::resizeEvent(QResizeEvent * e) {

    QMainWindow::resizeEvent(e);
    switch(e->type()) {
    case QResizeEvent::Resize : {
        if (fit) {
            zoom_fit();
        }
        settings->setValue("viewwindow_size",this->saveGeometry());
        break;
    }
    default: break;
    }
}

void ViewWindow::OpenPhoto() {

    image = new QPixmap(contentlist.at(photoid).filePath());
    imagesize = image->size();
}

void ViewWindow::ViewPhoto() {

    OpenPhoto();

    *image = image->scaledToWidth(qRound(imagesize.width() * (scale)/100),Qt::SmoothTransformation);
    ui->label->setAlignment(Qt::AlignCenter);

    ui->label->setPixmap(*image);
    zoomlabel->setText(tr("Zoom: ")+QString::number(scale)+tr("%"));

    this->setWindowTitle(contentlist.at(photoid).fileName());

}

void ViewWindow::InitToolBar() {

    //go to previous
    QPushButton * previous = new QPushButton(QIcon("./icons/go-previous.png"),"");
    connect(previous,SIGNAL(clicked()),this,SLOT(go_previous()));
    ui->toolBar->addWidget(previous);
    toolbarbuttons.append(previous);

    //go to next
    QPushButton * next = new QPushButton(QIcon("./icons/go-next.png"),"");
    connect(next,SIGNAL(clicked()),this,SLOT(go_next()));
    ui->toolBar->addWidget(next);
    toolbarbuttons.append(next);

    ui->toolBar->addSeparator();

    //zoom in
    QPushButton * zoomin = new QPushButton(QIcon("./icons/zoom-in.png"),"");
    connect(zoomin,SIGNAL(clicked()),this,SLOT(zoom_in()));
    ui->toolBar->addWidget(zoomin);
    toolbarbuttons.append(zoomin);

    //zoom out
    QPushButton * zoomout = new QPushButton(QIcon("./icons/zoom-out.png"),"");
    connect(zoomout,SIGNAL(clicked()),this,SLOT(zoom_out()));
    ui->toolBar->addWidget(zoomout);
    toolbarbuttons.append(zoomout);

    //zoom original
    QPushButton * zoomoriginal = new QPushButton(QIcon("./icons/zoom-original.png"),"");
    connect(zoomoriginal,SIGNAL(clicked()),this,SLOT(zoom_original()));
    ui->toolBar->addWidget(zoomoriginal);
    toolbarbuttons.append(zoomoriginal);

    //zoom fit
    QPushButton * zoomfit = new QPushButton(QIcon("./icons/zoom-best-fit.png"),"");
    connect(zoomfit,SIGNAL(clicked()),this,SLOT(zoom_fit()));
    ui->toolBar->addWidget(zoomfit);
    toolbarbuttons.append(zoomfit);

    //zoom fullscreen
    QPushButton * zoomfullscreen = new QPushButton(QIcon("./icons/view-fullscreen.png"),"");
    connect(zoomfullscreen,SIGNAL(clicked()),this,SLOT(fullscreen()));
    ui->toolBar->addWidget(zoomfullscreen);
    toolbarbuttons.append(zoomfullscreen);

    for (int i = 0; i < toolbarbuttons.size(); i++) {
        toolbarbuttons.at(i)->setFlat(true);
        toolbarbuttons.at(i)->setFixedSize(settings->value("icon_size").toInt()+4,settings->value("icon_size").toInt()+4);
        toolbarbuttons.at(i)->setIconSize(QSize(settings->value("icon_size").toInt(),settings->value("icon_size").toInt()));
    }

    ui->toolBar->addSeparator();


}

void ViewWindow::on_actionQuit_triggered() {

    close();
}

void ViewWindow::go_previous() {

    photoid--;

    if (photoid >= 0) {
        OpenPhoto();
        zoom_fit();
    } else {
        photoid = 0;
    }
}

void ViewWindow::go_next() {

    photoid++;

    if (photoid < contentlist.size()) {
        OpenPhoto();
        zoom_fit();
    } else {
        photoid = contentlist.size() - 1;
    }
}

void ViewWindow::zoom_in() {

    if (scale < 10) {
        scale += 1;
    } else {
        if (scale < 200) {
            scale += 5;
        } else {
            scale += 25;
        }
    }
    fit = 0;
    ViewPhoto();
}

void ViewWindow::zoom_out() {

    if (scale <= 10) {
        if (scale > 1) {
           scale -= 1;
        }
    } else {
        if (scale > 200) {
            scale -= 25;
        } else {
            scale -= 5;
        }
    }
    fit = 0;
    ViewPhoto();
}

void ViewWindow::zoom_original() {

    fit = 0;
    scale = 100;
    ViewPhoto();
}

void ViewWindow::zoom_fit() {
    fit = 1;
    scale = trunc(qMin((float(ui->scrollArea->width()) / float(imagesize.width())),(float(ui->scrollArea->geometry().height()) / float(imagesize.height()))) * 100);
    ViewPhoto();
}

void ViewWindow::fullscreen() {


}

