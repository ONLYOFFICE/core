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
