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

    contentlist = content;

    this->setWindowTitle(content.at(id).fileName());
    ViewPhoto(content.at(id).filePath());

}

ViewWindow::~ViewWindow() {

    delete ui;
}

void ViewWindow::ViewPhoto(QString file) {

    QPixmap * image;

    image = new QPixmap(file);

    *image = image->scaledToHeight(600);

    ui->label->setPixmap(*image);


}
