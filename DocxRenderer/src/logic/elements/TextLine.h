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

            CTextLine* m_pLine {nullptr};
            CShape* m_pDominantShape {nullptr};

            UINT m_iNumDuplicates {0};
        public:
            CTextLine();
            virtual ~CTextLine();
            virtual void Clear() override final;
            virtual bool IsBigger(const CBaseItem* oSrc) override final;
            virtual bool IsBiggerOrEqual(const CBaseItem* oSrc) override final;
            virtual void AddContent(CBaseItem* pObj) override final;
            virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

            void SortConts();

            void CheckLineToNecessaryToUse();

            //Объединяем слова из двух строк
            void Merge(CTextLine* pLine);
            bool IsForceBlock();

            void MergeConts();
            //Вычисляем ширину сложной строки
            void CalculateWidth();
            //Определяем на основании выравнивания подходят ли текущая и следующая строки для добавления в параграф
            bool AreAlignmentsAppropriate(const CTextLine* pLine);

            void SetVertAlignType(const eVertAlignType& oType);

            //Вычисляем
            double CalculateBeforeSpacing(double dPreviousStringBaseline);
            double CalculateRightBorder(const double& dPageWidth);
    };
}
