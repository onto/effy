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
#include <qpreviewscrollarea.h>

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
    QPreviewScrollArea * scrollarea;

protected:
    void changeEvent(QEvent *e);

private:
    QFileInfoList contentlist;
    QString currentpath;
    ViewWindow * viewwindow;
    SettingsWindow * settingswindow;
    Aboutwindow * aboutwindow;
    QFutureWatcher<QImage> * imagescaling;
    static int previewsize;
    Ui::MainWindow *ui;
    QList<QPushButton *> toolbarbuttons;
    QProgressBar * progressbar;
    QFileSystemModel * model;

    void setWidgetsConf();
    void setTreeConf();
    void setToolBarConf();
    void view();
    static QImage scaled(const QString &file);

public:
    void openDir(QString path);

private slots:
    void on_actionAbout_Effy_triggered();
    void on_actionAbout_Qt_triggered();
    void zoomin();
    void zoomout();
    void view_settings();
    void on_actionQuit_triggered();
    void on_treeView_activated(QModelIndex index);
    void resize();
    void update_settings();
    void open_photo(int id);
    void add_preview(int id);

    void resizeEvent (QResizeEvent * event );
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent * event);

signals:
    void widget_resize();
};

#endif // QT_NO_CONCURRENT

#endif // MAINWINDOW_H
