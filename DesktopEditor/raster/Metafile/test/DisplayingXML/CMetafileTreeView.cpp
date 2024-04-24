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

#include "CMetafileTreeView.h"

#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <CTextEditDelegate.h>

#include "CEditItemWidget.h"

CMetafileTreeView::CMetafileTreeView(QWidget *parent) :
    QTreeView(parent)
{}

CMetafileTreeView::~CMetafileTreeView()
{}

void CMetafileTreeView::SetMetafile(const std::wstring &wsXmlFilePath)
{
        setModel(nullptr);

        if (wsXmlFilePath.empty())
                return;

        QFile *oXmlFile = new QFile(QString::fromStdWString(wsXmlFilePath));

        if (!oXmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
                QMessageBox::warning(this, "Warning", "Couldn't open XML file");
                oXmlFile->close();
                delete oXmlFile;
                return;
        }

        XmlUtils::CXmlNode oXmlRootNode;

        if (!oXmlRootNode.FromXmlFile(wsXmlFilePath) ||
            (oXmlRootNode.GetName() != L"EMF" &&
             oXmlRootNode.GetName() != L"WMF"))
        {
                oXmlFile->close();
                delete oXmlFile;
                return;
        }

        setHeaderHidden(true);

        QStandardItemModel *pStandardItemModel = new QStandardItemModel;
        QStandardItem *pStandardItem = new QStandardItem(QString::fromStdWString(L'<' + oXmlRootNode.GetName() + L'>'));
        pStandardItem->setEditable(false);

        ReadXmlNode(oXmlRootNode, pStandardItem, 1);

        pStandardItemModel->appendRow(pStandardItem);
        setModel(pStandardItemModel);

        oXmlFile->close();
        delete oXmlFile;
}

bool CMetafileTreeView::IsClear()
{
        return NULL == model();
}

void CMetafileTreeView::SetMode(bool bLightMode)
{
        if (dynamic_cast<CTextEditDelegate*>(itemDelegate()) != nullptr)
        {
                CTextEditDelegate *pTextEditDelegate = static_cast<CTextEditDelegate*>(itemDelegate());
                pTextEditDelegate->SetMode(bLightMode);
                pTextEditDelegate->updateEditorGeometry(this, QStyleOptionViewItem(), QModelIndex());
        }
}

void CMetafileTreeView::Clear()
{
        QAbstractItemModel *pModel = model();
        if (NULL != pModel)
        {
                delete pModel;
                setModel(NULL);
        }
}

bool CMetafileTreeView::SaveInXmlFile(const std::wstring& wsSaveFilePath)
{
        QStandardItemModel *pStandardItemModel = (QStandardItemModel*)model();
        QStandardItem *pRootNode = pStandardItemModel->item(0);

        XmlUtils::CXmlWriter oXmlWriter;

        oXmlWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

        WriteXmlNode(oXmlWriter, pRootNode);

        return oXmlWriter.SaveToFile(wsSaveFilePath);
}

void CMetafileTreeView::ReadXmlNode(XmlUtils::CXmlNode& oXmlNode, QStandardItem* oStandartItem, unsigned int unLevel)
{
    std::vector<XmlUtils::CXmlNode> oXmlChilds;

        if (oXmlNode.GetChilds(oXmlChilds))
        {
                if (unLevel == 2)
                        oStandartItem->setData("Record", 3);

                for (size_t i = 0; i < oXmlChilds.size(); ++i)
                {
                        XmlUtils::CXmlNode & oXmlChild = oXmlChilds[i];
                        if (XmlChild.IsValid())
                        {
                                QStandardItem *pStandardItem = new QStandardItem(QString("<%1>").arg(QString::fromStdWString(oXmlChild.GetName())));

                                if (unLevel > 1)
                                        pStandardItem->setEditable(false);

                                ReadXmlNode(oXmlChild, pStandardItem, unLevel + 1);
                                oStandartItem->appendRow(pStandardItem);
                        }
                }
        }
        else
        {
                QString qsName  = QString::fromWCharArray(oXmlNode.GetName().c_str());
                QString qsValue = QString::fromWCharArray(oXmlNode.GetText().c_str());

                if (qsName == "Buffer")
                {
                        oStandartItem->setData(true, 1);
                        QFontMetrics oFontMetrics(font()) ;
                        oStandartItem->setSizeHint(QSize(oFontMetrics.width("<Buffer>...</Buffer>"), oFontMetrics.height()));
                }

                if (!qsValue.isEmpty())
                        oStandartItem->setText(QString("<%1>%2</%1>").arg(qsName).arg(qsValue));
        }
}

std::wstring StringNormalization(std::wstring wsString)
{
    std::wstring wsText;
    for (wchar_t wChar : wsString)
        if (wChar == L'<')
               wsText += L"&lt;";
        else if (wChar == L'>')
               wsText += L"&gt;";
        else if (wChar == L'&')
               wsText += L"&amp;";
        else if (wChar == L'\'')
               wsText += L"&apos;";
        else if (wChar == L'"')
               wsText += L"&quot;";

        else wsText += wChar;
    return wsText;
}

void CMetafileTreeView::WriteXmlNode(XmlUtils::CXmlWriter &oXmlWriter, QStandardItem *oStandartItem)
{
        unsigned int unCountNodes = oStandartItem->rowCount();

        QString qsNodeText = oStandartItem->data(0).toString();

        unsigned int unFirstQuotationMark = qsNodeText.indexOf(L'>');
        unsigned int unLastSlash = qsNodeText.lastIndexOf(L'/');

        std::wstring wsName = qsNodeText.mid(1, ((unLastSlash > unFirstQuotationMark) ? (unFirstQuotationMark) : (unLastSlash)) - 1).toStdWString();

        if (oStandartItem->rowCount() == 0)
        {
                std::wstring wsValue;

                if (unLastSlash > unFirstQuotationMark)
                        wsValue = qsNodeText.mid(unFirstQuotationMark + 1, unLastSlash - unFirstQuotationMark - 2).toStdWString();

                oXmlWriter.WriteNode(wsName, StringNormalization(wsValue));

                return;
        }

        oXmlWriter.WriteNodeBegin(wsName);

        for (unsigned int unIndexNode = 0; unIndexNode < unCountNodes; ++unIndexNode)
        {
                QStandardItem *pNode = oStandartItem->child(unIndexNode);
                WriteXmlNode(oXmlWriter, pNode);
        }

        oXmlWriter.WriteNodeEnd(wsName);
}

void CMetafileTreeView::EditItem(QStandardItem *pStandardItem)
{
        if (NULL == pStandardItem)
                return;

        CEditItemWidget *pEditItemWidget = new CEditItemWidget;

        connect(pEditItemWidget, &CEditItemWidget::signalDeleteItem, this, &CMetafileTreeView::slotDeleteItem);

        pEditItemWidget->SetMainWindow((MainWindow*)parent()->parent()->parent()->parent());
        pEditItemWidget->SetItem(pStandardItem);
        pEditItemWidget->show();
}

void CMetafileTreeView::slotDeleteItem(QStandardItem *pDeletedItem)
{
        emit signalDeleteItem(pDeletedItem);
}

void CMetafileTreeView::mouseReleaseEvent(QMouseEvent *event)
{
        if (event->button() == Qt::RightButton)
                emit clickedRightMouseButton(event->pos());
        else
                QTreeView::mouseReleaseEvent(event);
}

