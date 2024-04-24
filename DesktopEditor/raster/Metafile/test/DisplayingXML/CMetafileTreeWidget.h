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

        //Методы QTreeView
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

        //Методы CMetafileTreeView
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
