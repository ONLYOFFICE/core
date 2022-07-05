#pragma once
#include "ElementParagraph.h"
#include "../resources/LinesTable.h"
#include "../resources/VectorGraphics.h"

namespace NSDocxRenderer
{
    enum eGraphicsType
    {
        gtUnknown,
        gtRectangle,
        gtCurve,
        gtComplicatedFigure,
        gtNoGraphics,
    };

    class CShape : public CBaseItem
    {
        public:
            std::wstring            m_strPath;
            NSStructures::CBrush	m_oBrush;
            NSStructures::CPen		m_oPen;

            bool m_bIsNoFill;
            bool m_bIsNoStroke;
            bool m_bIsBehindDoc;

            LONG m_lCoordSizeX;
            LONG m_lCoordSizeY;

            LONG m_lTxId;

            eGraphicsType m_eGraphicsType;
            eSimpleLineType m_eSimpleLineType;
            eLineType m_eLineType;

            std::vector<CParagraph*> m_arParagraphs;

            CFontManagerLight* m_pManagerLight;

        public:
            CShape();
            virtual ~CShape();
            virtual void Clear();

            CShape(const CShape& oSrc);
            CShape(const CShape& oSrc1, const CShape& oSrc2);

            CShape& operator=(const CShape& oSrc);

            void GetDataFromVector(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize);

            void WritePath(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize);

            void DetermineGraphicsType(const double& dWidth, const double& dHeight, const size_t& nPeacks, const size_t& nCurves);

            bool IsItFitLine();
            bool IsCorrelated(const CShape* pShape);
            void ChangeGeometryOfDesiredShape(CShape* pShape);

            void DetermineLineType(CShape* pShape = nullptr);

            virtual void ToXml(NSStringUtils::CStringBuilder& oWriter);
    };
}
