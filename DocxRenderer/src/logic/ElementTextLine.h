#pragma once
#include "ElementContText.h"

namespace NSDocxRenderer
{
    class CTextLine : public CBaseItem
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

            double m_dBaselinePos {0.0};
            double m_dBaselineOffset {0.0};

            AssumedTextAlignmentType m_eAlignmentType {atatUnknown};

            eVertAlignType m_eVertAlignType {eVertAlignType::vatUnknown};

            const CShape* m_pDominantShape {nullptr};

        public:
            CTextLine();
            void Clear() override final;

            ~CTextLine();

            CTextLine(const CTextLine& oSrc);
            CTextLine& operator=(const CTextLine& oSrc);

            void AddCont(CContText* pCont);
            bool IsBigger(const CBaseItem* oSrc) override final;
            bool IsBiggerOrEqual(const CBaseItem* oSrc) override final;
            void SortConts();

            //Объединяем слова из двух строк
            void Merge(const CTextLine* pTextLine);
            //Объединяем подходящие слова в текущей строке, если возможно
            void Analyze();
            bool IsForceBlock();
            void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

            //Вычисляем ширину сложной строки
            void CalculateWidth();
            //Добавляем символ пробела в конец строки для связывания строк в параграфе
            void AddSpaceToEnd();
            //Пытаемся понять тип выравнивания для текущей строки
            void DetermineAssumedTextAlignmentType(double dWidthOfPage);
            //Определяем на основании выравнивания подходят ли текущая и следующая строки для добавления в параграф
            bool AreAlignmentsAppropriate(const CTextLine* oSrc);
            //Определяем пересекаются ли линии
            CrossingType GetCrossingType(const CTextLine* oSrc);

            void SetVertAlignType(const eVertAlignType& oType);

            //Вычисляем
            double CalculateBeforeSpacing(const double* pPreviousStringOffset);
            double CalculateStringOffset();
            double CalculateRightBorder(const double& dPageWidth);
            double RightBorderCorrection();
    };
}
