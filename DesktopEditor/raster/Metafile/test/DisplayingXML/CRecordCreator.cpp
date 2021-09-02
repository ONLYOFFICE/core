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
                        case 0: Convert_Widgets_EMR_POLYBEZIER(); break;
                        case 1: Convert_Widgets_EMR_POLYGON();break;
                        case 2: Convert_Widgets_EMR_POLYLINE(); break;
                        case 3: Convert_Widgets_EMR_POLYBEZIERTO(); break;
                        case 4: Convert_Widgets_EMR_POLYLINETO(); break;
                        case 5: Convert_Widgets_EMR_POLYPOLYLINE(); break;
                        case 34: Convert_Widgets_EMR_SELECTOBJECT(); break;
                }
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
                case 0: Create_Widgets_EMR_POLYBEZIER(); break;
                case 1: Create_Widgets_EMR_POLYGON();break;
                case 2: Create_Widgets_EMR_POLYLINE(); break;
                case 3: Create_Widgets_EMR_POLYBEZIERTO(); break;
                case 4: Create_Widgets_EMR_POLYLINETO(); break;
                case 5: Create_Widgets_EMR_POLYPOLYLINE(); break;
                case 34: Create_Widgets_EMR_SELECTOBJECT(); break;
        }

        QPushButton *pOkButton =  ui->buttonBox->button(QDialogButtonBox::Ok);
        if (NULL != pOkButton)
                pOkButton->setEnabled(true);
}

void CRecordCreator::Create_Widgets_EMR_POLYBEZIER()
{
        this->setWindowTitle("Create: EMR_POLYBEZIER");

        CreateBoundsWidget();
        CreatePointsWidget();
}

void CRecordCreator::Convert_Widgets_EMR_POLYBEZIER()
{
        m_pNewItem = new QStandardItem("<EMR_POLYBEZIER>");

        ConvertWidgets();
}

void CRecordCreator::Create_Widgets_EMR_POLYGON()
{
        this->setWindowTitle("Create: EMR_POLYGON");

        CreateBoundsWidget();
        CreatePointsWidget();
}

void CRecordCreator::Convert_Widgets_EMR_POLYGON()
{
        m_pNewItem = new QStandardItem("<EMR_POLYGON>");

        ConvertWidgets();
}

void CRecordCreator::Create_Widgets_EMR_POLYLINE()
{
        this->setWindowTitle("Create: EMR_POLYLINE");

        CreateBoundsWidget();
        CreatePointsWidget();
}

void CRecordCreator::Convert_Widgets_EMR_POLYLINE()
{
        m_pNewItem = new QStandardItem("<EMR_POLYLINE>");

        ConvertWidgets();
}

void CRecordCreator::Create_Widgets_EMR_POLYBEZIERTO()
{
        this->setWindowTitle("Create: EMR_POLYBEZIERTO");

        CreateBoundsWidget();
        CreatePointsWidget();
}

void CRecordCreator::Convert_Widgets_EMR_POLYBEZIERTO()
{
        m_pNewItem = new QStandardItem("<EMR_POLYBEZIERTO>");

        ConvertWidgets();
}

void CRecordCreator::Create_Widgets_EMR_POLYLINETO()
{
        this->setWindowTitle("Create: EMR_POLYLINETO");

        CreateBoundsWidget();
        CreatePointsWidget();
}

void CRecordCreator::Convert_Widgets_EMR_POLYLINETO()
{
        m_pNewItem = new QStandardItem("<EMR_POLYLINETO>");

        ConvertWidgets();
}

void CRecordCreator::Create_Widgets_EMR_POLYPOLYLINE()
{
        this->setWindowTitle("Create: EMR_POLYPOLYLINE");

        CreateBoundsWidget();
        CreatePolyWidget();
}

void CRecordCreator::Convert_Widgets_EMR_POLYPOLYLINE()
{
        m_pNewItem = new QStandardItem("<EMR_POLYPOLYLINE>");

        ConvertWidgets();
}

void CRecordCreator::Create_Widgets_EMR_SELECTOBJECT()
{
        this->setWindowTitle("Create: EMR_SELECTOBJECT");

        CFormWidget *pFormWidget = new CFormWidget("Index", "0");
        ui->dataLayout->addWidget(pFormWidget);

        m_arWidgets.push_back(pFormWidget);
}

void CRecordCreator::Convert_Widgets_EMR_SELECTOBJECT()
{
        m_pNewItem = new QStandardItem("<EMR_SELECTOBJECT>");
        ConvertWidgets();
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

void CRecordCreator::ConvertWidgets()
{
        if (NULL == m_pNewItem)
                return;

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
