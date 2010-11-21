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

#include <QtGui>
#include "viewwindow.h"
#include "settingswindow.h"
#include "aboutwindow.h"
#include <qprolabel.h>

#ifndef QT_NO_CONCURRENT

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSettings * settings;

protected:
    void changeEvent(QEvent *e);

private:
    QFileInfoList contentlist;
    int columncount;
    int rowcount;
    QString currentpath;
    ViewWindow * viewwindow;
    SettingsWindow * settingswindow;
    Aboutwindow * aboutwindow;
    QList<QProLabel *> labels;
    QList<QLabel *> namelabels;
    QList<QVBoxLayout *> previewlayouts;
    QFutureWatcher<QImage> * imagescaling;
    static int previewsize;
    Ui::MainWindow *ui;
    QList<QPushButton *> toolbarbuttons;
    QLabel * pathlabel;
    QDirModel * model;

    void SetWidgetsConf();
    void SetTreeConf();
    void SetToolBarConf();
    void SaveSettings();
    void OpenPhoto(int id);
    void OpenDir(QString path);
    void View();
    void ShowPreview(int id);
    static QImage Scaled(const QString &file);
    void Update();

private slots:
    void on_actionAbout_Effy_triggered();
    void on_actionAbout_Qt_triggered();
    void gohome();
    void zoomin();
    void zoomout();
    void view_settings();
    void on_actionQuit_triggered();
    void on_treeView_activated(QModelIndex index);
    void resized_image(int q);
    void label_dbl_clicked(int id);
    void label_clicked(int id);
    void resize();
    void update_settings();

signals:
    void widget_resize();

protected:
    void resizeEvent (QResizeEvent * event );
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent * event);
};

#endif // QT_NO_CONCURRENT

#endif // MAINWINDOW_H
