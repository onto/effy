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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "viewwindow.h"
#include "resizethread.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    int previewsize;
    QFileInfoList contentlist;
    int columncount;
    ViewWindow * viewwindow;

    Ui::MainWindow *ui;
    void SetTreeConf();
    void SetTableConf();
    void OpenPhoto(int id);
    void OpenDir(QString path);
    void ViewInTable(QFileInfoList content,int size,int col);


private slots:

    void on_smallButton_clicked();
    void on_largeButton_clicked();
    void on_tableWidget_cellActivated(int row, int column);
    void on_actionQuit_triggered();
    void on_treeView_activated(QModelIndex index);
    void on_resize();
    void on_resize_image(QImage * image, int col, int q);

};

#endif // MAINWINDOW_H
