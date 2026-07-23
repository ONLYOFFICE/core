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

#ifndef CEDITITEMWIDGET_H
#define CEDITITEMWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QBoxLayout>
#include <QStandardItem>

#include "MainWindow.h"

namespace Ui {
        class CEditItemWidget;
}

class CEditItemWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit CEditItemWidget(QWidget *parent = nullptr);
        ~CEditItemWidget();

        void SetMainWindow(MainWindow *pMainWindow);
        void SetItem(QStandardItem *pStandardItem);

    private slots:
        void slotSaveButtonCliked();
        void slotCancelButtonClicked();

        void slotDeleteButtonClicked();

    signals:
        void signalDeleteItem(QStandardItem *pDeletedItem);

    private:
        void ParsingItem();
        void ParsingAttachments(QStandardItem* pStandardItem, unsigned int unLevel = 0);

        void closeEvent(QCloseEvent *event) override;
        void DeleteItem(QStandardItem *pStandardItem);

        Ui::CEditItemWidget *ui;
        MainWindow *m_pMainWindow;
        QStandardItem *m_pStandardItem;
        QBoxLayout *m_pContentLayout;
        QBoxLayout *m_pButtonsLayout;

        QMap<QTextEdit*, QStandardItem*> m_oBind;
};

#endif // CEDITITEMWIDGET_H
