#pragma once
#include "ContText.h"

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

            double m_dBaselineOffset {0.0};

            AssumedTextAlignmentType m_eAlignmentType {atatUnknown};

            eVertAlignType m_eVertAlignType {eVertAlignType::vatUnknown};

            const CShape* m_pDominantShape {nullptr};
#if USING_DELETE_DUPLICATING_CONTS == 0
            CTextLine* m_pDuplicateLine {nullptr};
#endif
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
            bool IsForceBlock();
            void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

            void MergeConts();
            //Вычисляем ширину сложной строки
            void CalculateWidth();
            //Пытаемся понять тип выравнивания для текущей строки
            void DetermineAssumedTextAlignmentType(double dWidthOfPage);
            //Определяем на основании выравнивания подходят ли текущая и следующая строки для добавления в параграф
            bool AreAlignmentsAppropriate(const CTextLine* oSrc);

            void SetVertAlignType(const eVertAlignType& oType);

            //Вычисляем
            double CalculateBeforeSpacing(const double* pPreviousStringOffset);
            double CalculateStringOffset();
            double CalculateRightBorder(const double& dPageWidth);
            double RightBorderCorrection();
    };
}
