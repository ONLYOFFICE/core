#ifndef CFORMWIDGET_H
#define CFORMWIDGET_H

#include "CSharedWidget.h"

#include <QTextEdit>

class CFormWidget : public CSharedWidget
{
public:
        explicit CFormWidget(QString qsName, QString qsDefaultValue = QString(), QWidget *pParent = NULL);
        virtual ~CFormWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        QString m_qsName;
        QTextEdit *m_pValue;
};

#endif // CFORMWIDGET_H
