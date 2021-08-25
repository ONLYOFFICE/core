#include "CEditItemWidget.h"
#include "ui_EditItem.h"

#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <QtDebug>

CEditItemWidget::CEditItemWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CEditItemWidget),
        m_pMainWindow(NULL),
        m_pStandardItem(NULL)
{
        this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        this->setWindowTitle("Edit");

        QVBoxLayout *pVerticalLayout    = new QVBoxLayout;
        QHBoxLayout *pButtonsLayout     = new QHBoxLayout;

        QPushButton *pSaveButton        = new QPushButton("Save");
        QPushButton *pCancelButton      = new QPushButton("Cancel");

        connect(pSaveButton, SIGNAL(clicked()), this, SLOT(on_Save_clicked()));
        connect(pCancelButton, SIGNAL(clicked()), this, SLOT(on_Cancel_clicked()));

        pButtonsLayout->addWidget(pSaveButton);
        pButtonsLayout->addWidget(pCancelButton);
//        pButtonsLayout->setStretch(0, 1);

        pVerticalLayout->addLayout(pButtonsLayout);
        this->setLayout(pVerticalLayout);

        ui->setupUi(this);
}

CEditItemWidget::~CEditItemWidget()
{
        delete ui;
}

void CEditItemWidget::SetMainWindow(QWidget *pMainWidget)
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

void CEditItemWidget::on_Save_clicked()
{

}

void CEditItemWidget::on_Cancel_clicked()
{
        if (NULL != m_pMainWindow)
                m_pMainWindow->setEnabled(true);

        this->close();
}

void CEditItemWidget::ParsingItem()
{
        if (NULL == m_pStandardItem)
                return;

        QHBoxLayout *pButtonsLayout = (QHBoxLayout*)layout()->takeAt(0);

        ParsingAttachments(m_pStandardItem);

        ((QVBoxLayout*)layout())->addLayout(pButtonsLayout);
}

void CEditItemWidget::ParsingAttachments(QStandardItem *pStandardItem, unsigned int unLevel)
{
        const unsigned int unCountRow           = pStandardItem->rowCount();
        const QString qsStandardItemValue       = pStandardItem->data(0).toString();

        QString qsName;
        unsigned int unFirstQuotationMark, unFirstSlash;

        unFirstQuotationMark = qsStandardItemValue.indexOf('>');
        unFirstSlash = qsStandardItemValue.indexOf('/');

        if (unCountRow == 0)
        {
                QHBoxLayout *pLayout = new QHBoxLayout;

                QString qsValue;

                if (unFirstSlash < unFirstQuotationMark)
                {

                }
                else
                {
                      qsName = qsStandardItemValue.mid(1, unFirstQuotationMark - 1);
                      qsValue = qsStandardItemValue.mid(unFirstQuotationMark + 1, unFirstSlash - unFirstQuotationMark - 2);
                }

                QLabel *pNameLabel = new QLabel(qsName);
                QTextEdit *pValueEdit = new QTextEdit(qsValue);

                pNameLabel->setStyleSheet(QString("QLabel { margin-left: %1 }").arg((unLevel - 1) * 20));

                QFontMetrics oFontMEtrics(pValueEdit->font());
                pValueEdit->setFixedHeight(oFontMEtrics.height() + 10);
                pValueEdit->setStyleSheet("QTextEdit { vertical-align: middle }");

                pLayout->addWidget(pNameLabel);
                pLayout->addWidget(pValueEdit);

                ((QVBoxLayout*)layout())->addLayout(pLayout);
        }
        else
        {
                qsName = qsStandardItemValue.mid(1, unFirstQuotationMark - 1);

                if (unLevel != 0)
                {
                        QLabel *pLabel = new QLabel(qsName);
                        pLabel->setStyleSheet(QString("QLabel { font-style: italic; margin-left: %1 }").arg((unLevel - 1) * 20));

                        layout()->addWidget(pLabel);
                }
                else
                        this->setWindowTitle("Edit: " + qsName);

                for (unsigned int unNumberRow = 0; unNumberRow < unCountRow; ++unNumberRow)
                        ParsingAttachments(pStandardItem->child(unNumberRow), unLevel + 1);
        }
}
