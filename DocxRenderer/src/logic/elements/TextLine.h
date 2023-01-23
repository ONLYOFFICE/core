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

            AssumedTextAlignmentType m_eAlignmentType {atatUnknown};

            eVertAlignType m_eVertAlignType {eVertAlignType::vatUnknown};

            const CShape* m_pDominantShape {nullptr};

            UINT m_iNumDuplicates {0};
        public:
            CTextLine();
            void Clear() override final;

            ~CTextLine();

            void AddCont(CContText *pCont);
            bool IsBigger(const CBaseItem* oSrc) override final;
            bool IsBiggerOrEqual(const CBaseItem* oSrc) override final;
            void SortConts();

            //Объединяем слова из двух строк
            void Merge(CTextLine* pLine);
            bool IsForceBlock();
            void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

            void MergeConts();
            //Вычисляем ширину сложной строки
            void CalculateWidth();
            //Пытаемся понять тип выравнивания для текущей строки
            void DetermineAssumedTextAlignmentType(double dWidthOfPage);
            //Определяем на основании выравнивания подходят ли текущая и следующая строки для добавления в параграф
            bool AreAlignmentsAppropriate(const CTextLine* pLine);

            void SetVertAlignType(const eVertAlignType& oType);

            //Вычисляем
            double CalculateBeforeSpacing(double dPreviousStringBaseline);
            double CalculateRightBorder(const double& dPageWidth);
    };
}
