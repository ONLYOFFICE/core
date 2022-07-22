#pragma once
#include "ElementParagraph.h"
#include "../resources/LinesTable.h"
#include "../resources/VectorGraphics.h"

namespace NSDocxRenderer
{
    enum class eGraphicsType
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
            std::wstring            m_strPath {L""};
            NSStructures::CBrush	m_oBrush;
            NSStructures::CPen		m_oPen;

            bool m_bIsNoFill {false};
            bool m_bIsNoStroke {false};
            bool m_bIsBehindDoc {true};

            LONG m_lCoordSizeX {100000};
            LONG m_lCoordSizeY {100000};

            LONG m_lTxId {-1};

            eGraphicsType m_eGraphicsType {eGraphicsType::gtUnknown};
            eSimpleLineType m_eSimpleLineType {eSimpleLineType::sltUnknown};
            eLineType m_eLineType {eLineType::ltUnknown};

            std::vector<CParagraph*> m_arParagraphs;

            //Показывает, что есть отношение графики к тексту (подчеркивания/зачеркивания/выделение).
            //note Пока сюда записывается указатель на символ с наибольшем размером шрифта.
            const CContText* m_pCont {nullptr};

        public:
            CShape();
            virtual ~CShape();
            virtual void Clear() override final;

            CShape(const CShape& oSrc);
            //CShape(const CShape& oSrc1, const CShape& oSrc2);

            CShape& operator=(const CShape& oSrc);

            void GetDataFromVector(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize);

            void WritePath(const CVectorGraphics& oVector);

            void DetermineGraphicsType(const double& dWidth, const double& dHeight, const size_t& nPeacks, const size_t& nCurves);

            bool IsItFitLine();
            bool IsCorrelated(const CShape* pShape);
            void ChangeGeometryOfDesiredShape(CShape* pShape);

            void DetermineLineType(CShape* pShape = nullptr, bool bIsLast = false);

            virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;
    };
}
