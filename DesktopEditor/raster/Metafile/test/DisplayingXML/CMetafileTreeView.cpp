#include "CMetafileTreeView.h"

#include <QFile>
#include <QMessageBox>
#include <QStandardItem>

#include "XMLHighlighter.h"

#ifdef _DEBUG
    #include <QDebug>
#endif

CMetafileTreeView::CMetafileTreeView(QWidget *parent) :
    QTreeView(parent)
{
    m_pFontBold.setBold(true);
    bClear = true;
}

CMetafileTreeView::~CMetafileTreeView()
{
}

void CMetafileTreeView::SetMetafile(const std::wstring &wsXmlFilePath)
{
    setModel(nullptr);

    if (wsXmlFilePath.empty())
        return;

    QFile *oXmlFile = new QFile(QString::fromStdWString(wsXmlFilePath));

    if (!oXmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Warning", "Couldn't open XML file");
        return;
    }

    XmlUtils::CXmlNode oXmlRootNode;

    if (!oXmlRootNode.FromXmlFile(wsXmlFilePath))
        return;

    if (oXmlRootNode.GetName() != L"EMF" &&
        oXmlRootNode.GetName() != L"WMF")
        return;

    setHeaderHidden(true);

    QStandardItemModel *oStandardItemModel = new QStandardItemModel;
    QStandardItem *oStandardItem = new QStandardItem(QString::fromStdWString(L'<' + oXmlRootNode.GetName() + L'>'));

    oStandardItem->setFont(m_pFontBold);
    oStandardItem->setForeground(Qt::darkRed);

    ReadXmlNode(oXmlRootNode, *oStandardItem, 1);

    QFont oMainFont;
    oMainFont.setPointSize(10);
    setFont(oMainFont);

    oStandardItemModel->appendRow(oStandardItem);
    setModel(oStandardItemModel);

    bClear = false;
}

bool CMetafileTreeView::IsClear()
{
    return NULL == model();
}

void CMetafileTreeView::ReadXmlNode(XmlUtils::CXmlNode& oXmlNode, QStandardItem& oStandartItem, unsigned int unLevel)
{
    XmlUtils::CXmlNodes oXmlChilds;

    if (oXmlNode.GetChilds(oXmlChilds))
    {
        for (unsigned int i = 0; i < oXmlChilds.GetCount(); ++i)
        {
            XmlUtils::CXmlNode oXmlChild;
            if (oXmlChilds.GetAt(i, oXmlChild))
            {
                    QString qsArguments;
                    if (0 < oXmlChild.GetAttributesCount())
                    {
                            std::vector<std::wstring> arNameArguments, arValueArguments;
                            oXmlChild.GetAllAttributes(arNameArguments, arValueArguments);

                            for (unsigned int i = 0; i < arNameArguments.size(); ++i)
                                qsArguments += QString(" %1=\"%2\"").arg(QString::fromStdWString(arNameArguments[i]), QString::fromStdWString(arValueArguments[i]));
                    }

                    QStandardItem *pStandardItem = new QStandardItem(QString("<%1%2>").arg(QString::fromStdWString(oXmlChild.GetName()), qsArguments));

                    if (unLevel == 1)
                    {
                        pStandardItem->setFont(m_pFontBold);
                        pStandardItem->setForeground(Qt::blue);
                    }

                    ReadXmlNode(oXmlChild, *pStandardItem, unLevel + 1);
                    oStandartItem.appendRow(pStandardItem);
            }
        }
    }
    else
    {
        std::wstring wsText = oXmlNode.GetText();
        if (!wsText.empty())
        {
            QString qsText = oStandartItem.text();
            oStandartItem.setText(qsText + (QString::fromStdWString(wsText)) + qsText[0] + '/' + qsText.mid(1));
        }
    }
}
