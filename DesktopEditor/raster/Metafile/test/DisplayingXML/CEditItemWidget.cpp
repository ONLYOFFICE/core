#include "CEditItemWidget.h"
#include "ui_EditItem.h"

#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

CEditItemWidget::CEditItemWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CEditItemWidget),
        m_pMainWindow(NULL),
        m_pStandardItem(NULL)
{
        this->setWindowFlags(Qt::Window  | Qt::WindowCloseButtonHint);
        this->setWindowTitle("Edit");

        QVBoxLayout *pVerticalLayout    = new QVBoxLayout;
        QHBoxLayout *pButtonsLayout     = new QHBoxLayout;

        QPushButton *pSaveButton        = new QPushButton("Save");
        QPushButton *pCancelButton      = new QPushButton("Cancel");

        connect(pSaveButton,    SIGNAL(clicked()), this, SLOT(on_Save_clicked()));
        connect(pCancelButton,  SIGNAL(clicked()), this, SLOT(on_Cancel_clicked()));

        pButtonsLayout->addWidget(pSaveButton);
        pButtonsLayout->addWidget(pCancelButton);
        pButtonsLayout->setStretch(0, 1);

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
        for (QStandardItem* oStandardItem : m_oBind)
        {
                const QTextEdit* pTextEdit = m_oBind.key(oStandardItem);

                unsigned int unFirstQuotationMark, unFirstSlash;

                const QString qsStandardItemValue = oStandardItem->data(0).toString();

                unFirstQuotationMark = qsStandardItemValue.indexOf('>');
                unFirstSlash = qsStandardItemValue.indexOf('/');

                const QString qsName = qsStandardItemValue.mid(1, ((unFirstSlash > unFirstQuotationMark) ? (unFirstQuotationMark) : (unFirstSlash)) - 1);
                const QString qsValue = pTextEdit->toPlainText();

                if (qsValue.size() == 0)
                        oStandardItem->setText(QString("<%1/>").arg(qsName));
                else
                        oStandardItem->setText(QString("<%1>%2</%1>").arg(qsName, qsValue));
        }

        on_Cancel_clicked();
}

void CEditItemWidget::on_Cancel_clicked()
{
        if (NULL != m_pMainWindow)
                m_pMainWindow->setEnabled(true);

        this->close();
}

void CEditItemWidget::on_DeleteItem_clicked()
{
        QStandardItem *pParent = m_pStandardItem->parent();

        pParent->removeRow(m_pStandardItem->index().row());

        on_Cancel_clicked();
}

void CEditItemWidget::ParsingItem()
{
        if (NULL == m_pStandardItem)
                return;

        QHBoxLayout *pButtonsLayout = (QHBoxLayout*)layout()->takeAt(0);

        ParsingAttachments(m_pStandardItem);

        if (m_pStandardItem->parent()->data(0).toString() == "<EMF>")
        {
                QPushButton *pDeleteButton = new QPushButton("Delete");

                connect(pDeleteButton,  SIGNAL(clicked()), this, SLOT(on_DeleteItem_clicked()));

                pButtonsLayout->addWidget(pDeleteButton);
        }

        ((QVBoxLayout*)layout())->addLayout(pButtonsLayout);
}

void CEditItemWidget::ParsingAttachments(QStandardItem *pStandardItem, unsigned int unLevel)
{
        const unsigned int unCountRow           = pStandardItem->rowCount();
        const QString qsStandardItemValue       = pStandardItem->data(0).toString();
        unsigned int unFirstQuotationMark, unFirstSlash;

        unFirstQuotationMark = qsStandardItemValue.indexOf('>');
        unFirstSlash = qsStandardItemValue.indexOf('/');

        const QString qsName = qsStandardItemValue.mid(1, ((unFirstSlash > unFirstQuotationMark) ? (unFirstQuotationMark) : (unFirstSlash)) - 1);

        if (unCountRow == 0)
        {
                QHBoxLayout *pLayout = new QHBoxLayout;

                QString qsValue;

                if (unFirstSlash > unFirstQuotationMark)
                      qsValue = qsStandardItemValue.mid(unFirstQuotationMark + 1, unFirstSlash - unFirstQuotationMark - 2);

                QLabel *pNameLabel = new QLabel(qsName);
                QTextEdit *pValueEdit = new QTextEdit(qsValue);

                pNameLabel->setStyleSheet(QString("QLabel { margin-left: %1 }").arg((unLevel - 1) * 20));

                QFontMetrics oFontMEtrics(pValueEdit->font());
                pValueEdit->setFixedHeight(oFontMEtrics.height() + 10);
                pValueEdit->setStyleSheet("QTextEdit { vertical-align: middle }");

                pLayout->addWidget(pNameLabel);
                pLayout->addWidget(pValueEdit);

                m_oBind.insert(pValueEdit, pStandardItem);

                ((QVBoxLayout*)layout())->addLayout(pLayout);
        }
        else
        {
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

void CEditItemWidget::closeEvent(QCloseEvent *event)
{
        on_Cancel_clicked();
}
