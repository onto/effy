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

#ifndef RESIZETHREAD_H
#define RESIZETHREAD_H

#include <QThread>
#include <QtGui>

class ResizeThread : public QThread {
    Q_OBJECT
private:
    QFileInfo file;
    QImage * image;
    int size, col, q;

public:
    ResizeThread(QFileInfo file, int size, int col, int q);
    void run();
signals:
    void finished(QImage *,int,int);
};

#endif // RESIZETHREAD_H
