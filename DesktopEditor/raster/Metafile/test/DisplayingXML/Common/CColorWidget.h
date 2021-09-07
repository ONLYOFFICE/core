#ifndef CCOLORWIDGET_H
#define CCOLORWIDGET_H

#include "CSharedWidget.h"
#include <QBoxLayout>

class CColorWidget : public CSharedWidget
{
public:
        explicit CColorWidget(const QString& qsName, QWidget *pParent = NULL);
        virtual ~CColorWidget();

        void setReverseOrder(bool bReverseOrder);

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_select_color_clicked();
private:
        QString m_qsName;
        QColor m_oColor;
        QBoxLayout *m_pColorLayout;

        bool m_bReverseOrder;
};

#endif // CCOLORWIDGET_H
