#ifndef CMETAFILETREEVIEW_H
#define CMETAFILETREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
#include <QFont>

#include "../../../../xml/include/xmlutils.h"


class CMetafileTreeView : public QTreeView
{
public:
    explicit CMetafileTreeView(QWidget *parent);
    virtual ~CMetafileTreeView();

    void SetMetafile(const std::wstring& wsXmlFilePath);
    bool IsClear();
    void SetMode(bool bLightMode);

private:
    void ReadXmlNode(XmlUtils::CXmlNode& oXmlNode, QStandardItem& oStandartItem, unsigned int unLevel = 0);
};

#endif // CMETAFILETREEVIEW_H
