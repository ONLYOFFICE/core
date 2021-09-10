#ifndef CPOLYTEXTOUT_H
#define CPOLYTEXTOUT_H

#include "CSharedWidget.h"

class CPolyTextOut : public CSharedWidget
{
public:
        explicit CPolyTextOut(QWidget *pParent = NULL);
        virtual ~CPolyTextOut();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createTextOut_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CPOLYTEXTOUT_H
