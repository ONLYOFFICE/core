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
        friend class CLogBrushExWidget;
        friend class CLogPaletteWidget;
        friend class CExtTextOutWidget;
        friend class CTriVertexWidget;
        friend class CRectangleWidget;
        friend class CPolyDrawWidget;
        friend class CFillRgnWidget;
        friend class CEmrTextWidget;
        friend class CLogPenWidget;
        friend class CPointsWidget;
        friend class CDesignVector;
        friend class CPointWidget;
        friend class CLogFontExDv;
        friend class CPolyTextOut;
        friend class CIEXYZWidget;
        friend class CSizeWidget;
        friend class CPolyWidget;
        friend class CInvertRgn;
        friend class CLogFontEx;
        friend class CDxWidget;
        friend class CFrameRgn;
        friend class CLogFont;

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
