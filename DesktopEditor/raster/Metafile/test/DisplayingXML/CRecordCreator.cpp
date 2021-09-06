#include "CRecordCreator.h"
#include "ui_RecordCreator.h"

#include "Common/CLogBrushExWidget.h"
#include "Common/CBoundsWidget.h"
#include "Common/CPointsWidget.h"
#include "Common/CLogPenWidget.h"
#include "Common/CPointWidget.h"
#include "Common/CColorWidget.h"
#include "Common/CEmptyWidget.h"
#include "Common/CSizeWidget.h"
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
                        case 0: ConvertWidgets("EMR_POLYBEZIER");               break;
                        case 1: ConvertWidgets("EMR_POLYGON");                  break;
                        case 2: ConvertWidgets("EMR_POLYLINE");                 break;
                        case 3: ConvertWidgets("EMR_POLYBEZIERTO");             break;
                        case 4: ConvertWidgets("EMR_POLYLINETO");               break;
                        case 5: ConvertWidgets("EMR_POLYPOLYLINE");             break;
                        case 6: ConvertWidgets("EMR_POLYPOLYGON");              break;
                        case 7: ConvertWidgets("EMR_SETWINDOWEXTEX");           break;
                        case 8: ConvertWidgets("EMR_SETWINDOWORGEX");           break;
                        case 9: ConvertWidgets("EMR_SETVIEWPORTEXTEX");         break;
                        case 10: ConvertWidgets("EMR_SETVIEWPORTEXTEX");        break;
                        case 11: ConvertWidgets("EMR_SETBRUSHORGEX");           break;
                        case 12: ConvertWidgets("EMR_SETPIXELV");               break;
                        case 13: ConvertWidgets("EMR_SETMAPPERFLAGS");          break;
                        case 14: ConvertWidgets("EMR_SETMAPMODE");              break;
                        case 15: ConvertWidgets("EMR_SETBKMODE");               break;
                        case 16: ConvertWidgets("EMR_SETPOLYFILLMODE");         break;
                        case 17: ConvertWidgets("EMR_SETROP2");                 break;
                        case 18: ConvertWidgets("EMR_SETSTRETCHBLTMODE");       break;
                        case 19: ConvertWidgets("EMR_SETTEXTALIGN");            break;
                        case 20: ConvertWidgets("EMR_SETCOLORADJUSTMENT");      break;
                        case 21: ConvertWidgets("EMR_SETTEXTCOLOR");            break;
                        case 22: ConvertWidgets("EMR_SETBKCOLOR");              break;
                        case 23: ConvertWidgets("EMR_OFFSETCLIPRGN");           break;
                        case 24: ConvertWidgets("EMR_MOVETOEX");                break;
                        case 25: ConvertWidgets("EMR_SETMETARGN");              break;
                        case 26: ConvertWidgets("EMR_EXCLUDECLIPRECT");         break;
                        case 27: ConvertWidgets("EMR_INTERSECTCLIPRECT");       break;
                        case 28: ConvertWidgets("EMR_SCALEVIEWPORTEXTEX");      break;
                        case 29: ConvertWidgets("EMR_SCALEWINDOWEXTEX");        break;
                        case 30: ConvertWidgets("EMR_SAVEDC");                  break;
                        case 31: ConvertWidgets("EMR_RESTOREDC");               break;
                        case 32: ConvertWidgets("EMR_SETWORLDTRANSFORM");       break;
                        case 33: ConvertWidgets("EMR_MODIFYWORLDTRANSFORM");    break;
                        case 34: ConvertWidgets("EMR_SELECTOBJECT");            break;
                        case 35: ConvertWidgets("EMR_CREATEPEN");               break;
                        case 36: ConvertWidgets("EMR_CREATEBRUSHINDIRECT");     break;
                        case 37: ConvertWidgets("EMR_DELETEOBJECT");            break;
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
                case 0: CreatePointWidgets("EMR_POLYBEZIER");                           break;
                case 1: CreatePointWidgets("EMR_POLYGON");                              break;
                case 2: CreatePointWidgets("EMR_POLYLINE");                             break;
                case 3: CreatePointWidgets("EMR_POLYBEZIERTO");                         break;
                case 4: CreatePointWidgets("EMR_POLYLINETO");                           break;
                case 5: CreatePolyWidgets("EMR_POLYPOLYLINE");                          break;
                case 6: CreatePolyWidgets("EMR_POLYPOLYGON");                           break;
                case 7: CreateEXTEX("EMR_SETWINDOWEXTEX");                              break;
                case 8: CreateORGEX("EMR_SETWINDOWORGEX");                              break;
                case 9: CreateEXTEX("EMR_SETVIEWPORTEXTEX");                            break;
                case 10: CreateORGEX("EMR_SETVIEWPORTORGEX");                           break;
                case 11: CreateORGEX("EMR_SETBRUSHORGEX");                              break;
                case 12: CreateColorWidgets("EMR_SETPIXELV", true);                     break;
                case 13: CreateFormWidget("EMR_SETMAPPERFLAGS",   "Flags");             break;
                case 14: CreateFormWidget("EMR_SETMAPMODE",       "MapMode");           break;
                case 15: CreateFormWidget("EMR_SETBKMODE",        "BackgroundMode");    break;
                case 16: CreateFormWidget("EMR_SETPOLYFILLMODE",  "PolygonFillMode");   break;
                case 17: CreateFormWidget("EMR_SETROP2",          "ROP2Mode");          break;
                case 18: CreateFormWidget("EMR_SETSTRETCHBLTMODE", "StretchMode");      break;
                case 19: CreateFormWidget("EMR_SETTEXTALIGN",      "TextAlignmentMode");break;
                case 20: Create_Widgets_EMR_SETCOLORADJUSTMENT();                       break;
                case 21: CreateColorWidgets("EMR_SETTEXTCOLOR");                        break;
                case 22: CreateColorWidgets("EMR_SETBKCOLOR");                          break;
                case 23: CreatePointWidgets("EMR_OFFSETCLIPRGN");                       break;
                case 24: CreatePointWidgets("EMR_MOVETOEX");                            break;
                case 25: CreateEmptyWidgets("EMR_SETMETARGN");                          break;
                case 26: CreateRectangleWidgets("EMR_EXCLUDECLIPRECT", "Clip");         break;
                case 27: CreateRectangleWidgets("EMR_INTERSECTCLIPRECT", "Clip");       break;
                case 28: CreateScaleWidgets("EMR_SCALEVIEWPORTEXTEX");                  break;
                case 29: CreateScaleWidgets("EMR_SCALEWINDOWEXTEX");                    break;
                case 30: CreateEmptyWidgets("EMR_SAVEDC");                              break;
                case 31: CreateFormWidget("EMR_RESTOREDC", "SavedDC");                  break;
                case 32: Create_Widgets_EMR_SETWORLDTRANSFORM();                        break;
                case 33: Create_Widgets_EMR_MODIFYWORLDTRANSFORM();                     break;
                case 34: CreateFormWidget("EMR_SELECTOBJECT",      "Index");            break;
                case 35: Create_Widgets_EMR_CREATEPEN();                                break;
                case 36: Create_Widgets_EMR_CREATEBRUSHINDIRECT();                      break;
                case 37: CreateFormWidget("EMR_DELETEOBJECT", "ihObject");              break;
        }

        QPushButton *pOkButton =  ui->buttonBox->button(QDialogButtonBox::Ok);
        if (NULL != pOkButton)
                pOkButton->setEnabled(true);
}

void CRecordCreator::CreatePointWidgets(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CreateRectangleWidget("Bounds");
        CreatePointsWidget();
}

void CRecordCreator::CreateFormWidget(const QString& qsName, const QString& qsFormName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CFormWidget *pFormWidget = new CFormWidget(qsFormName, "0");
        ui->dataLayout->addWidget(pFormWidget);

        m_arWidgets.push_back(pFormWidget);
}

void CRecordCreator::CreateColorWidgets(const QString& qsName, bool bBounds)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        if (bBounds)
                CreateRectangleWidget("Bounds");

        CreateColorWidget();
}

void CRecordCreator::CreateEmptyWidgets(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CEmptyWidget *pEmptyWidget = new CEmptyWidget(qsName);

        QLabel *pLabel = new QLabel("Запись не содержит параметров");
        ui->dataLayout->addWidget(pLabel);

        m_arWidgets.push_back(pEmptyWidget);
}

void CRecordCreator::CreateRectangleWidgets(const QString &qsName, const QString &qsReactangleName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CReactangleWidget *pRectangleWidget = new CReactangleWidget(qsReactangleName);
        ui->dataLayout->addWidget(pRectangleWidget);

        m_arWidgets.push_back(pRectangleWidget);
}

void CRecordCreator::CreateScaleWidgets(const QString &qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CFormWidget *pXNumWidget    = new CFormWidget("xNum",   "1");
        CFormWidget *pXDenomWidget  = new CFormWidget("xDenom", "1");
        CFormWidget *pYNumWidget    = new CFormWidget("YNum",   "1");
        CFormWidget *pYDenomWidget  = new CFormWidget("YDenom", "1");

        m_arWidgets.push_back(pXNumWidget);
        m_arWidgets.push_back(pXDenomWidget);
        m_arWidgets.push_back(pYNumWidget);
        m_arWidgets.push_back(pYDenomWidget);

        QGridLayout *pLayout = new QGridLayout;

        pLayout->addWidget(pXNumWidget, 0, 0);
        pLayout->addWidget(new QLabel("≠0"), 0, 1);

        pLayout->addWidget(pXDenomWidget, 1, 0);
        pLayout->addWidget(new QLabel("≠0"), 1, 1);

        pLayout->addWidget(pYNumWidget, 2, 0);
        pLayout->addWidget(new QLabel("≠0"), 2, 1);

        pLayout->addWidget(pYDenomWidget, 3, 0);
        pLayout->addWidget(new QLabel("≠0"), 3, 1);

        ui->dataLayout->addLayout(pLayout);
}

void CRecordCreator::Create_Widgets_EMR_SETCOLORADJUSTMENT()
{
        this->setWindowTitle("Create: EMR_SETCOLORADJUSTMENT");

        CFormWidget *pSizeWidget            = new CFormWidget("Size",                   "0");
        CFormWidget *pValuesWidget          = new CFormWidget("Values",                 "0");
        CFormWidget *pIndexWidget           = new CFormWidget("IlluminantIndex",        "0");
        CFormWidget *pRGammaWidget          = new CFormWidget("RedGamma",               "0");
        CFormWidget *pGGammaWidget          = new CFormWidget("GreenGamma",             "0");
        CFormWidget *pBGammaWidget          = new CFormWidget("BlueGamma",              "0");
        CFormWidget *pReferenceBWidget      = new CFormWidget("ReferenceBlack",         "0");
        CFormWidget *pReferenceWWidget      = new CFormWidget("ReferenceWhite",         "0");
        CFormWidget *pConstrastWidget       = new CFormWidget("Constrast",              "0");
        CFormWidget *pBrightnessWidget      = new CFormWidget("Brightness",             "0");
        CFormWidget *pColorfulnessWidget    = new CFormWidget("Colorfulness",           "0");
        CFormWidget *pRedGreenTintWidget    = new CFormWidget("RedGreenTint",           "0");

        ui->dataLayout->addWidget(pSizeWidget);
        ui->dataLayout->addWidget(pValuesWidget);
        ui->dataLayout->addWidget(pIndexWidget);
        ui->dataLayout->addWidget(pRGammaWidget);
        ui->dataLayout->addWidget(pGGammaWidget);
        ui->dataLayout->addWidget(pBGammaWidget);
        ui->dataLayout->addWidget(pReferenceBWidget);
        ui->dataLayout->addWidget(pReferenceWWidget);
        ui->dataLayout->addWidget(pConstrastWidget);
        ui->dataLayout->addWidget(pBrightnessWidget);
        ui->dataLayout->addWidget(pColorfulnessWidget);
        ui->dataLayout->addWidget(pRedGreenTintWidget);

        m_arWidgets.push_back(pSizeWidget);
        m_arWidgets.push_back(pValuesWidget);
        m_arWidgets.push_back(pIndexWidget);
        m_arWidgets.push_back(pRGammaWidget);
        m_arWidgets.push_back(pGGammaWidget);
        m_arWidgets.push_back(pBGammaWidget);
        m_arWidgets.push_back(pReferenceBWidget);
        m_arWidgets.push_back(pReferenceWWidget);
        m_arWidgets.push_back(pConstrastWidget);
        m_arWidgets.push_back(pBrightnessWidget);
        m_arWidgets.push_back(pColorfulnessWidget);
        m_arWidgets.push_back(pRedGreenTintWidget);
}

void CRecordCreator::Create_Widgets_EMR_SETWORLDTRANSFORM()
{
        this->setWindowTitle("Create: EMR_SETWORLDTRANSFORM");

        CreateXForm();
}

void CRecordCreator::Create_Widgets_EMR_MODIFYWORLDTRANSFORM()
{
        this->setWindowTitle("Create: EMR_MODIFYWORLDTRANSFORM");

        CreateXForm();

        CFormWidget *pMode = new CFormWidget("ModifyWorldTransformMode", "1");
        ui->dataLayout->addWidget(pMode);

        m_arWidgets.push_back(pMode);
}

void CRecordCreator::Create_Widgets_EMR_CREATEPEN()
{
        this->setWindowTitle("Create: EMR_CREATEPEN");

        CFormWidget *pIhPen = new CFormWidget("ihPen", "1");
        CLogPenWidget *pLogPenWidget = new CLogPenWidget();

        ui->dataLayout->addWidget(pIhPen);
        ui->dataLayout->addWidget(pLogPenWidget);

        m_arWidgets.push_back(pIhPen);
        m_arWidgets.push_back(pLogPenWidget);
}

void CRecordCreator::Create_Widgets_EMR_CREATEBRUSHINDIRECT()
{
        this->setWindowTitle("Create: EMR_CREATEBRUSHINDIRECT");

        CFormWidget *pIhBrush  = new CFormWidget("ihBrush", "1");
        CLogBrushExWidget *pLogBrushExWidget = new CLogBrushExWidget();

        ui->dataLayout->addWidget(pIhBrush);
        ui->dataLayout->addWidget(pLogBrushExWidget);

        m_arWidgets.push_back(pIhBrush);
        m_arWidgets.push_back(pLogBrushExWidget);
}

void CRecordCreator::CreatePolyWidgets(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CreateRectangleWidget("Bounds");
        CreatePolyWidget();
}

void CRecordCreator::CreateRectangleWidget(const QString& qsName)
{
        CReactangleWidget *pRectanleWidget = new CReactangleWidget(qsName);
        ui->dataLayout->addWidget(pRectanleWidget);

        m_arWidgets.push_back(pRectanleWidget);
}

void CRecordCreator::CreatePointsWidget()
{
        CPointsWidget *pPointsWidget = new CPointsWidget();
        ui->dataLayout->addWidget(pPointsWidget);

        m_arWidgets.push_back(pPointsWidget);
}

void CRecordCreator::CreateColorWidget()
{
        CColorWidget *pColorWidget = new CColorWidget("Color");
        ui->dataLayout->addWidget(pColorWidget);

        m_arWidgets.push_back(pColorWidget);
}

void CRecordCreator::CreatePolyWidget()
{
        CPolyWidget *pPolyWidget = new CPolyWidget;
        ui->dataLayout->addWidget(pPolyWidget);

        m_arWidgets.push_back(pPolyWidget);
}

void CRecordCreator::CreateXForm()
{
        CFormWidget *pM11 = new CFormWidget("M11", "0");
        CFormWidget *pM12 = new CFormWidget("M12", "0");
        CFormWidget *pM21 = new CFormWidget("M21", "0");
        CFormWidget *pM22 = new CFormWidget("M22", "0");
        CFormWidget *pDx  = new CFormWidget("Dx",  "0");
        CFormWidget *pDy  = new CFormWidget("Dy",  "0");

        m_arWidgets.push_back(pM11);
        m_arWidgets.push_back(pM12);
        m_arWidgets.push_back(pM21);
        m_arWidgets.push_back(pM22);
        m_arWidgets.push_back(pDx);
        m_arWidgets.push_back(pDy);

        ui->dataLayout->addWidget(pM11);
        ui->dataLayout->addWidget(pM12);
        ui->dataLayout->addWidget(pM21);
        ui->dataLayout->addWidget(pM22);
        ui->dataLayout->addWidget(pDx);
        ui->dataLayout->addWidget(pDy);
}

void CRecordCreator::CreateEXTEX(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CSizeWidget *pSizeWidget = new CSizeWidget;
        ui->dataLayout->addWidget(pSizeWidget);

        m_arWidgets.push_back(pSizeWidget);
}

void CRecordCreator::CreateORGEX(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        CPointWidget *pPointWidget = new CPointWidget("Extent");
        ui->dataLayout->addWidget(pPointWidget);

        m_arWidgets.push_back(pPointWidget);
}

void CRecordCreator::ConvertWidgets(const QString& qsName)
{
        if (qsName.isEmpty())
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
