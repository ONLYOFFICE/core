#include <QStandardItem>
#include <QHBoxLayout>
#include <QTreeView>

#include "CXMLHighlighter.h"
#include "CItemWidget.h"

CItemWidget::CItemWidget(QWidget *parent) :
        QWidget(parent),
        m_pTextEdit(new QTextEdit)
{        
        m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_pTextEdit->setReadOnly(true);
}

CItemWidget::~CItemWidget()
{
        delete m_pTextEdit;
}

QString CItemWidget::GetText()
{
        return m_pTextEdit->toPlainText();
}

void CItemWidget::SetText(const QString &qsString)
{
        m_pTextEdit->setText(qsString);
}

void CItemWidget::SetFont(const QFont &oFont)
{
        m_pTextEdit->setFont(oFont);
}
