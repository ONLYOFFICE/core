#ifndef CPOLYDRAWWIDGET_H
#define CPOLYDRAWWIDGET_H

#include "CSharedWidget.h"

class CPolyDrawWidget : public CSharedWidget
{
public:
        explicit CPolyDrawWidget(QWidget *pParent = NULL);
        virtual ~CPolyDrawWidget();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createPoint_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CPOLYDRAWWIDGET_H
