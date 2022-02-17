#include "CRecordCreator.h"
#include "ui_RecordCreator.h"

#include "Common/CPixelFormatDescriptorWidget.h"
#include "Common/CExtSelectClipRgnWidget.h"
#include "Common/CPaletteEntriesWidget.h"
#include "Common/CLogColorSpaceWidget.h"
#include "Common/CGradientFillWidget.h"
#include "Common/CSmallTextOutWidget.h"
#include "Common/CPolyTextOutWidget.h"
#include "Common/CLogFontExDvWidget.h"
#include "Common/CLogBrushExWidget.h"
#include "Common/CLogPaletteWidget.h"
#include "Common/CExtTextOutWidget.h"
#include "Common/CInvertRgnWidget.h"
#include "Common/CRectangleWidget.h"
#include "Common/CFrameRgnWidget.h"
#include "Common/CPolyDrawWidget.h"
#include "Common/CPaintRgnWidget.h"
#include "Common/CFillRgnWidget.h"
#include "Common/CPointsWidget.h"
#include "Common/CLogPenWidget.h"
#include "Common/CPointWidget.h"
#include "Common/CColorWidget.h"
#include "Common/CEmptyWidget.h"
#include "Common/CXFormWidget.h"
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
                ConvertWidgets(ui->recordsBox->currentText().toUpper());

        Close();

        return m_pNewItem;
}

void CRecordCreator::on_selectButton_clicked()
{
        m_nSelectedRecord = ui->recordsBox->currentIndex();
        ClearData();

        switch (m_nSelectedRecord) //TODO: исправить пустые виджеты
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
                case 38: Create_Widgets_EMR_ANGLEARC();                                 break;
                case 39: CreateRectangleWidgets("EMR_ELLIPSE", "Box");                  break;
                case 40: CreateRectangleWidgets("EMR_RECTANGLE", "Box");                break;
                case 41: Create_Widgets_EMR_ROUNDRECT();                                break;
                case 42: CreateArcWidgets("EMR_ARC");                                   break;
                case 43: CreateArcWidgets("EMR_CHORD");                                 break;
                case 44: CreateArcWidgets("EMR_PIE");                                   break;
                case 45: CreateFormWidget("EMR_SELECTPALETTE", "ihPal");                break;
                case 46: Create_Widgets_EMR_CREATEPALETTE();                            break;
                case 47: Create_Widgets_EMR_SETPALETTEENTRIES();                        break;
                case 48: Create_Widgets_EMR_RESIZEPALETTE();                            break;
                case 49: CreateEmptyWidgets("EMR_REALIZEPALETTE");                      break;
                case 50: Create_Widgets_EMR_EXTFLOODFILL();                             break;
                case 51: Create_Widgets_EMR_LINETO();                                   break;
                case 52: CreateArcWidgets("EMR_ARCTO");                                 break;
                case 53: CreatePolyDrawWidgets("EMR_POLYDRAW");                         break;
                case 54: CreateFormWidget("EMR_SETARCDIRECTION", "ArcDirection");       break;
                case 55: CreateFormWidget("EMR_SETMITERLIMIT",   "MiterLimit");         break;
                case 56: CreateEmptyWidgets("EMR_BEGINPATH");                           break;
                case 57: CreateEmptyWidgets("EMR_ENDPATH");                             break;
                case 58: CreateEmptyWidgets("EMR_CLOSEFIGURE");                         break;
                case 59: CreateRectangleWidgets("EMR_FILLPATH",          "Bounds");     break;
                case 60: CreateRectangleWidgets("EMR_STROKEANDFILLPATH", "Bounds");     break;
                case 61: CreateRectangleWidgets("EMR_STROKEPATH",        "Bounds");     break;
                case 62: CreateEmptyWidgets("EMR_FLATTENPATH");                         break;
                case 63: CreateEmptyWidgets("EMR_WIDENPATH");                           break;
                case 64: CreateFormWidget("EMR_SELECTCLIPPATH", "RegionMode");          break;
                case 65: CreateEmptyWidgets("EMR_ABORTPATH");                           break;
                case 66: Create_Widgets_EMR_FILLRGN();                                  break;
                case 67: Create_Widgets_EMR_FRAMERGN();                                 break;
                case 68: Create_Widgets_EMR_INVERTRGN();                                break;
                case 69: Create_Widgets_EMR_PAINTRGN();                                 break;
                case 70: Create_Widgets_EMR_EXTSELECTCLIPRGN();                         break;
                case 71: CreateNotSupportedWidgets("EMR_BITBLT");                       break;
                case 72: CreateNotSupportedWidgets("EMR_STRETCHBLT");                   break;
                case 73: CreateNotSupportedWidgets("EMR_MASKBLT");                      break;
                case 74: CreateNotSupportedWidgets("EMR_PLGBLT");                       break;
                case 75: CreateNotSupportedWidgets("EMR_SETDIBITSTODEVICE");            break;
                case 76: CreateNotSupportedWidgets("EMR_STRETCHDIBITS");                break;
                case 77: Create_Widgets_EMR_EXTCREATEFONTINDIRECTW();                   break;
                case 78: CreateExtTextOutWidget("EMR_EXTTEXTOUTA");                     break;
                case 79: CreateExtTextOutWidget("EMR_EXTTEXTOUTW");                     break;
                case 80: CreatePointWidgets("EMR_POLYBEZIER16");                        break;
                case 81: CreatePointWidgets("EMR_POLYGON16");                           break;
                case 82: CreatePointWidgets("EMR_POLYLINE16");                          break;
                case 83: CreatePointWidgets("EMR_POLYBEZIERTO16");                      break;
                case 84: CreatePointWidgets("EMR_POLYLINETO16");                        break;
                case 85: CreatePolyWidgets("EMR_POLYPOLYLINE16");                       break;
                case 86: CreatePolyWidgets("EMR_POLYPOLYGON16");                        break;
                case 87: CreatePolyDrawWidgets("EMR_POLYDRAW16");                       break;
                case 88: CreateEmptyWidgets("EMR_CREATEMONOBRUSH");                     break;
                case 89: CreateEmptyWidgets("EMR_CREATEDIBPATTERNBRUSHPT");             break;
                case 90: CreateEmptyWidgets("EMR_EXTCREATEPEN");                        break;
                case 91: CreatePolyTextOutWidget("EMR_POLYTEXTOUTA");                   break;
                case 92: CreatePolyTextOutWidget("EMR_POLYTEXTOUTW");                   break;
                case 93: CreateFormWidget("EMR_SETICMMODE", "ICMMode");                 break;
                case 94: Create_Widgets_EMR_CREATECOLORSPACE();                         break;
                case 95: CreateFormWidget("EMR_SETCOLORSPACE",      "ihCS");            break;
                case 96: CreateFormWidget("EMR_DELETECOLORSPACE",   "ihCS");            break;
                case 97: CreateFormWidget("EMR_GLSRECORD",          "Data");            break;
                case 98: Create_Widgets_EMR_GLSBOUNDEDRECORD();                         break;
                case 99: Create_Widgets_EMR_PIXELFORMAT();                              break;
                case 100: CreateFormWidget("EMR_DRAWESCAPE",    "Data");                break;
                case 101: CreateFormWidget("EMR_EXTESCAPE",     "Data");                break;
                case 102: Create_Widgets_EMR_SMALLTEXTOUT();                            break;
                case 103: Create_Widgets_EMR_FORCEUFIMAPPING();                         break;
                case 104: Create_Widgets_EMR_NAMEDESCAPE();                             break;
                case 105: Create_Widgets_EMR_COLORCORRECTPALETTE();                     break;
                case 106: CreateSetICMPRrofileWidget("EMR_SETICMPROFILEA");             break;
                case 107: CreateSetICMPRrofileWidget("EMR_SETICMPROFILEW");             break;
                case 108: CreateEmptyWidgets("EMR_ALPHABLEND");                         break;
                case 109: CreateFormWidget("EMR_SETLAYOUT", "LayoutMode");              break;
                case 110: CreateEmptyWidgets("EMR_TRANSPARENTBLT");                     break;
                case 111: Create_Widgets_EMR_GRADIENTFILL();                            break;
                case 112: Create_Widgets_EMR_SETLINKEDUFIS();                           break;
                case 113: Create_Widgets_EMR_SETTEXTJUSTIFICATION();                    break;
                case 114: Create_Widgets_EMR_COLORMATCHTOTARGETW();                     break;
                case 115: Create_Widgets_EMR_CREATECOLORSPACEW();                       break;
        }

        QPushButton *pOkButton =  ui->buttonBox->button(QDialogButtonBox::Ok);
        if (NULL != pOkButton)
                pOkButton->setEnabled(true);
}

void CRecordCreator::CreatePointWidgets(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        AddSharedWidgets({new CPointWidget(QString("Offset"))});
}

void CRecordCreator::CreateFormWidget(const QString& qsName, const QString& qsFormName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidget(new CFormWidget(qsFormName, "0"));
}

void CRecordCreator::CreateColorWidgets(const QString& qsName, bool bBounds)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));

        if (bBounds)
                AddSharedWidget(new CRectangleWidget("Bounds"));

        AddSharedWidget(new CColorWidget("Color"));
}

void CRecordCreator::CreateEmptyWidgets(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidget(new CEmptyWidget(qsName));
}

void CRecordCreator::CreateRectangleWidgets(const QString &qsName, const QString &qsReactangleName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidget(new CRectangleWidget(qsReactangleName));
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

void CRecordCreator::CreateArcWidgets(const QString &qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidgets({new CRectangleWidget("Box"),
                          new CPointWidget("Start"),
                          new CPointWidget("End")});
}

void CRecordCreator::CreateNotSupportedWidgets(const QString &qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        ui->dataLayout->addWidget(new QLabel("На данный момент создание данной записи не поддерживается"));
}

void CRecordCreator::CreateExtTextOutWidget(const QString &qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidget(new CExtTextOutWidget());
}

void CRecordCreator::CreatePolyTextOutWidget(const QString &qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidget(new CPolyTextOutWidget());
}

void CRecordCreator::CreateSetICMPRrofileWidget(const QString &qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidgets({new CFormWidget("dwFlags","0"),
                          new CFormWidget("cbName", "0"),
                          new CFormWidget("cbData", "0"),
                          new CFormWidget("Data",   "0")});
}

void CRecordCreator::Create_Widgets_EMR_SETCOLORADJUSTMENT()
{
        this->setWindowTitle("Create: EMR_SETCOLORADJUSTMENT");

        AddSharedWidgets({new CFormWidget("Size",            "0"),
                          new CFormWidget("Values",          "0"),
                          new CFormWidget("IlluminantIndex", "0"),
                          new CFormWidget("RedGamma",        "0"),
                          new CFormWidget("GreenGamma",      "0"),
                          new CFormWidget("BlueGamma",       "0"),
                          new CFormWidget("ReferenceBlack",  "0"),
                          new CFormWidget("ReferenceWhite",  "0"),
                          new CFormWidget("Constrast",       "0"),
                          new CFormWidget("Brightness",      "0"),
                          new CFormWidget("Colorfulness",    "0"),
                          new CFormWidget("RedGreenTint",    "0")});
}

void CRecordCreator::Create_Widgets_EMR_SETWORLDTRANSFORM()
{
        this->setWindowTitle("Create: EMR_SETWORLDTRANSFORM");
        AddSharedWidget(new CXFormWidget());
}

void CRecordCreator::Create_Widgets_EMR_MODIFYWORLDTRANSFORM()
{
        this->setWindowTitle("Create: EMR_MODIFYWORLDTRANSFORM");
        AddSharedWidgets({new CXFormWidget(),
                          new CFormWidget("ModifyWorldTransformMode", "1")});
}

void CRecordCreator::Create_Widgets_EMR_CREATEPEN()
{
        this->setWindowTitle("Create: EMR_CREATEPEN");
        AddSharedWidgets({new CFormWidget("ihPen", "1"),
                          new CLogPenWidget()});
}

void CRecordCreator::Create_Widgets_EMR_CREATEBRUSHINDIRECT()
{
        this->setWindowTitle("Create: EMR_CREATEBRUSHINDIRECT");
        AddSharedWidgets({new CFormWidget("ihBrush", "1"),
                          new CLogBrushExWidget()});
}

void CRecordCreator::Create_Widgets_EMR_ANGLEARC()
{
        this->setWindowTitle("Create: EMR_ANGLEARC");
        AddSharedWidgets({new CPointWidget("Center"),
                          new CFormWidget("Radius"),
                          new CFormWidget("StartAngle"),
                          new CFormWidget("SweepAngle")});
}

void CRecordCreator::Create_Widgets_EMR_ROUNDRECT()
{
        this->setWindowTitle("Create: EMR_ROUNDRECT");
        AddSharedWidgets({new CRectangleWidget("Box"),
                          new CSizeWidget("Corner")});
}

void CRecordCreator::Create_Widgets_EMR_CREATEPALETTE()
{
        this->setWindowTitle("Create: EMR_CREATEPALETTE");
        AddSharedWidgets({new CFormWidget("ihPal", "1"),
                          new CLogPaletteWidget()});
}

void CRecordCreator::Create_Widgets_EMR_SETPALETTEENTRIES()
{
        this->setWindowTitle("Create: EMR_SETPALETTEENTRIES");
        AddSharedWidget(new CPaletteEntriesWidget());
}

void CRecordCreator::Create_Widgets_EMR_RESIZEPALETTE()
{
        this->setWindowTitle("Create: EMR_RESIZEPALETTE");
        AddSharedWidgets({new CFormWidget("ihPal", "1"),
                          new CFormWidget("NumberOfEntries", "1")});
}

void CRecordCreator::Create_Widgets_EMR_EXTFLOODFILL()
{
        this->setWindowTitle("Create: EMR_EXTFLOODFILL");
        AddSharedWidgets({new CPointWidget("Start"),
                          new CColorWidget("Color"),
                          new CFormWidget("FloodFillMode", "1")});
}

void CRecordCreator::Create_Widgets_EMR_LINETO()
{
        this->setWindowTitle("Create: EMR_LINETO");
        AddSharedWidget(new CPointWidget("Point"));
}

void CRecordCreator::Create_Widgets_EMR_FILLRGN()
{
        this->setWindowTitle("Create: EMR_FILLRGN");
        AddSharedWidget(new CFillRgnWidget());
}

void CRecordCreator::Create_Widgets_EMR_FRAMERGN()
{
        this->setWindowTitle("Create: EMR_FRAMERGN");
        AddSharedWidget(new CFrameRgnWidget());
}

void CRecordCreator::Create_Widgets_EMR_INVERTRGN()
{
        this->setWindowTitle("Create: EMR_INVERTRGN");
        AddSharedWidget(new CInvertRgnWidget());
}

void CRecordCreator::Create_Widgets_EMR_PAINTRGN()
{
        this->setWindowTitle("Create: EMR_PAINTRGN");
        AddSharedWidget(new CPaintRgnWidget());
}

void CRecordCreator::Create_Widgets_EMR_EXTSELECTCLIPRGN()
{
        this->setWindowTitle("Create: EMR_EXTSELECTCLIPRGN");
        AddSharedWidget(new CExtSelectClipRgnWidget());
}

void CRecordCreator::Create_Widgets_EMR_EXTCREATEFONTINDIRECTW()
{
        this->setWindowTitle("Create: EMR_EXTCREATEFONTINDIRECTW");
        AddSharedWidgets({new CFormWidget("ihFonts", "1"),
                          new CLogFontExDvWidget()});
}

void CRecordCreator::Create_Widgets_EMR_CREATECOLORSPACE()
{
        this->setWindowTitle("Create: EMR_CREATECOLORSPACE");
        AddSharedWidgets({new CFormWidget("ihCS", "1"),
                          new CLogColorSpaceWidget()});
}

void CRecordCreator::Create_Widgets_EMR_GLSBOUNDEDRECORD()
{
        this->setWindowTitle("Create: EMR_GLSBOUNDEDRECORD");
        AddSharedWidgets({new CRectangleWidget("Bounds"),
                          new CFormWidget("Data")});
}

void CRecordCreator::Create_Widgets_EMR_PIXELFORMAT()
{
        this->setWindowTitle("Create: EMR_PIXELFORMAT");
        AddSharedWidget(new CPixelFormatDescriptorWidget());
}

void CRecordCreator::Create_Widgets_EMR_SMALLTEXTOUT()
{
        this->setWindowTitle("Create: EMR_SMALLTEXTOUT");
        AddSharedWidget(new CSmallTextOutWidget());
}

void CRecordCreator::Create_Widgets_EMR_FORCEUFIMAPPING()
{
        this->setWindowTitle("Create: EMR_FORCEUFIMAPPING");
        AddSharedWidget(new CUniversalFontIdWidget());
}

void CRecordCreator::Create_Widgets_EMR_NAMEDESCAPE()
{
        this->setWindowTitle("Create: EMR_NAMEDESCAPE");
        AddSharedWidgets({new CFormWidget("DriverName"),
                          new CFormWidget("Data")});
}

void CRecordCreator::Create_Widgets_EMR_COLORCORRECTPALETTE()
{
        this->setWindowTitle("Create: EMR_COLORCORRECTPALETTE");
        AddSharedWidgets({new CFormWidget("ihPalette",  "0"),
                          new CFormWidget("nFirstEntry","0"),
                          new CFormWidget("nPalEntries","0")});
}

void CRecordCreator::Create_Widgets_EMR_GRADIENTFILL()
{
        this->setWindowTitle("Create: EMR_GRADIENTFILL");
        AddSharedWidget(new CGradientFillWidget());
}

void CRecordCreator::Create_Widgets_EMR_SETLINKEDUFIS()
{
        this->setWindowTitle("Create: Create: EMR_GRADIENTFILL");
        AddSharedWidget(new CLinkedUFISWidget());
}

void CRecordCreator::Create_Widgets_EMR_SETTEXTJUSTIFICATION()
{
        this->setWindowTitle("Create: Create: EMR_SETTEXTJUSTIFICATION");
        AddSharedWidgets({new CFormWidget("nBreakExtra", "0"),
                          new CFormWidget("nBreakCount", "0")});
}

void CRecordCreator::Create_Widgets_EMR_COLORMATCHTOTARGETW()
{
        this->setWindowTitle("Create: Create: EMR_SETTEXTJUSTIFICATION");
        AddSharedWidgets({new CFormWidget("dwAction", "1"),
                          new CFormWidget("dwFlags",  "0"),
                          new CFormWidget("Name"),
                          new CFormWidget("Data")});
}

void CRecordCreator::Create_Widgets_EMR_CREATECOLORSPACEW()
{
        this->setWindowTitle("Create: Create: EMR_SETTEXTJUSTIFICATION");
        AddSharedWidgets({new CFormWidget("ihCS",   "1"),
                          new CLogColorSpaceWidget(),
                          new CFormWidget("dwFlags","0"),
                          new CFormWidget("Data")});
}

void CRecordCreator::CreatePolyWidgets(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidgets({new CRectangleWidget("Bounds"),
                          new CPolyWidget()});
}

void CRecordCreator::CreatePolyDrawWidgets(const QString &qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidget(new CPolyDrawWidget());
}

void CRecordCreator::CreateEXTEX(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidget(new CSizeWidget("Size"));
}

void CRecordCreator::CreateORGEX(const QString& qsName)
{
        this->setWindowTitle(QString("Create: %1").arg(qsName));
        AddSharedWidget(new CPointWidget("Extent"));
}

void CRecordCreator::ConvertWidgets(const QString& qsName)
{
        if (qsName.isEmpty() || m_arWidgets.empty())
                return;

        m_pNewItem = new QStandardItem(QString("<%1>").arg(qsName));

        m_pNewItem->setData("Record", 3);

        for (const CSharedWidget* pWidget : m_arWidgets)
                m_pNewItem->appendRows(pWidget->GetData());
}

void CRecordCreator::AddSharedWidget(CSharedWidget *pWidget)
{
        if (NULL == pWidget)
                return;

        ui->dataLayout->addWidget(pWidget);
        m_arWidgets.push_back(pWidget);
}

void CRecordCreator::AddSharedWidgets(std::vector<CSharedWidget *> arWidgets)
{
        for (CSharedWidget* pWidget : arWidgets)
                AddSharedWidget(pWidget);
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
