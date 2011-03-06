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

#include "qpreviewscrollarea.h"

QPreviewScrollArea::QPreviewScrollArea() : QScrollArea() {

    this->setWidgetResizable(true);
    this->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    gridlayout = new QGridLayout();
    gridlayout->setSpacing(4);

    vboxlayout = new QVBoxLayout();
    vboxlayout->addLayout(gridlayout);

    spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vboxlayout->addItem(spacer);

    scrollareawidget = new QWidget();
    this->setWidget(scrollareawidget);

    scrollareawidget->setLayout(vboxlayout);

    border = 8;
}

QPreviewScrollArea::~QPreviewScrollArea() {

    clear();
    delete gridlayout;
    delete scrollareawidget;
}

void QPreviewScrollArea::clear() {

   qDeleteAll(imagelabels);
   qDeleteAll(textlabels);
   qDeleteAll(layouts);
   imagelabels.clear();
   textlabels.clear();
   layouts.clear();
}

void QPreviewScrollArea::addImage(int id, QPixmap image,QString text) {

    QProLabel * label = new QProLabel();
    label->setPixmap(image);
    label->setId(id);
    label->setFixedWidth(previewsize+border);
    label->setFixedHeight(image.height()+border);
    label->setAlignment(Qt::AlignCenter);
    imagelabels.insert(id,label);

    connect(label,SIGNAL(dbl_clicked(int)),this,SIGNAL(dbl_clicked(int)));
    connect(label,SIGNAL(clicked(int)),this,SLOT(highlight(int)));

    QLabel * tlabel = new QLabel();
    QFontMetrics metr(tlabel->font());
    text = metr.elidedText(text,Qt::ElideMiddle,previewsize);
    tlabel->setText(text);
    tlabel->setAlignment(Qt::AlignCenter);
    tlabel->setFixedWidth(previewsize+border);
    textlabels.insert(id,tlabel);

    QVBoxLayout * layout = new QVBoxLayout();
    layouts.insert(id,layout);
    gridlayout->addLayout(layout,id/colcount,id%colcount,Qt::AlignBottom);

    layout->insertWidget(0,label);
    layout->insertWidget(1,tlabel);
}

void QPreviewScrollArea::setPreviewSize(int size) {

    previewsize = size;
}

void QPreviewScrollArea::update() {

    qDeleteAll(layouts);
    layouts.clear();
    colcount = trunc(this->width()/(previewsize+border*2));

    int imagelabels_size = imagelabels.size();

    for (int i = 0; i < imagelabels_size; i++) {

        QVBoxLayout * layout = new QVBoxLayout();

        layout->addWidget(imagelabels.at(i));
        layout->addWidget(textlabels.at(i));

        layouts.append(layout);
        gridlayout->addLayout(layout,qRound(i/colcount),i%colcount);
    }
}

void QPreviewScrollArea::highlight(int id) {

    foreach (QProLabel * label,imagelabels) {

        label->setHighlight(false);
    }
    imagelabels.at(id)->setHighlight(true);
}

int QPreviewScrollArea::borderSize() {

    return border;
}
