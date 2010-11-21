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

#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QtGui>
#include <aboutwindow.h>

namespace Ui {
    class ViewWindow;
}

class ViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewWindow(QFileInfoList content, int id, QWidget *parent = 0);
    ~ViewWindow();

private:
    Ui::ViewWindow *ui;
    Aboutwindow * aboutwindow;
    QFileInfoList contentlist;
    QList<QPushButton *> toolbarbuttons;
    QPixmap * image;
    QLabel * zoomlabel;
    QSettings * settings;
    int photoid;
    int scale;
    bool fit;

    void InitToolBar();
    void OpenPhoto();
    void Update();

    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);


private slots:
    void on_actionAbout_Effy_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionQuit_triggered();
    void go_previous();
    void go_next();
    void go_first();
    void go_last();
    void zoom_in();
    void zoom_out();
    void zoom_original();
    void zoom_fit();
    void rotate_right();
    void rotate_left();
    void flip_vertical();
    void flip_horizontal();
};

#endif // VIEWWINDOW_H
