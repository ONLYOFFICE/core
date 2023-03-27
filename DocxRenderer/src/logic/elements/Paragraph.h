#pragma once
#include "BaseItem.h"
#include "TextLine.h"

namespace NSDocxRenderer
{
    enum TextAssociationType
    {
            tatBlockChar        = 0, // Каждый символ во фрейме
            tatBlockLine        = 1, // Каждая линия - параграф во фрейме. Линии могут объединяться в рамках одного блока.
            tatPlainLine        = 2, // Каждая линия - параграф обычный
            tatShapeLine        = 3, // Каждая линия - параграф в шейпе. Линии могут объединяться в рамках одного блока.
            tatPlainParagraph   = 4  // Линии объединяются в параграфы
    };

    class CParagraph : public CBaseItem
    {
    public:
        enum TextAlignmentType
        {
            tatUnknown,
            tatByLeftEdge,
            tatByCenter,
            tatByRightEdge,
            tatByWidth
        };

        enum TextConversionType
        {
            tctUnknown,
            tctTextToParagraph,
            tctTextToFrame,
            tctTextToShape
        };

        // text frame properties
        TextConversionType m_eTextConversionType {tctUnknown};
        bool               m_bIsNeedFirstLineIndent {false};
        bool               m_bIsAroundTextWrapping {true}; //по умолчанию обтекание включено, если отсутсвует w:wrap
        bool               m_bIsShadingPresent {false};
        LONG               m_lColorOfShadingFill {c_iWhiteColor}; //BGR
        TextAlignmentType  m_eTextAlignmentType {tatUnknown};

        // geometry paragraph
        double		m_dRight {0.0}; //сдвиг относительно правого края страницы
        double		m_dFirstLine {0.0}; //сдвиг относительно m_dLeft

        double		m_dSpaceBefore {0.0}; //по умолчанию выставляется 0, если отсутсвует w:before
        double		m_dSpaceAfter {0.0}; //в shape по умолчанию выставляется 8pt, если отсутсвует w:after
        double		m_dBaselinePos {0.0};
        TextAssociationType m_eTextAssociationType {tatPlainParagraph};

        std::vector<CTextLine*> m_arLines;
    public:
        CParagraph(const TextAssociationType& eType);
        virtual ~CParagraph();
        void Clear() override final;

        void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

        void RemoveHighlightColor();

        void MergeLines();
   };
}
