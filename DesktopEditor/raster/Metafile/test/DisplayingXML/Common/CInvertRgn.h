#ifndef CINVERTRGN_H
#define CINVERTRGN_H

#include "CSharedWidget.h"

class CInvertRgn : public CSharedWidget
{
public:
        explicit CInvertRgn(QWidget *pParent = NULL);
        virtual ~CInvertRgn();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createRgnData_clicked();
private:
        unsigned int m_unCounter;
};

class CPaintRgn : public CInvertRgn
{
};

#endif // CINVERTRGN_H
