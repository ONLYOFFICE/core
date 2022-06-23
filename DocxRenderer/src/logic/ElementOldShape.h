#pragma once
#include "BaseItem.h"
#include "../resources/VectorGraphics.h"
#include "ElementParagraph.h"

namespace NSDocxRenderer
{
    class COldShape : public CBaseItem
    {
        public:
            //Подобранные константы
            static const double POSITION_CORRECTION_FOR_X_MM;
            static const double POSITION_CORRECTION_FOR_Y_MM;
            static const double SIZE_CORRECTION_FOR_X_MM;
            static const double SIZE_CORRECTION_FOR_Y_MM;

        public:
            std::wstring m_strPath;
            NSStructures::CBrush	m_oBrush;
            NSStructures::CPen		m_oPen;

            double m_dLeft;
            double m_dTop;
            double m_dWidth;
            double m_dHeight;

            bool m_bIsNoFill;
            bool m_bIsNoStroke;

            LONG m_lCoordSizeX;
            LONG m_lCoordSizeY;

            LONG m_lTxId;

            std::vector<CParagraph*> m_arParagraphs;

            CFontManagerLight* m_pManagerLight;

        public:
            COldShape();
            COldShape(const COldShape& oSrc);
            virtual ~COldShape();
            virtual void Clear();

            COldShape& operator=(const COldShape& oSrc);

            void GetDataFromVector(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize);

            void WritePath(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize);

            virtual void ToXml(NSStringUtils::CStringBuilder& oWriter);
    };
}
