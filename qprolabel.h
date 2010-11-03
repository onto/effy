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

#ifndef QPROLABEL_H
#define QPROLABEL_H

#include <QtGui>

class QProLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QProLabel(QWidget *parent, int id);
    void setUnHighlight();
    void setHighlight();

private:
    int id;
    int click;

signals:
    void dbl_clicked(int);
    void clicked(int);

protected:
    //void mouseReleaseEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

};

#endif // QPROLABEL_H
