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

#include "aboutwindow.h"
#include "ui_aboutwindow.h"

Aboutwindow::Aboutwindow(QWidget *parent) : QWidget(parent), ui(new Ui::Aboutwindow) {

    ui->setupUi(this);
    this->move(qApp->desktop()->availableGeometry(this).center()-rect().center());
    this->setWindowIcon(QIcon("./icons/icon.png"));

    this->setWindowTitle("About Effy");

    ui->label->setPixmap(QPixmap("./icons/icon.png").scaled(32,32,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->label_4->setText(" effy");
}

Aboutwindow::~Aboutwindow() {

    delete ui;
}

void Aboutwindow::keyPressEvent(QKeyEvent *event) {

    if (event->modifiers() & Qt::ControlModifier) {

        switch (event->key()) {

        case (Qt::Key_Q) : {
            this->close();
            break;
        }
        }
    }
}

void Aboutwindow::on_pushButton_clicked() {

    close();
}
