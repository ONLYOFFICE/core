#pragma once
#include "BaseItem.h"
#include "../../resources/VectorGraphics.h"
#include "Paragraph.h"

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
            std::wstring m_strPath {L""};
            NSStructures::CBrush	m_oBrush;
            NSStructures::CPen		m_oPen;

            bool m_bIsNoFill {false};
            bool m_bIsNoStroke {false};

            LONG m_lCoordSizeX {100000};
            LONG m_lCoordSizeY {100000};

            LONG m_lTxId {-1};

            std::vector<CParagraph*> m_arParagraphs;

        public:
            COldShape();
            virtual ~COldShape();
            void Clear() override final;

            void GetDataFromVector(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize);

            void WritePath(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize);

            void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;
    };
}
