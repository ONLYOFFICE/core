#include "CMapModel.h"

CMapModel::CMapModel(QObject *parent) :
    QAbstractTableModel(parent)
{}

CMapModel::~CMapModel()
{}

int CMapModel::rowCount(const QModelIndex &parent) const
{
        return m_pMap.count();
}

int CMapModel::columnCount(const QModelIndex &parent) const
{
        return 2;
}

QVariant CMapModel::data(const QModelIndex &index, int role) const
{
        if (m_pMap.isEmpty() || !index.isValid() || index.row() >= m_pMap.count() || role != Qt::DisplayRole)
                return QVariant();

        QMap<class QString,unsigned int>::const_iterator it = m_pMap.cbegin();
        it += index.row();

        if (index.column() == 0)
                return it.key();
        if (index.column() == 1)
                return it.value();

        return QVariant();
}

void CMapModel::SetMap(const QMap<QString, unsigned int>& map)
{
        m_pMap = map;
}
