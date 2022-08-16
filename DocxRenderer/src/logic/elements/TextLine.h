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

            std::vector<std::shared_ptr<CContText>> m_arConts;

            double m_dBaselineOffset {0.0};

            AssumedTextAlignmentType m_eAlignmentType {atatUnknown};

            eVertAlignType m_eVertAlignType {eVertAlignType::vatUnknown};

            std::shared_ptr<CShape> m_pDominantShape {nullptr};
#if (USING_DELETE_DUPLICATING_CONTS == 0)
            std::shared_ptr<CTextLine> m_pDuplicateLine {nullptr};
#endif
        public:
            CTextLine();
            void Clear() override final;

            ~CTextLine();

            void AddCont(const std::shared_ptr<CContText> pCont);
            bool IsBigger(const CBaseItem* oSrc) override final;
            bool IsBiggerOrEqual(const CBaseItem* oSrc) override final;
            void SortConts();

            //Объединяем слова из двух строк
            void Merge(const std::shared_ptr<CTextLine> pLine);
            bool IsForceBlock();
            void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

            void MergeConts();
            //Вычисляем ширину сложной строки
            void CalculateWidth();
            //Пытаемся понять тип выравнивания для текущей строки
            void DetermineAssumedTextAlignmentType(double dWidthOfPage);
            //Определяем на основании выравнивания подходят ли текущая и следующая строки для добавления в параграф
            bool AreAlignmentsAppropriate(const std::shared_ptr<CTextLine> pLine);

            void SetVertAlignType(const eVertAlignType& oType);

            //Вычисляем
            double CalculateBeforeSpacing(double dPreviousStringOffset);
            double CalculateStringOffset();
            double CalculateRightBorder(const double& dPageWidth);
            double RightBorderCorrection();
    };
}
