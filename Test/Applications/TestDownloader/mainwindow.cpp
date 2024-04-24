/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <functional>

#define URL L"http://download.onlyoffice.com/install/desktop/editors/windows/onlyoffice/updates/editors_update_x64.exe"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    downloader = new Downloader(URL, false);
    downloader->SetEvent_OnComplete(std::bind(&MainWindow::onLoadUpdateFinished, this, std::placeholders::_1));
    downloader->SetEvent_OnProgress(std::bind(&MainWindow::onProgress, this, std::placeholders::_1));
}

MainWindow::~MainWindow()
{
    delete downloader;
    delete ui;
}

void MainWindow::onProgress(const int percent)
{
    qDebug() << "Precent... " << percent;
    QMetaObject::invokeMethod(this, "onProgressSlot", Qt::QueuedConnection, Q_ARG(int, percent));
}

void MainWindow::onProgressSlot(int percent)
{
    ui->textEdit->setText(QString::number(percent));
    ui->progressBar->setValue(percent);
}

void MainWindow::onLoadUpdateFinished(int error)
{
    qDebug() << "Loading finished...";
    QMetaObject::invokeMethod(this, "onLoadFinishedSlot", Qt::QueuedConnection, Q_ARG(int, error));
}

void MainWindow::onLoadFinishedSlot(int error)
{
    if (error == 0) {
        ui->textEdit->setText("Loading finished...");
    } else {
        qDebug() << "Download error: " << error;
        ui->textEdit->setText("Download error");
    }

    const QString path = QString::fromStdWString(downloader->GetFilePath());
    if (QDir().exists(path)) QDir().remove(path);
}

void MainWindow::on_buttonStart_clicked()
{
    qDebug() << "Start loading...";
    ui->progressBar->setValue(0);
//    const QString tmp_file = QDir::tempPath() + QDir::separator() + QString("temp.tmp");
    const QString tmp_file = QString("C:\\Users\\Vladimir.Gorshenkov\\NewControl\\core\\Test\\Applications\\TestDownloader\\temp.tmp");
    ui->textEdit->setText(QString("Start loading: %1 to %2").arg(QString::fromStdWString(URL), tmp_file));
    downloader->SetFilePath(tmp_file.toStdWString());
    downloader->Start(0);
}

void MainWindow::on_buttonCancel_clicked()
{
    qDebug() << "Loading cancel...";
    ui->progressBar->setValue(0);
    ui->textEdit->setText("Loading cancel...");
    const QString path = QString::fromStdWString(downloader->GetFilePath());
    downloader->Cancel();
//    if (QDir().exists(path)) QDir().remove(path);

    qDebug() << "Stop finished";
}


