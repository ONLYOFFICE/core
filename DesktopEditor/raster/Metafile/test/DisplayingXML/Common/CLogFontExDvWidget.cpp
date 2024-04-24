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

        QPushButton *pAddedButton = new QPushButton("Добавить значение");

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

        QPushButton *pCreateButton = new QPushButton("Добавить значение");

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
