#ifndef CMETAFILETREEVIEW_H
#define CMETAFILETREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QFont>

#include "../../../../xml/include/xmlutils.h"


class CMetafileTreeView : public QTreeView
{
public:
    CMetafileTreeView(QWidget *parent);
    virtual ~CMetafileTreeView();

    void SetMetafile(const std::wstring& wsXmlFilePath);
    bool IsClear();

private:
    void ReadXmlNode(XmlUtils::CXmlNode& oXmlNode, QStandardItem& oStandartItem, unsigned int unLevel = 0);

    bool bClear;
    QFont m_pFontBold;
};

#endif // CMETAFILETREEVIEW_H
