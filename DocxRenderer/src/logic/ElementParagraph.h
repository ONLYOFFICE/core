#pragma once
#include "BaseItem.h"
#include "FontManager.h"
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
        TextConversionType m_eTextConversionType;
        bool               m_bIsNeedFirstLineIndent;
        bool               m_bIsAroundTextWrapping; //по умолчанию обтекание включено, если отсутсвует w:wrap
        bool               m_bIsShadingPresent;
        LONG               m_lColorOfShadingFill; //BGR
        TextAlignmentType  m_eTextAlignmentType;

        // geometry paragraph
        double		m_dLeft;  //сдвиг относительно левого края страницы
        double		m_dRight; //сдвиг относительно правого края страницы
        double		m_dTop;
        double		m_dWidth;
        double		m_dHeight; //высота каждой строки
        double		m_dFirstLine; //сдвиг относительно m_dLeft

        CFontManagerLight* m_pManagerLight;

        double		m_dSpaceBefore; //по умолчанию выставляется 0, если отсутсвует w:before
        double		m_dSpaceAfter; //в shape по умолчанию выставляется 8pt, если отсутсвует w:after
        double		m_dBaselinePos;
        TextAssociationType m_eTextAssociationType;

        std::vector<CTextLine*> m_arLines;

        // statistic
        size_t      m_nNumLines; //число изначально входящих линий

    public:
        CParagraph(const TextAssociationType& eType);
        CParagraph(const CParagraph& oSrc);
        virtual ~CParagraph();
        virtual void Clear();

        CParagraph& operator=(const CParagraph& oSrc);

        virtual void ToXml(NSStringUtils::CStringBuilder& oWriter);

        void RemoveHighlightColor();
   };
}
