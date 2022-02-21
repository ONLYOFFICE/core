#ifndef CSHAREDWIDGET_H
#define CSHAREDWIDGET_H

#include <QFrame>
#include <QBoxLayout>
#include <QStandardItem>

class CSharedWidget : public QFrame
{
        Q_OBJECT
public:
        explicit CSharedWidget(QWidget *pParent = NULL) : QFrame(pParent) {setFrameShape(QFrame::Box);};
        virtual ~CSharedWidget() {};

        virtual QList<QStandardItem*> GetData() const
        {
                QList<QStandardItem*> arData;

                for (const CSharedWidget *pWidget : m_arWidgets)
                        arData.append(pWidget->GetData());

                return arData;
        }
private:
        friend class CPixelFormatDescriptorWidget;
        friend class CGradientRectangleWidget;
        friend class CGradientTriangleWidget;
        friend class CExtSelectClipRgnWidget;
        friend class CUniversalFontIdWidget;
        friend class CPaletteEntriesWidget;
        friend class CLogColorSpaceWidget;
        friend class CSmallTextOutWidget;
        friend class CCIEXYZTripleWidget;
        friend class CGradientFillWidget;
        friend class CDesignVectorWidget;
        friend class CLogFontExDvWidget;
        friend class CPolyTextOutWidget;
        friend class CLogBrushExWidget;
        friend class CLogPaletteWidget;
        friend class CExtTextOutWidget;
        friend class CLinkedUFISWidget;
        friend class CInvertRgnWidget;
        friend class CLogFontExWidget;
        friend class CTriVertexWidget;
        friend class CRectangleWidget;
        friend class CPolyDrawWidget;
        friend class CFrameRgnWidget;
        friend class CPaintRgnWidget;
        friend class CLogFontWidget;
        friend class CFillRgnWidget;
        friend class CEmrTextWidget;
        friend class CLogPenWidget;
        friend class CPointsWidget;
        friend class CXFormWidget;
        friend class CIEXYZWidget;
        friend class CPointWidget;
        friend class CSizeWidget;
        friend class CPolyWidget;
        friend class CDxWidget;

        void AddWidget(CSharedWidget* pWidget, QBoxLayout *pLayout = NULL)
        {
                if (NULL == pWidget)
                        return;

                m_arWidgets.push_back(pWidget);

                if (NULL != pLayout)
                        pLayout->addWidget(pWidget);
        }

        void AddWidgets(std::vector<CSharedWidget*> arWidgets, QBoxLayout *pLayout = NULL)
        {
                for (CSharedWidget* pWidget : arWidgets)
                        AddWidget(pWidget, pLayout);
        }

        std::vector<CSharedWidget*> m_arWidgets;
};

#endif // CSHAREDWIDGET_H
