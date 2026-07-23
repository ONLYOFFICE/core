/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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


