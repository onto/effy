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
    int previewsize;
    QFileInfoList contentlist;
    int columncount;

    Ui::MainWindow *ui;
    void SetTreeConf();
    void SetTableConf();
    void OpenPhoto(int id);
    void OpenDir(QString path);
    void ViewInTable(QFileInfoList content,int size,int col);


private slots:

    void on_tableWidget_cellActivated(int row, int column);
    void on_actionQuit_triggered();
    void on_treeView_activated(QModelIndex index);

};

#endif // MAINWINDOW_H
