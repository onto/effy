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

    //setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
    this->move(qApp->desktop()->availableGeometry(this).center()-rect().center());
    this->setWindowIcon(QIcon("/usr/share/effy/icons/icon.png"));

    contentlist = content;
    photoid = id;
    scale = 100;
    fit = 1;

    settings = new QSettings("effy","effy");

    zoomlabel = new QLabel(tr("Zoom: 100%"));
    ui->statusbar->addWidget(zoomlabel);

    ui->label->setAlignment(Qt::AlignCenter);

    this->restoreGeometry(settings->value("viewwindow_size").toByteArray());
    ui->scrollArea->setGeometry(settings->value("photoarea_size").toRect());

    initToolBar();

    openPhoto();
    zoom_fit();
}

ViewWindow::~ViewWindow() {

    delete image;
    qDeleteAll(toolbarbuttons);
    toolbarbuttons.clear();
    delete zoomlabel;
    delete settings;
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
        settings->setValue("photoarea_size",ui->scrollArea->geometry());
        break;
    }
    default: break;
    }
}

void ViewWindow::keyPressEvent(QKeyEvent *event) {

    if (event->modifiers() & Qt::ControlModifier) {

        switch (event->key()) {

        case (Qt::Key_Q) : {
            this->close();
            break;
            }
        case (Qt::Key_Left) : {
            this->go_first();
            break;
            }
        case (Qt::Key_Right) : {
            this->go_last();
            break;
            }
        case (Qt::Key_Minus) : {
            this->zoom_out();
            break;
            }
        case (Qt::Key_Equal) : {
            this->zoom_in();
            break;
            }
        case (Qt::Key_F) : {
            this->zoom_fit();
            break;
            }
        case (Qt::Key_0) : {
            this->zoom_original();
            break;
            }
        }

    } else {

        switch (event->key()) {

        case (Qt::Key_Left) : {
            this->go_previous();
            break;
            }
        case (Qt::Key_Right) : {
            this->go_next();
            break;
            }
        }
    }
}

void ViewWindow::wheelEvent(QWheelEvent *event) {

    if (event->delta() < 0) {
        go_next();
    } else {
        go_previous();
    }

}

void ViewWindow::openPhoto() {

    image = new QPixmap(contentlist.at(photoid).filePath());
}

void ViewWindow::update() {

    zoomlabel->setText(tr("Zoom: ")+QString::number(scale)+tr("%"));
    this->setWindowTitle(contentlist.at(photoid).fileName());
}

void ViewWindow::initToolBar() {

    ui->toolBar->setVisible(settings->value("show_viewwindow_toolbar").toBool());

    //go to first
    QPushButton * first = new QPushButton(QIcon::fromTheme("go-first",QIcon("/usr/share/effy/icons/go-first.png")),"");
    connect(first,SIGNAL(clicked()),this,SLOT(go_first()));
    ui->toolBar->addWidget(first);
    toolbarbuttons.append(first);

    //go to previous
    QPushButton * previous = new QPushButton(QIcon::fromTheme("go-previous",QIcon("/usr/share/effy/icons/go-previous.png")),"");
    connect(previous,SIGNAL(clicked()),this,SLOT(go_previous()));
    ui->toolBar->addWidget(previous);
    toolbarbuttons.append(previous);

    //go to next
    QPushButton * next = new QPushButton(QIcon::fromTheme("go-next",QIcon("/usr/share/effy/icons/go-next.png")),"");
    connect(next,SIGNAL(clicked()),this,SLOT(go_next()));
    ui->toolBar->addWidget(next);
    toolbarbuttons.append(next);

    //go to last
    QPushButton * last = new QPushButton(QIcon::fromTheme("go-last",QIcon("/usr/share/effy/icons/go-last.png")),"");
    connect(last,SIGNAL(clicked()),this,SLOT(go_last()));
    ui->toolBar->addWidget(last);
    toolbarbuttons.append(last);

    ui->toolBar->addSeparator();

    //zoom in
    QPushButton * zoomin = new QPushButton(QIcon::fromTheme("zoom-in",QIcon("/usr/share/effy/icons/zoom-in.png")),"");
    connect(zoomin,SIGNAL(clicked()),this,SLOT(zoom_in()));
    ui->toolBar->addWidget(zoomin);
    toolbarbuttons.append(zoomin);

    //zoom out
    QPushButton * zoomout = new QPushButton(QIcon::fromTheme("zoom-out",QIcon("/usr/share/effy/icons/zoom-out.png")),"");
    connect(zoomout,SIGNAL(clicked()),this,SLOT(zoom_out()));
    ui->toolBar->addWidget(zoomout);
    toolbarbuttons.append(zoomout);

    //zoom original
    QPushButton * zoomoriginal = new QPushButton(QIcon::fromTheme("zoom-original",QIcon("/usr/share/effy/icons/zoom-original.png")),"");
    connect(zoomoriginal,SIGNAL(clicked()),this,SLOT(zoom_original()));
    ui->toolBar->addWidget(zoomoriginal);
    toolbarbuttons.append(zoomoriginal);

    //zoom fit
    QPushButton * zoomfit = new QPushButton(QIcon::fromTheme("zoom-best-fit",QIcon("/usr/share/effy/icons/zoom-best-fit.png")),"");
    connect(zoomfit,SIGNAL(clicked()),this,SLOT(zoom_fit()));
    ui->toolBar->addWidget(zoomfit);
    toolbarbuttons.append(zoomfit);

    ui->toolBar->addSeparator();

    //rotate-right
    QPushButton * rotateright = new QPushButton(QIcon::fromTheme("rotate-right",QIcon("/usr/share/effy/icons/rotate-right.png")),"");
    connect(rotateright,SIGNAL(clicked()),this,SLOT(rotate_right()));
    ui->toolBar->addWidget(rotateright);
    toolbarbuttons.append(rotateright);

    //rotate-left
    QPushButton * rotateleft = new QPushButton(QIcon::fromTheme("rotate-left",QIcon("/usr/share/effy/icons/rotate-left.png")),"");
    connect(rotateleft,SIGNAL(clicked()),this,SLOT(rotate_left()));
    ui->toolBar->addWidget(rotateleft);
    toolbarbuttons.append(rotateleft);

    //flip-horizontal
    QPushButton * fliphorizontal = new QPushButton(QIcon::fromTheme("flip-horizontal",QIcon("/usr/share/effy/icons/flip-horizontal.png")),"");
    connect(fliphorizontal,SIGNAL(clicked()),this,SLOT(flip_horizontal()));
    ui->toolBar->addWidget(fliphorizontal);
    toolbarbuttons.append(fliphorizontal);

    //flip-vertical
    QPushButton * flipvertical = new QPushButton(QIcon::fromTheme("flip-vertical",QIcon("/usr/share/effy/icons/flip-vertical.png")),"");
    connect(flipvertical,SIGNAL(clicked()),this,SLOT(flip_vertical()));
    ui->toolBar->addWidget(flipvertical);
    toolbarbuttons.append(flipvertical);

    int size = settings->value("icon_size").toInt();
    int toolbarbuttons_size = toolbarbuttons.size();

    for (int i = 0; i < toolbarbuttons_size; i++) {

        toolbarbuttons.at(i)->setFlat(true);
        toolbarbuttons.at(i)->setFixedSize(size+8,size+8);
        toolbarbuttons.at(i)->setIconSize(QSize(size,size));
    }
}

void ViewWindow::on_actionQuit_triggered() {

    close();
}

void ViewWindow::go_first() {

    if (photoid != 0) {
        photoid = 0;
        openPhoto();
        zoom_fit();
        update();
    }
}

void ViewWindow::go_previous() {

    photoid--;

    if (photoid >= 0) {
        openPhoto();
        zoom_fit();
        update();
    } else {
        photoid = 0;
    }
}

void ViewWindow::go_next() {

    photoid++;

    if (photoid < contentlist.size()) {
        openPhoto();
        zoom_fit();
        update();
    } else {
        photoid = contentlist.size() - 1;
    }
}

void ViewWindow::go_last() {

    if (photoid != contentlist.size() - 1) {
        photoid = contentlist.size() - 1;
        openPhoto();
        zoom_fit();
        update();
    }
}

void ViewWindow::zoom_in() {

    if (scale < 200) {
        scale += 10;
    } else {
        scale += 50;
    }

    ui->label->setPixmap(image->scaledToHeight(trunc(image->height() * (scale)/100),Qt::SmoothTransformation));
    update();
}

void ViewWindow::zoom_out() {

    if (scale > 10) {
        if (scale > 200) {
            scale -= 50;
        } else {
            scale -= 10;
        }
    }

    ui->label->setPixmap(image->scaledToHeight(trunc(image->height() * (scale)/100),Qt::SmoothTransformation));
    update();
}

void ViewWindow::zoom_original() {

    scale = 100;
    ui->label->setPixmap(*image);
    update();
}

void ViewWindow::zoom_fit() {

    scale = trunc(qMin((float(ui->scrollArea->geometry().width()) / float(image->width())),(float(ui->scrollArea->geometry().height()) / float(image->height()))) * 100);

    if (scale > 100) scale = 100;

    ui->label->setPixmap(image->scaledToHeight(trunc(image->height() * (scale)/100 - 1),Qt::SmoothTransformation));

    update();
}

void ViewWindow::rotate_left() {

    QTransform transform;
    transform.rotate(-90);

    *image = image->transformed(transform);

    zoom_fit();
    update();
}

void ViewWindow::rotate_right() {

    QTransform transform;
    transform.rotate(90);

    *image = image->transformed(transform);

    zoom_fit();
    update();
}

void ViewWindow::flip_horizontal() {

    *image = image->fromImage(image->toImage().mirrored(true,false));

    zoom_fit();
    update();
}

void ViewWindow::flip_vertical() {

    *image = image->fromImage(image->toImage().mirrored(false,true));

    zoom_fit();
    update();
}

void ViewWindow::on_actionAbout_Qt_triggered() {

    QMessageBox about;
    about.aboutQt(0,tr("About Qt"));
}

void ViewWindow::on_actionAbout_Effy_triggered() {

    aboutwindow = new Aboutwindow();
    aboutwindow->show();
}
