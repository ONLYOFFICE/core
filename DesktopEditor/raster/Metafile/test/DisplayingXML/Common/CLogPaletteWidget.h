#ifndef CLOGPALETTEWIDGET_H
#define CLOGPALETTEWIDGET_H

#include "CSharedWidget.h"

class CLogPaletteWidget : public CSharedWidget
{
public:
        explicit CLogPaletteWidget(QWidget *pParent = NULL);
        virtual ~CLogPaletteWidget();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_CreateColor_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CLOGPALETTEWIDGET_H
