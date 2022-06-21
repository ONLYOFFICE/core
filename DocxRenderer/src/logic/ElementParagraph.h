#ifndef DOCX_RENDERER_ELEMENT_PARAGRAPH_H
#define DOCX_RENDERER_ELEMENT_PARAGRAPH_H

#include "Common.h"
#include "FontManager.h"

namespace NSDocxRenderer
{
    // у класса T должен быть метод IsBigger, IsBiggerOrEqual
    template<typename T>
    void SortElements(std::vector<T*>& oArray)
    {
        int nSize = (int)oArray.size();

        // handle 0, 1 and 2 elements
        if (nSize <= 1)
            return;
        if (nSize == 2)
        {
            if (oArray[0]->IsBigger(oArray[1]))
            {
                T* pTemp	= oArray[0];
                oArray[0]	= oArray[1];
                oArray[1]	= pTemp;
            }
            return;
        }

        T* tTemp;

        // arrange elements as tree with greater elements appearing first
        int nIndex = (nSize >> 1) - 1, nCurr = 0, nNext = 0;
        int nLast = nSize - 1;
        int nHalf = nSize >> 1;
        do
        {
            // save element at start of chain
            tTemp = oArray[nIndex];

            nCurr = nIndex;
            while (nCurr < nHalf)
            {
                nNext = (nCurr << 1) + 1;
                if (nNext < nLast && (oArray[nNext + 1]->IsBigger(oArray[nNext])))
                    nNext++;
                if (tTemp->IsBiggerOrEqual(oArray[nNext]))
                    break;

                // promote element in chain
                oArray[nCurr] = oArray[nNext];
                nCurr = nNext;
            }

            // restore element at end of chain
            oArray[nCurr] = tTemp;
        }
        while (nIndex--);

        // sequentially reduce tree size by removing maximum element and rebalancing
        nIndex = nSize;
        while (--nIndex)
        {
            // save element at start of chain
            tTemp = oArray[nIndex];
            oArray[nIndex] = oArray[0];

            nCurr = 0;
            nLast = nIndex - 1;
            nHalf = nIndex >> 1;
            while (nCurr < nHalf)
            {
                nNext = (nCurr << 1) + 1;
                if (nNext < nLast && (oArray[nNext + 1]->IsBigger(oArray[nNext])))
                    nNext++;
                if (tTemp->IsBiggerOrEqual(oArray[nNext]))
                    break;

                // promote element in chain
                oArray[nCurr] = oArray[nNext];
                nCurr = nNext;
            }

            // restore element at end of chain
            oArray[nCurr] = tTemp;
        }
    }

    void RemoveSpacesAtBothEnds(NSStringUtils::CStringUTF32& oText);

    class CContText
    {
    public:
        enum UnderlineType
        {
            utUnknown,
            utThinLine,
            utThickLine,
            utDoubleThinLine,
            //todo добавить другие типы
        };

    public:
        NSStructures::CFont		m_oFont;
        NSStructures::CBrush	m_oBrush;

        std::wstring m_strPickFontName;
        LONG	m_lPickFontStyle;

        NSStringUtils::CStringUTF32 m_oText;
        NSStringUtils::CStringUTF32 m_oGidText;

        double m_dX;
        double m_dY;
        double m_dWidth;
        double m_dHeight;
        double m_dLastX;

        double m_dWidthWithoutSpaces;
        double m_dLeftWithoutSpaces;

        double m_dPosition;
        double m_dSpaceWidthMM;
        double m_dBaselineOffset;

        double m_dCalculateWidth;
        double m_dSpaceByText;

        bool   m_bIsNeedSpaceAtTheEnd;
        bool   m_bIsHighlightPresent;
        LONG   m_lHighlightColor;

        UnderlineType m_eUnderlineType;

        public:
        CContText();
        ~CContText(){}

        void Clear();

        CContText(const CContText& oSrc);

        CContText& operator=(const CContText& oSrc);
        void CopyFormat(const CContText& oSrc);

        bool IsBigger(const CContText* oSrc);
        bool IsBiggerOrEqual(const CContText* oSrc);

        double GetIntersect(const CContText* oSrc) const;

        void Write(NSStringUtils::CStringBuilder& oWriter,
                   CFontManagerLight* pManagerLight,
                   bool bIsAddSpace = false);

        void WriteTo(double dSpacingMM,
                     NSStringUtils::CStringBuilder& oWriter,
                     CFontManagerLight* pManagerLight);

        void AddSpaceToEnd();
    };

    class CTextLine
    {
    public:
        enum AssumedTextAlignmentType
        {
            atatUnknown,
            atatByLeftEdge,
            atatByCenter,
            atatByRightEdge,
            atatByWidth
        };

        std::vector<CContText*> m_arConts;

        double m_dBaselinePos;
        double m_dBaselineOffset;

        double m_dX;
        double m_dY;
        double m_dWidth;
        double m_dHeight;

        AssumedTextAlignmentType m_eAlignmentType;

    public:
        CTextLine();
        void Clear();

        ~CTextLine();

        CTextLine(const CTextLine& oSrc);
        CTextLine& operator=(const CTextLine& oSrc);

        void AddCont(CContText* pCont, double dBaselineOffset);
        bool IsBigger(const CTextLine* oSrc);
        bool IsBiggerOrEqual(const CTextLine* oSrc);
        void SortConts();

        //Объединяем слова из двух строк
        void Merge(CTextLine* pTextLine);
        //Объединяем подходящие слова в текущей строке, если возможно
        void Analyze();
        bool IsForceBlock();
        void ToXml(NSStringUtils::CStringBuilder& oWriter, CFontManagerLight* pManagerLight);
        //Вычисляем ширину сложной строки
        void CalculateWidth();
        //Добавляем символ пробела в конец строки для связывания строк в параграфе
        void AddSpaceToEnd();
        //Пытаемся понять тип выравнивания для текущей строки
        void DetermineAssumedTextAlignmentType(double dWidthOfPage);
        //Определяем на основании выравнивания подходят ли текущая и следующая строки для добавления в параграф
        bool AreAlignmentsAppropriate(const CTextLine* oSrc);
        //Определяем пересекаются ли линии
        bool AreLinesCrossing(const CTextLine* oSrc);

        //Вычисляем
        double CalculateBeforeSpacing(const double* pPreviousStringOffset);
        double CalculateStringOffset();
        double CalculateRightBorder(const double* pPageWidth);
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

        void CalculateTextAlignmentType();
   };
}

#endif // DOCX_RENDERER_ELEMENT_PARAGRAPH_H
