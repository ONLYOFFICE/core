#ifndef CSHAREDWIDGET_H
#define CSHAREDWIDGET_H

#include <QFrame>
#include <QStandardItem>

class CSharedWidget : public QFrame
{
        Q_OBJECT
public:
        explicit CSharedWidget(QWidget *pParent = NULL) : QFrame(pParent) {setFrameShape(QFrame::Box);};
        virtual ~CSharedWidget() {};

        virtual QList<QStandardItem*> GetData() const = 0;
private:
        friend class CBoundsWidget;
        friend class CPointsWidget;
        friend class CPolyWidget;

        void ConvertWidgets(QStandardItem* pParentItem) const
        {
                if (NULL == pParentItem || m_arWidgets.empty())
                        return;

                for (const CSharedWidget* pWidget : m_arWidgets)
                        pParentItem->appendRow(pWidget->GetData());
        };

        std::vector<CSharedWidget*> m_arWidgets;
};

#endif // CSHAREDWIDGET_H
