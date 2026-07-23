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

#include "CTextEditDelegate.h"
#include "CXMLHighlighter.h"
#include "CItemWidget.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPainter>

#include <QComboBox>

CTextEditDelegate::CTextEditDelegate(QWidget *parent) :
        QStyledItemDelegate(parent),
        pFont(NULL)
{
        bLightMode = false;
}

CTextEditDelegate::~CTextEditDelegate()
{
        ClearFont();
}

void CTextEditDelegate::SetFont(QFont *pFont)
{
        ClearFont();
        this->pFont = pFont;
}

void CTextEditDelegate::SetMode(bool bIsLightMode)
{
        bLightMode = bIsLightMode;
}

QWidget *CTextEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        CItemWidget *pItemWidget = new CItemWidget(parent);

        if (NULL != pFont)
                pItemWidget->SetFont(*pFont);

        return pItemWidget;
}

void CTextEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
        QString qsValue = index.model()->data(index, Qt::EditRole).toString();
        CItemWidget* pItemWidget = static_cast<CItemWidget*>(editor);
        pItemWidget->SetText(qsValue);
}

void CTextEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
        CItemWidget* pItemWidget = static_cast<CItemWidget*>(editor);
        model->setData(index, pItemWidget->GetText(), Qt::EditRole);
}

void CTextEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        editor->setGeometry(option.rect);
}

void CTextEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        QString qsValue;

        if (index.data(1) == true)
                qsValue = "<Buffer>...<Buffer>";
        else
                qsValue = index.data(0).toString();

        if (index.data(5) == true)
        {
                QRect oRect(option.rect);

                oRect.setTop(oRect.top() + 3);
                oRect.setBottom(oRect.bottom() - 3);

                QPen oOldPen = painter->pen();

                QPen oNewPen = oOldPen;
                oNewPen.setStyle(Qt::DashLine);

                painter->setPen(oNewPen);

                painter->drawRect(oRect);

                painter->setPen(oOldPen);
        }

        if (bLightMode)
                painter->drawText(option.rect, qsValue);
        else
        {
                QTextDocument oEmptyDocument;
                CXMLHighlighter oXMLHighlighter(&oEmptyDocument);
                oXMLHighlighter.PaintByRegex(qsValue, painter, option.rect, pFont);
        }
}

void CTextEditDelegate::ClearFont()
{
        if (NULL != pFont)
                delete pFont;
}
