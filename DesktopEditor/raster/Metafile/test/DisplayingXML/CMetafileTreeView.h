#ifndef CMETAFILETREEVIEW_H
#define CMETAFILETREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>

#include "../../../../xml/include/xmlutils.h"

class CMetafileTreeView : public QTreeView
{
        Q_OBJECT
    public:
        explicit CMetafileTreeView(QWidget *parent = NULL);
        virtual ~CMetafileTreeView();

        void SetMetafile(const std::wstring& wsXmlFilePath);
        bool IsClear();
        void SetMode(bool bLightMode);
        void Clear();
        QMap<QString, unsigned int>* GetStatistics();

        bool SaveInXmlFile(const std::wstring& wsSaveFilePath);

        void EditItem(QStandardItem* pStandardItem);
    signals:
        void clickedRightMouseButton(QPoint oPoint);
        void clickedLeftMouseButton(QModelIndex oModelInde);

    protected:
        virtual void mouseReleaseEvent(QMouseEvent* event) override;

    private:
        void ReadXmlNode(XmlUtils::CXmlNode& oXmlNode, QStandardItem* oStandartItem, unsigned int unLevel = 0);
        void WriteXmlNode(XmlUtils::CXmlWriter& oXmlWriter, QStandardItem* oStandartItem);

        QMap<QString, unsigned int> m_mStatistics;
};

#endif // CMETAFILETREEVIEW_H
