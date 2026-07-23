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

#ifndef CMETAFILETREEWIDGET_H
#define CMETAFILETREEWIDGET_H

#include <QFrame>
#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>

#include "CTextEditDelegate.h"
#include "CMetafileTreeView.h"
#include "MainWindow.h"

class CMetafileTreeWidget : public QWidget
{
        Q_OBJECT
    public:
        CMetafileTreeWidget(QWidget *parent = NULL);
        ~CMetafileTreeWidget();

        void Clear();
        void SetMainWindow(MainWindow *pMainWindow);

        //QTreeView methods
        void setItemDelegate(CTextEditDelegate *pTextEditDelegate);
        void expandAll();
        void collapseAll();
        QAbstractItemModel* model();
        void setFrameShadow(QFrame::Shadow enShadow);
        void setSelectionBehavior(QAbstractItemView::SelectionBehavior unBehavior);
        void setAutoExpandDelay(int nDelay);
        void setRootIsDecorated(bool bShow);
        void setAnimated(bool bEnable);
        QHeaderView* header();

        //CMetafileTreeView methods
        void SetMetafile(const std::wstring& wsXmlFilePath);
        bool IsClearTree();
        void SetMode(bool bLightMode);
        void ClearTree();
        QMap<QString, unsigned int> GetStatistics();

        bool SaveInXmlFile(const std::wstring& wsSaveFilePath);

        void EditItem(QStandardItem* pStandardItem);

    protected:
        virtual void mouseReleaseEvent(QMouseEvent* event) override;

    private slots:
        void slotFindNext();
        void slotFindPrev();
        void slotRBClickedOnMetafileTree(QPoint oPoint);
        void slotDeleteItem(QStandardItem *pDeletedItem);

    private:
        void InsertRecord(QStandardItem *pParentItem, unsigned int unRow, bool bAfterRecord = true);

        QTextEdit *m_pFindText;
        QPushButton *m_pFindNextButton;
        QPushButton *m_pFindPrevButton;

        CMetafileTreeView *m_pMetafileTreeView;
        int m_nIndexSelectedItem;
        QStandardItem *m_pSelectedItem;

        MainWindow *m_pMainWindow;
};

#endif // CMETAFILETREEWIDGET_H
