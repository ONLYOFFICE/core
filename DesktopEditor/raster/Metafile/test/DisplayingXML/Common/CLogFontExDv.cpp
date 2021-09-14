#include "CFormWidget.h"
#include "CLogFontExDv.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>

CLogFontExDv::CLogFontExDv(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        CLogFontEx *pLogFontEx          = new CLogFontEx();
        CDesignVector *pDesignVector    = new CDesignVector();

        m_arWidgets.push_back(pLogFontEx);
        m_arWidgets.push_back(pDesignVector);

        pLayout->addWidget(pLogFontEx);
        pLayout->addWidget(pDesignVector);

        setLayout(pLayout);
}

CLogFontExDv::~CLogFontExDv()
{

}

QList<QStandardItem *> CLogFontExDv::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogFontExDv>");

        for (const CSharedWidget *pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}

CLogFontEx::CLogFontEx(QWidget *pParent) :
        CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("LogFontEx:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CLogFont *pLogFontWidget        = new CLogFont();
        CFormWidget *pFullNameWidget    = new CFormWidget("FullName");
        CFormWidget *pStyleWidget       = new CFormWidget("Style");
        CFormWidget *pScriptWidget      = new CFormWidget("Script");

        m_arWidgets.push_back(pLogFontWidget);
        m_arWidgets.push_back(pFullNameWidget);
        m_arWidgets.push_back(pStyleWidget);
        m_arWidgets.push_back(pScriptWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pLogFontWidget);
        pLayout->addWidget(pFullNameWidget);
        pLayout->addWidget(pStyleWidget);
        pLayout->addWidget(pScriptWidget);

        setLayout(pLayout);
}

CLogFontEx::~CLogFontEx()
{

}

QList<QStandardItem *> CLogFontEx::GetData() const
{
        QList<QStandardItem *> arDatas;

        for (const CSharedWidget* pWidget : m_arWidgets)
                arDatas.append(pWidget->GetData());

        return arDatas;
}

CLogFont::CLogFont(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("LogFont:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CFormWidget *pHeightWidget              = new CFormWidget("Height", "0");
        CFormWidget *pWidthWidget               = new CFormWidget("Width", "0");
        CFormWidget *pEscapementWidget          = new CFormWidget("Escapement", "0");
        CFormWidget *pOrientationWidget         = new CFormWidget("Orientation", "0");
        CFormWidget *pWeightWidget              = new CFormWidget("Weight", "0");
        CFormWidget *pItalicWidget              = new CFormWidget("Italic", "0");
        CFormWidget *pUnderlineWidget           = new CFormWidget("Underline", "0");
        CFormWidget *pStrikeOutWidget           = new CFormWidget("StrikeOut", "0");
        CFormWidget *pCharSetOutWidget          = new CFormWidget("CharSet", "0");
        CFormWidget *pOutPrecisionOutWidget     = new CFormWidget("OutPrecision", "0");
        CFormWidget *pClipPrecisionWidget       = new CFormWidget("ClipPrecision", "0");
        CFormWidget *pQualityWidget             = new CFormWidget("Quality", "0");
        CFormWidget *pPitchAndFamilyWidget      = new CFormWidget("PitchAndFamily", "0");
        CFormWidget *pFacenameWidget            = new CFormWidget("Facename", "0");

        m_arWidgets.push_back(pHeightWidget);
        m_arWidgets.push_back(pWidthWidget);
        m_arWidgets.push_back(pEscapementWidget);
        m_arWidgets.push_back(pOrientationWidget);
        m_arWidgets.push_back(pWeightWidget);
        m_arWidgets.push_back(pItalicWidget);
        m_arWidgets.push_back(pUnderlineWidget);
        m_arWidgets.push_back(pStrikeOutWidget);
        m_arWidgets.push_back(pCharSetOutWidget);
        m_arWidgets.push_back(pOutPrecisionOutWidget);
        m_arWidgets.push_back(pClipPrecisionWidget);
        m_arWidgets.push_back(pQualityWidget);
        m_arWidgets.push_back(pPitchAndFamilyWidget);
        m_arWidgets.push_back(pFacenameWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pHeightWidget);
        pLayout->addWidget(pWidthWidget);
        pLayout->addWidget(pEscapementWidget);
        pLayout->addWidget(pOrientationWidget);
        pLayout->addWidget(pWeightWidget);
        pLayout->addWidget(pItalicWidget);
        pLayout->addWidget(pUnderlineWidget);
        pLayout->addWidget(pStrikeOutWidget);
        pLayout->addWidget(pCharSetOutWidget);
        pLayout->addWidget(pOutPrecisionOutWidget);
        pLayout->addWidget(pClipPrecisionWidget);
        pLayout->addWidget(pQualityWidget);
        pLayout->addWidget(pPitchAndFamilyWidget);
        pLayout->addWidget(pFacenameWidget);

        setLayout(pLayout);
}

CLogFont::~CLogFont()
{

}

QList<QStandardItem *> CLogFont::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<LogFont>");

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}

CDesignVector::CDesignVector(QWidget *pParent) :
        CSharedWidget(pParent),
        m_unCounter(0)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("DesignVector:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        QPushButton *pAddedButton = new QPushButton("Добавить значение");

        connect(pAddedButton, &QPushButton::clicked, this, &CDesignVector::on_createValue_clicked);

        pLayout->addWidget(pName);
        pLayout->addWidget(pAddedButton);

        setLayout(pLayout);
}

CDesignVector::~CDesignVector()
{

}

QList<QStandardItem *> CDesignVector::GetData() const
{
        QStandardItem *pMainItem = new QStandardItem("<DesignVector>");

        pMainItem->appendRow(new QStandardItem("<Signature>134248036</Signature>"));
//        pMainItem->appendRows(new QStandardItem(""))

        for (const CSharedWidget* pWidget : m_arWidgets)
                pMainItem->appendRows(pWidget->GetData());

        return {pMainItem};
}

void CDesignVector::on_createValue_clicked()
{
        if (m_unCounter == 16)
                return;

        CFormWidget *pDataItem = new CFormWidget(QString("Value%1").arg(++m_unCounter));
        m_arWidgets.push_back(pDataItem);

        QBoxLayout *pMainLayout = (QBoxLayout*)layout();
        pMainLayout->insertWidget(pMainLayout->count() - 1, pDataItem);
}

CUniversalFontIdWidget::CUniversalFontIdWidget(QWidget *pParent)
        : CSharedWidget(pParent)
{
        QVBoxLayout *pLayout = new QVBoxLayout();

        QLabel *pName = new QLabel("UniversalFontId:");
        pName->setStyleSheet("QLabel { text-decoration: underline; }");

        CFormWidget *pChecksumWidget = new CFormWidget("Checksum", "0");
        CFormWidget *pIndexWidget = new CFormWidget("Index", "0");

        m_arWidgets.push_back(pChecksumWidget);
        m_arWidgets.push_back(pIndexWidget);

        pLayout->addWidget(pName);
        pLayout->addWidget(pChecksumWidget);
        pLayout->addWidget(pIndexWidget);

        setLayout(pLayout);
}

CUniversalFontIdWidget::~CUniversalFontIdWidget()
{

}

QList<QStandardItem *> CUniversalFontIdWidget::GetData() const
{
        QStandardItem *pItem = new QStandardItem("<UniversalFontId>");

        for (const CSharedWidget* pWidget : m_arWidgets)
                pItem->appendRows(pWidget->GetData());

        return {pItem};
}
