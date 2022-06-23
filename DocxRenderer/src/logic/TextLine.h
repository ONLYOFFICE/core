#pragma once
#include "ContText.h"

namespace NSDocxRenderer
{
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

            void AddCont(CContText* pCont);
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
            double CalculateRightBorder(const double& dPageWidth);
    };
}
