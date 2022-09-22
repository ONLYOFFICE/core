#ifndef CPOLYWIDGET_H
#define CPOLYWIDGET_H

#include "CSharedWidget.h"

class CPolyWidget : public CSharedWidget
{
        Q_OBJECT
public:
        explicit CPolyWidget(QWidget *pParent = NULL);
        virtual ~CPolyWidget();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createPoly_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CPOLYWIDGET_H
