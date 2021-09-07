#ifndef CPALETTEENTRIESWIDGET_H
#define CPALETTEENTRIESWIDGET_H

#include "CSharedWidget.h"

class CPaletteEntriesWidget : public CSharedWidget
{
public:
        explicit CPaletteEntriesWidget(QWidget *pParent = NULL);
        virtual ~CPaletteEntriesWidget();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_CreateColor_clicked();
private:
        unsigned int m_unCounter;
};

#endif // CPALETTEENTRIESWIDGET_H
