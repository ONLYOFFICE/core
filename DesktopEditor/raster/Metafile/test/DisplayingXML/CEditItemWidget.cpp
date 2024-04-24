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

#include "CEditItemWidget.h"
#include "ui_EditItem.h"

#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>

#include <MainWindow.h>

CEditItemWidget::CEditItemWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CEditItemWidget),
        m_pMainWindow(NULL),
        m_pStandardItem(NULL),
        m_pContentLayout(NULL),
        m_pButtonsLayout(NULL)
{
        this->setWindowFlags(Qt::Window  | Qt::WindowCloseButtonHint);
        this->setWindowTitle("Edit");

        QVBoxLayout *pMainLayout    = new QVBoxLayout(this);
        QScrollArea *pScrollArea    = new QScrollArea();
        QWidget *pMainWidget        = new QWidget();
        m_pContentLayout            = new QVBoxLayout(pMainWidget);

        pMainLayout->addWidget(pScrollArea);
        pScrollArea->setWidget(pMainWidget);

        m_pButtonsLayout     = new QHBoxLayout;

        QPushButton *pSaveButton    = new QPushButton("Save");
        QPushButton *pCancelButton  = new QPushButton("Cancel");

        connect(pSaveButton,    &QPushButton::clicked, this, &CEditItemWidget::slotSaveButtonCliked);
        connect(pCancelButton,  &QPushButton::clicked, this, &CEditItemWidget::slotCancelButtonClicked);

        m_pButtonsLayout->addWidget(pSaveButton);
        m_pButtonsLayout->addWidget(pCancelButton);
        m_pButtonsLayout->setStretch(0, 1);

        pMainLayout->addLayout(m_pButtonsLayout);

        m_pContentLayout->setSizeConstraint(QLayout::SetFixedSize);

        ui->setupUi(this);
}

CEditItemWidget::~CEditItemWidget()
{
        delete ui;
}

void CEditItemWidget::SetMainWindow(MainWindow *pMainWidget)
{
        m_pMainWindow = pMainWidget;

        if(NULL != pMainWidget)
                pMainWidget->setEnabled(false);
}

void CEditItemWidget::SetItem(QStandardItem *pStandardItem)
{
        m_pStandardItem = pStandardItem;
        ParsingItem();
}

void CEditItemWidget::slotSaveButtonCliked()
{
        for (QStandardItem* oStandardItem : m_oBind)
        {
                const QTextEdit* pTextEdit = m_oBind.key(oStandardItem);

                unsigned int unFirstQuotationMark, unLastSlash;

                const QString qsStandardItemValue = oStandardItem->data(0).toString();

                unFirstQuotationMark = qsStandardItemValue.indexOf('>');
                unLastSlash = qsStandardItemValue.lastIndexOf('/');

                const QString qsName = qsStandardItemValue.mid(1, ((unLastSlash > unFirstQuotationMark) ? (unFirstQuotationMark) : (unLastSlash)) - 1);
                const QString qsValue = pTextEdit->toPlainText();

                if (qsValue.size() == 0)
                        oStandardItem->setText(QString("<%1/>").arg(qsName));
                else
                        oStandardItem->setText(QString("<%1>%2</%1>").arg(qsName).arg(qsValue));
        }

        if (!m_oBind.empty() && NULL != m_pMainWindow)
        {
                if (m_pMainWindow->SaveInXmlFile(L"Temp.xml") &&
                    m_pMainWindow->ConvertToEmf(L"Temp.xml"))
                        m_pMainWindow->DisplayingFile(L"TempFile.emf", false);
        }

        slotCancelButtonClicked();
}

void CEditItemWidget::slotCancelButtonClicked()
{
        if (NULL != m_pMainWindow)
                m_pMainWindow->setEnabled(true);

        this->close();
}

void CEditItemWidget::slotDeleteButtonClicked()
{
        DeleteItem(m_pStandardItem);
}

void CEditItemWidget::ParsingItem()
{
        if (NULL == m_pStandardItem)
                return;

        if (true == m_pStandardItem->data(3))
        {
                QPushButton *pDeleteButton = new QPushButton("Delete");

                connect(pDeleteButton, &QPushButton::clicked, this, &CEditItemWidget::slotDeleteButtonClicked);

                m_pButtonsLayout->addWidget(pDeleteButton);
        }

        ParsingAttachments(m_pStandardItem);
}

void CEditItemWidget::ParsingAttachments(QStandardItem *pStandardItem, unsigned int unLevel)
{
        const unsigned int unCountRow           = pStandardItem->rowCount();
        const QString qsStandardItemValue       = pStandardItem->data(0).toString();
        unsigned int unFirstQuotationMark, unLastSlash;

        unFirstQuotationMark = qsStandardItemValue.indexOf('>');
        unLastSlash = qsStandardItemValue.lastIndexOf('/');

        const QString qsName = qsStandardItemValue.mid(1, ((unLastSlash > unFirstQuotationMark) ? (unFirstQuotationMark) : (unLastSlash)) - 1);

        if (unCountRow == 0)
        {
                QHBoxLayout *pLayout = new QHBoxLayout;

                QString qsValue;

                if (unLastSlash > unFirstQuotationMark)
                      qsValue = qsStandardItemValue.mid(unFirstQuotationMark + 1, unLastSlash - unFirstQuotationMark - 2);

                QLabel *pNameLabel = new QLabel(qsName);
                QTextEdit *pValueEdit = new QTextEdit(qsValue);

                pNameLabel->setStyleSheet(QString("QLabel { margin-left: %1 }").arg((unLevel - 1) * 20));

                QFontMetrics oFontMEtrics(pValueEdit->font());
                pValueEdit->setFixedHeight(oFontMEtrics.height() + 10);
                pValueEdit->setStyleSheet("QTextEdit { vertical-align: middle }");
                pLayout->addWidget(pNameLabel);
                pLayout->addWidget(pValueEdit);

                m_oBind.insert(pValueEdit, pStandardItem);

                m_pContentLayout->addLayout(pLayout);
        }
        else
        {
                if (unLevel != 0)
                {
                        QLabel *pLabel = new QLabel(qsName);
                        pLabel->setStyleSheet(QString("QLabel { font-style: italic; margin-left: %1 }").arg((unLevel - 1) * 20));

                        m_pContentLayout->addWidget(pLabel);
                }
                else
                        this->setWindowTitle("Edit: " + qsName);

                for (unsigned int unNumberRow = 0; unNumberRow < unCountRow; ++unNumberRow)
                        ParsingAttachments(pStandardItem->child(unNumberRow), unLevel + 1);
        }
}

void CEditItemWidget::closeEvent(QCloseEvent *event)
{
        slotCancelButtonClicked();
}

void CEditItemWidget::DeleteItem(QStandardItem *pStandardItem)
{
        QStandardItem *pParent = pStandardItem->parent();

        if (NULL == pParent)
                return;

        emit signalDeleteItem(pStandardItem);

        pParent->removeRow(pStandardItem->index().row());

        if (NULL != m_pMainWindow)
        {
                if (m_pMainWindow->SaveInXmlFile(L"Temp.xml") &&
                    m_pMainWindow->ConvertToEmf(L"Temp.xml"))
                        m_pMainWindow->DisplayingFile(L"TempFile.emf", false);
        }

        slotCancelButtonClicked();
}
