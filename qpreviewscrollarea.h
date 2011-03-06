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

#ifndef QPREVIEWSCROLLAREA_H
#define QPREVIEWSCROLLAREA_H

#include <QtGui>
#include <qprolabel.h>

class QPreviewScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    QPreviewScrollArea();
    ~QPreviewScrollArea();

    void clear();
    void addImage(int id, QPixmap image,QString text);
    void setPreviewSize(int size);
    void update();
    int  borderSize();

private:
    int colcount;
    int previewsize;
    int border;
    QList<QProLabel *> imagelabels;
    QList<QLabel *> textlabels;
    QList<QVBoxLayout *> layouts;
    QGridLayout * gridlayout;
    QVBoxLayout * vboxlayout;
    QSpacerItem * spacer;
    QWidget * scrollareawidget;

signals:    
    void dbl_clicked(int);

private slots:
    void highlight(int id);

};

#endif // QPREVIEWSCROLLAREA_H
