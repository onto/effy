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

#include "qprolabel.h"

QProLabel::QProLabel(QWidget *parent, int i) :
    QLabel(parent) {

    id = i;
    click = 0;
    ishighlight = false;

    connect(this,SIGNAL(clicked(int)),SLOT(highlight()));
}

void QProLabel::mouseReleaseEvent(QMouseEvent *ev) {

    switch (ev->button()) {
    case Qt::LeftButton : {
            click++;
            click %=2;
            if (click == 0) {
                emit dbl_clicked(id);
            }
            emit clicked(id);
            break;
        }

    default: break;
    }
}

bool QProLabel::isHighlight() {

    return click;
}

void QProLabel::highlight() {

    if (ishighlight) {
        ishighlight = false;
        setUnHighlight();
    } else {
        ishighlight = true;
        setHighlight();
    }
}

void QProLabel::setUnHighlight() {
    setStyleSheet("");
    click = 0;
}

void QProLabel::setHighlight() {
    click = 1;
    setStyleSheet("background-color: rgb(255,255,255);");
}
