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

#include "CFormWidget.h"
#include "CLogFontExDvWidget.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>

CLogFontExDvWidget::CLogFontExDvWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        AddWidgets({new CLogFontExWidget(),
                    new CDesignVectorWidget()}, pLayout);

        setLayout(pLayout);
}

CLogFontExDvWidget::~CLogFontExDvWidget()
{

}

QList<QStandardItem *> CLogFontExDvWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogFontExDv>");

        for (const CSharedWidget *pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}

CLogFontExWidget::CLogFontExWidget(QWidget *pParent) :
        CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("LogFontEx:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CLogFontWidget(),
                    new CFormWidget("FullName"),
                    new CFormWidget("Style"),
                    new CFormWidget("Script")}, pLayout);

        setLayout(pLayout);
}

CLogFontExWidget::~CLogFontExWidget()
{

}

CLogFontWidget::CLogFontWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("LogFont:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("Height",           "0"),
                    new CFormWidget("Width",            "0"),
                    new CFormWidget("Escapement",       "0"),
                    new CFormWidget("Orientation",      "0"),
                    new CFormWidget("Weight",           "0"),
                    new CFormWidget("Italic",           "0"),
                    new CFormWidget("Underline",        "0"),
                    new CFormWidget("StrikeOut",        "0"),
                    new CFormWidget("CharSet",          "0"),
                    new CFormWidget("OutPrecision",     "0"),
                    new CFormWidget("ClipPrecision",    "0"),
                    new CFormWidget("Quality",          "0"),
                    new CFormWidget("PitchAndFamily",   "0"),
                    new CFormWidget("Facename",         "0")}, pLayout);

        setLayout(pLayout);
}

CLogFontWidget::~CLogFontWidget()
{

}

QList<QStandardItem *> CLogFontWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogFont>");
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}

CDesignVectorWidget::CDesignVectorWidget(QWidget *pParent) :
        CSharedWidget(pParent),
        m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("DesignVector:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        QPushButton *pAddedButton = new QPushButton("Add value");

        connect(pAddedButton, &QPushButton::clicked, this, &CDesignVectorWidget::on_createValue_clicked);

        pLayout->addWidget(pName);
        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CDesignVectorWidget::~CDesignVectorWidget()
{

}

QList<QStandardItem *> CDesignVectorWidget::GetData() const
{
        QStandardItem *pMainItem = new QStandardItem("<DesignVector>");
        pMainItem->appendRow(new QStandardItem("<Signature>134248036</Signature>"));
        pMainItem->appendRows(CSharedWidget::GetData());
        return {pMainItem};
}

void CDesignVectorWidget::on_createValue_clicked()
{
        if (m_unCounter == 16)
                return;

        CFormWidget *pDataItem = new CFormWidget(QString("Value%1").arg(++m_unCounter));
        m_arWidgets.push_back(pDataItem);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pDataItem);
}

CLinkedUFISWidget::CLinkedUFISWidget(QWidget *pParent)
        : CSharedWidget(pParent),
          m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QPushButton *pCreateButton = new QPushButton("Add value");

        connect(pCreateButton, &QPushButton::clicked, this, &CLinkedUFISWidget::on_createValue_clicked);

        pLayout->addWidget(pCreateButton);

        setLayout(pLayout);
}

CLinkedUFISWidget::~CLinkedUFISWidget()
{

}

void CLinkedUFISWidget::on_createValue_clicked()
{
        CUniversalFontIdWidget *pUniversalFontIdWidget = new CUniversalFontIdWidget(++m_unCounter);

        m_arWidgets.push_back(pUniversalFontIdWidget);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pUniversalFontIdWidget);
}


CUniversalFontIdWidget::CUniversalFontIdWidget(unsigned int unIndex, QWidget *pParent)
        : CSharedWidget(pParent),
          m_unIndex(unIndex)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel(((m_unIndex > 0) ? QString("UniversalFontId%1:").arg(m_unIndex) : "UniversalFontId:"));
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        pLayout->addWidget(pName);

        AddWidgets({new CFormWidget("Checksum", "0"),
                    new CFormWidget("Index",    "0")}, pLayout);

        setLayout(pLayout);
}

CUniversalFontIdWidget::~CUniversalFontIdWidget()
{

}

QList<QStandardItem *> CUniversalFontIdWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem(((m_unIndex > 0) ? QString("<UniversalFontId%1>").arg(m_unIndex) : "<UniversalFontId>"));
        pItem->appendRows(CSharedWidget::GetData());
        return {pItem};
}
