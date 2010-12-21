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

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QtGui>


namespace Ui {
    class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;
    QSettings * settings;

    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:

    void on_radioButton_3_toggled(bool checked);
    void on_checkBox_2_toggled(bool checked);
    void on_checkBox_toggled(bool checked);
    void on_radioButton_2_toggled(bool checked);
    void on_radioButton_toggled(bool checked);
    void on_pushButton_clicked();
    void on_previewstepSlider_valueChanged(int value);
    void on_previewsizeSlider_valueChanged(int value);
    void on_rootfolderEdit_editingFinished();
    void on_savefolderBox_stateChanged(int );
    void on_toolButton_clicked();

signals:
    void close_window();
};

#endif // SETTINGSWINDOW_H
