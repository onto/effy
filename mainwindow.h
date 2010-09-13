#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

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
    Ui::MainWindow *ui;
    void SetTreeConf();
    void OpenDirToTab(QString path);


private slots:

    void on_treeView_activated(QModelIndex index);
    void on_tabWidget_tabCloseRequested(int index);
};

#endif // MAINWINDOW_H
