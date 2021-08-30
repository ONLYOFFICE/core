#include "CRecordCreator.h"
#include "ui_RecordCreator.h"

#include <QLabel>

CRecordCreator::CRecordCreator(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CRecordCreator),
        m_pMainWindow(NULL),
        m_pNewItem(NULL),
        m_nSelectedRecord(-1)
{
        ui->setupUi(this);
        this->setWindowTitle("Create");
}

CRecordCreator::~CRecordCreator()
{
        delete ui;
}

void CRecordCreator::SetMainWindow(MainWindow *pMainWindow)
{
        m_pMainWindow = pMainWindow;

        if(NULL != pMainWindow)
                pMainWindow->setEnabled(false);
}

QStandardItem* CRecordCreator::CreateRecord()
{
        this->show();

        if (exec() == QDialog::Accepted)
        {
                switch (m_nSelectedRecord)
                {
                        case -1: return NULL;
                        case 34: Convert_EMR_SELECTOBJECT(); break;
                }
        }
        else
        {
                qDebug() << "Cancel";
        }

        if (NULL != m_pMainWindow)
                m_pMainWindow->setEnabled(true);

        return m_pNewItem;
}

void CRecordCreator::on_selectButton_clicked()
{
        m_nSelectedRecord = ui->recordsBox->currentIndex();
        ClearData();

        switch (m_nSelectedRecord)
        {
                case 0:
                        break;

                case 34: Create_EMR_SELECTOBJECT_boxs(); break;
        }
}

void CRecordCreator::Create_EMR_SELECTOBJECT_boxs()
{
        this->setWindowTitle("Create: EMR_SELECTOBJECT");

        QHBoxLayout *pDataLayout = new QHBoxLayout;

        QLabel *pName           = new QLabel("Index");
        QTextEdit *pValue       = new QTextEdit("0");

        m_mData.insert("Index", pValue);

        AlignTextEdit(pValue);

        pDataLayout->addWidget(pName);
        pDataLayout->addWidget(pValue);

        ui->dataLayout->addLayout(pDataLayout);
}

void CRecordCreator::Convert_EMR_SELECTOBJECT()
{
        m_pNewItem = new QStandardItem("<EMR_SELECTOBJECT>");

        QStandardItem *pValue = new QStandardItem(QString("<Index>%1</Index>").arg(m_mData["Index"]->toPlainText()));
        m_pNewItem->appendRow(pValue);
}

void CRecordCreator::ClearData()
{
        while (QLayoutItem * pItem = ui->dataLayout->takeAt(0))
        {
                delete pItem->widget();
                delete pItem;
        }

        m_pNewItem = NULL;
        m_mData.clear();
}

void CRecordCreator::AlignTextEdit(QTextEdit *pTextEdit)
{
        if (NULL == pTextEdit)
                return;

        QFontMetrics oFontMEtrics(pTextEdit->font());
        pTextEdit->setFixedHeight(oFontMEtrics.height() + 10);
        pTextEdit->setStyleSheet("QTextEdit { vertical-align: middle }");
}

