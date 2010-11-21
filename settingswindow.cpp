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

#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    this->move(qApp->desktop()->availableGeometry(this).center()-rect().center());

    this->setWindowIcon(QIcon("./icons/icon.png"));

    this->setWindowTitle("Settings");

    settings = new QSettings("effy","effy");

    ui->rootfolderEdit->setText(settings->value("root_folder").toString());

    ui->savefolderBox->setChecked(settings->value("save_last_folder").toBool());

    ui->previewsizeSlider->setValue(settings->value("preview_size").toInt());
    ui->previewsizeEdit->setText(settings->value("preview_size").toString());

    ui->previewstepSlider->setValue(settings->value("preview_step").toInt());
    ui->previewstepEdit->setText(settings->value("preview_step").toString());

    switch (settings->value("icon_size").toInt()) {
    case 16: ui->radioButton_3->setChecked(true); break;
    case 24: ui->radioButton->setChecked(true); break;
    case 32: ui->radioButton_2->setChecked(true); break;

    }

    ui->checkBox->setChecked(settings->value("show_mainwindow_toolbar").toBool());
    ui->checkBox_2->setChecked(settings->value("show_viewwindow_toolbar").toBool());
}

SettingsWindow::~SettingsWindow() {

    delete settings;
    delete ui;
}

void SettingsWindow::closeEvent(QCloseEvent *) {

    emit close_window();
}

void SettingsWindow::keyPressEvent(QKeyEvent *event) {

    if (event->modifiers() & Qt::ControlModifier) {

        switch (event->key()) {

        case (Qt::Key_Q) : {
            this->close();
            break;
        }
        }
    }
}

void SettingsWindow::on_toolButton_clicked() {

    ui->rootfolderEdit->setText(QFileDialog::getExistingDirectory(0,"Change Folder",""));
    settings->setValue("root_folder",ui->rootfolderEdit->text());
}

void SettingsWindow::on_savefolderBox_stateChanged(int) {

    settings->setValue("save_last_folder",ui->savefolderBox->isChecked());
}

void SettingsWindow::on_rootfolderEdit_editingFinished() {

    settings->setValue("root_folder",ui->rootfolderEdit->text());
}

void SettingsWindow::on_previewsizeSlider_valueChanged(int value) {

    settings->setValue("preview_size",value);
    ui->previewsizeEdit->setText(QString::number(value));
}
void SettingsWindow::on_previewstepSlider_valueChanged(int value) {

    settings->setValue("preview_step",value);
    ui->previewstepEdit->setText(QString::number(value));
}

void SettingsWindow::on_pushButton_clicked() {

    close();
}

void SettingsWindow::on_radioButton_toggled(bool) {

    settings->setValue("icon_size","24");
}

void SettingsWindow::on_radioButton_2_toggled(bool) {

    settings->setValue("icon_size","32");
}

void SettingsWindow::on_checkBox_toggled(bool) {

    settings->setValue("show_mainwindow_toolbar",ui->checkBox->isChecked());
}

void SettingsWindow::on_checkBox_2_toggled(bool) {

    settings->setValue("show_viewwindow_toolbar",ui->checkBox_2->isChecked());
}

void SettingsWindow::on_radioButton_3_toggled(bool) {

    settings->setValue("icon_size","16");
}
