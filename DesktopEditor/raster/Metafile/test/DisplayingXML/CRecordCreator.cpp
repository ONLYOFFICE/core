#include "CRecordCreator.h"
#include "ui_RecordCreator.h"

#include "Common/CBoundsWidget.h"
#include "Common/CPointsWidget.h"
#include "Common/CPolyWidget.h"
#include "Common/CFormWidget.h"
#include "Common/Common.h"

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

        QPushButton *pOkButton =  ui->buttonBox->button(QDialogButtonBox::Ok);
        if (NULL != pOkButton)
        {
                 pOkButton->setEnabled(false);
                 connect(pOkButton, &QPushButton::clicked, this, &QDialog::accept);
        }
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
                        case -1: break;
                        case 0: ConvertWidgets("EMR_POLYBEZIER");       break;
                        case 1: ConvertWidgets("EMR_POLYGON");          break;
                        case 2: ConvertWidgets("EMR_POLYLINE");         break;
                        case 3: ConvertWidgets("EMR_POLYBEZIERTO");     break;
                        case 4: ConvertWidgets("EMR_POLYLINETO");       break;
                        case 5: ConvertWidgets("EMR_POLYPOLYLINE");     break;
                        case 6: ConvertWidgets("EMR_POLYPOLYGON");      break;
                        case 34: ConvertWidgets("EMR_SELECTOBJECT");    break;
                };
        }

        Close();

        return m_pNewItem;
}

void CRecordCreator::on_selectButton_clicked()
{
        m_nSelectedRecord = ui->recordsBox->currentIndex();
        ClearData();

        switch (m_nSelectedRecord)
        {
                case 0: CreatePointWidgets("EMR_POLYBEZIER");   break;
                case 1: CreatePointWidgets("EMR_POLYGON");      break;
                case 2: CreatePointWidgets("EMR_POLYLINE");     break;
                case 3: CreatePointWidgets("EMR_POLYBEZIERTO"); break;
                case 4: CreatePointWidgets("EMR_POLYLINETO");   break;
                case 5: CreatePolyWidgets("EMR_POLYPOLYLINE");  break;
                case 6: CreatePolyWidgets("EMR_POLYPOLYGON");  break;
                case 34: Create_Widgets_EMR_SELECTOBJECT();     break;
        }

        QPushButton *pOkButton =  ui->buttonBox->button(QDialogButtonBox::Ok);
        if (NULL != pOkButton)
                pOkButton->setEnabled(true);
}

void CRecordCreator::CreatePointWidgets(QString qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CreateBoundsWidget();
        CreatePointsWidget();
}

void CRecordCreator::Create_Widgets_EMR_SELECTOBJECT()
{
        this->setWindowTitle("Create: EMR_SELECTOBJECT");

        CFormWidget *pFormWidget = new CFormWidget("Index", "0");
        ui->dataLayout->addWidget(pFormWidget);

        m_arWidgets.push_back(pFormWidget);
}

void CRecordCreator::CreatePolyWidgets(QString qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CreateBoundsWidget();
        CreatePolyWidget();
}

void CRecordCreator::CreateBoundsWidget()
{
        CBoundsWidget *pBoundsWiget = new CBoundsWidget;
        ui->dataLayout->addWidget(pBoundsWiget);

        m_arWidgets.push_back(pBoundsWiget);
}

void CRecordCreator::CreatePointsWidget()
{
        CPointsWidget *pPointsWidget = new CPointsWidget;
        ui->dataLayout->addWidget(pPointsWidget);

        m_arWidgets.push_back(pPointsWidget);
}

void CRecordCreator::CreatePolyWidget()
{
        CPolyWidget *pPolyWidget = new CPolyWidget;
        ui->dataLayout->addWidget(pPolyWidget);

        m_arWidgets.push_back(pPolyWidget);
}

void CRecordCreator::ConvertWidgets(QString qsName)
{
        if (qsName.size() == 0)
                return;

        m_pNewItem = new QStandardItem(QString("<%1>").arg(qsName));

        for (const CSharedWidget* pWidget : m_arWidgets)
                m_pNewItem->appendRows(pWidget->GetData());
}

void CRecordCreator::ClearData()
{
        ClearLayout(ui->dataLayout);

        m_pNewItem = NULL;
        m_arWidgets.clear();
}

void CRecordCreator::Close()
{
        if (NULL != m_pMainWindow)
                m_pMainWindow->setEnabled(true);

        close();
}
