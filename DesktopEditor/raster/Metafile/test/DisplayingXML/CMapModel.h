#ifndef CMAPMODEL_H
#define CMAPMODEL_H

#include <QAbstractTableModel>
#include <QMap>

class CMapModel : public QAbstractTableModel
{
        Q_OBJECT
    public:
        explicit CMapModel(QObject *parent = nullptr);
        virtual ~CMapModel();

        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        void SetMap(const QMap<QString, unsigned int>& map);

    private:
        QMap<QString, unsigned int> m_pMap;
};

#endif // CMAPMODEL_H
