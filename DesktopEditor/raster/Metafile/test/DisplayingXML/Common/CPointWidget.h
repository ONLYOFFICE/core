#ifndef CPOINTWIDGET_H
#define CPOINTWIDGET_H

#include "CSharedWidget.h"

class CPointWidget : public CSharedWidget
{
public:
        explicit CPointWidget(const QString& qsNameRecord, unsigned int unIndex = 0, QWidget *pParent = NULL);
        virtual ~CPointWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        unsigned int m_unIndex;
        QString m_qsNameRecord;
};

#endif // CPOINTWIDGET_H
