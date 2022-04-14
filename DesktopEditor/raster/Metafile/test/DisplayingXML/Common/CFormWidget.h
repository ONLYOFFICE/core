#ifndef CFORMWIDGET_H
#define CFORMWIDGET_H

#include "CSharedWidget.h"

#include <QTextEdit>

class CFormWidget : public CSharedWidget
{
public:
        explicit CFormWidget(const QString& qsName, const QString& qsDefaultValue = QString(), QWidget *pParent = NULL);
        virtual ~CFormWidget();

        virtual QList<QStandardItem*> GetData() const override;
        QTextEdit* GetTextEditWidget() const;
private:
        QString m_qsName;
        QTextEdit *m_pValue;
};

#endif // CFORMWIDGET_H
