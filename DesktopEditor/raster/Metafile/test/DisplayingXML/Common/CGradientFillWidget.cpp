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

        QPushButton *pCreateObjectsButton = new QPushButton("Create objects!");

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
