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

#include "CGradientFillWidget.h"
#include "CRectangleWidget.h"
#include "Common.h"

#include <QPushButton>
#include <QLabel>

CGradientFillWidget::CGradientFillWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_pNVerWidget(NULL),
          m_pNTriWidget(NULL),
          m_pUlModeWidget(NULL),
          m_pDataLayout(NULL)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        m_pNVerWidget                   = new CFormWidget("nVer",   "0");
        m_pNTriWidget                   = new CFormWidget("nTri",   "0");
        m_pUlModeWidget                 = new CFormWidget("ulMode", "0");

        QPushButton *pCreateObjectsButton = new QPushButton("Создать объекты!");

        connect(pCreateObjectsButton, &QPushButton::clicked, this, &CGradientFillWidget::on_createObjects_clicked);

        AddWidgets({new CRectangleWidget("Bounds"),
                    m_pNVerWidget,
                    m_pNTriWidget,
                    m_pUlModeWidget}, pLayout);

        pLayout->addWidget(pCreateObjectsButton);

        m_pDataLayout = new QVBoxLayout();
        pLayout->addLayout(m_pDataLayout);

        setLayout(pLayout);
}

CGradientFillWidget::~CGradientFillWidget()
{

}

void CGradientFillWidget::on_createObjects_clicked()
{
        QString qsNVerValue = m_pNVerWidget->GetTextEditWidget()->toPlainText();
        QString qsNTriValue = m_pNTriWidget->GetTextEditWidget()->toPlainText();
        QString qsUlModeValue = m_pUlModeWidget->GetTextEditWidget()->toPlainText();

        if (qsNVerValue.isEmpty() || qsNTriValue.isEmpty() || qsUlModeValue.isEmpty())
                return;

        unsigned int unNVerValue   = qsNVerValue.toInt();
        unsigned int unNTriValue   = qsNTriValue.toInt();
        unsigned int unUlModeValue = qsUlModeValue.toInt();

        if (unNVerValue == 0 || unNTriValue == 0)
                return;

        ClearLayout(m_pDataLayout);

        while (m_arWidgets.size() > 4)
                m_arWidgets.pop_back();

        for (unsigned int unIndex = 0; unIndex < unNVerValue;)
        {
                CTriVertexWidget *pTriVertexWidget = new CTriVertexWidget(++unIndex);
                m_arWidgets.push_back(pTriVertexWidget);
                m_pDataLayout->addWidget(pTriVertexWidget);
        }

        for (unsigned int unIndex = 0; unIndex < unNTriValue;)
        {
                CSharedWidget *pGradientWidget;
                if (unUlModeValue == 2)
                        pGradientWidget = new CGradientTriangleWidget(++unIndex);
                else
                        pGradientWidget = new CGradientRectangleWidget(++unIndex);

                m_arWidgets.push_back(pGradientWidget);
                m_pDataLayout->addWidget(pGradientWidget);
        }

}

CTriVertexWidget::CTriVertexWidget(unsigned int unIndex, QWidget *pParent)
        : CSharedWidget(pParent),
          m_unIndex(unIndex)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel(((m_unIndex > 0) ? QString("TriVertex%1:").arg(m_unIndex) : "TriVertex:"));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("x",        "0"),
                    new CFormWidget("y",        "0"),
                    new CFormWidget("Red",      "0"),
                    new CFormWidget("Green",    "0"),
                    new CFormWidget("Blue",     "0"),
                    new CFormWidget("Alpha",    "0")}, pLayout);

        setLayout(pLayout);
}

CTriVertexWidget::~CTriVertexWidget()
{

}

QList<QStandardItem *> CTriVertexWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_unIndex > 0) ? QString("<TriVertex%1>").arg(m_unIndex) : "<TriVertex>"));
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}

CGradientRectangleWidget::CGradientRectangleWidget(unsigned int unIndex, QWidget *pParent)
        : CSharedWidget(pParent),
          m_unIndex(unIndex)
{
        QHBoxLayout *pLayout = new QHBoxLayout();

        QLabel *pName = new QLabel(((m_unIndex > 0) ? QString("GradientRectangle%1:").arg(m_unIndex) : "GradientRectangle:"));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("UpperLeft",  "0"),
                    new CFormWidget("LowerRight", "0")}, pLayout);

        setLayout(pLayout);
}

CGradientRectangleWidget::~CGradientRectangleWidget()
{

}

QList<QStandardItem *> CGradientRectangleWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_unIndex > 0) ? QString("<GradientRectangle%1>").arg(m_unIndex) : "<GradientRectangle>"));
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}

CGradientTriangleWidget::CGradientTriangleWidget(unsigned int unIndex, QWidget *pParent)
        : CSharedWidget(pParent),
          m_unIndex(unIndex)
{
        QHBoxLayout *pLayout = new QHBoxLayout();

        QLabel *pName = new QLabel(((m_unIndex > 0) ? QString("GradientTriangle%1:").arg(m_unIndex) : "GradientTriangle:"));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("Vertex1", "0"),
                    new CFormWidget("Vertex2", "0"),
                    new CFormWidget("Vertex3", "0")}, pLayout);

        setLayout(pLayout);
}

CGradientTriangleWidget::~CGradientTriangleWidget()
{

}

QList<QStandardItem *> CGradientTriangleWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_unIndex > 0) ? QString("<GradientTriangle%1>").arg(m_unIndex) : "<GradientTriangle>"));
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
