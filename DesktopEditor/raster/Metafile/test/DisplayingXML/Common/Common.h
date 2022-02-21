#include <QLayout>
#include <QTextEdit>
#include <QLayoutItem>

static void AlignTextEdit(QTextEdit *pTextEdit)
{
        if (NULL == pTextEdit)
                return;

        QFontMetrics oFontMEtrics(pTextEdit->font());
        pTextEdit->setFixedHeight(oFontMEtrics.height() + 10);
        pTextEdit->setStyleSheet("QTextEdit { vertical-align: middle }");
}

static void ClearLayout(QLayout *pLayout)
{
        if (pLayout == NULL)
            return;

        QLayoutItem *pItem;
        while((pItem = pLayout->takeAt(0)))
        {
                if (pItem->layout())
                {
                        ClearLayout(pItem->layout());
                        delete pItem->layout();
                }
                else if (pItem->widget())
                {
                        delete pItem->widget();
                }
                else delete pItem;
        }
}
