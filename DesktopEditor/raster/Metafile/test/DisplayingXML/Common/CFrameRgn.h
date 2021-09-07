#ifndef CFRAMERGN_H
#define CFRAMERGN_H

#include "CSharedWidget.h"

class CFrameRgn : public CSharedWidget
{
public:
        explicit CFrameRgn(QWidget *pParent = NULL);
        virtual ~CFrameRgn();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createRgnData_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CFRAMERGN_H
