#pragma once
#include "ContText.h"
#include "BaseItem.h"


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

		AssumedTextAlignmentType m_eAlignmentType{atatUnknown};
		eVertAlignType m_eVertAlignType          {eVertAlignType::vatUnknown};

		std::shared_ptr<CTextLine> m_pLine;
		std::shared_ptr<CShape>  m_pDominantShape {nullptr};

		UINT m_iNumDuplicates {0};

		double m_dTopWithMaxAscent{0};
		double m_dBotWithMaxDescent{0};

	public:
		CTextLine() = default;
		virtual ~CTextLine();
		virtual void Clear() override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void RecalcWithNewItem(const CContText* pCont);
		virtual eVerticalCrossingType GetVerticalCrossingType(const CTextLine* pLine) const noexcept;

		void AddCont(std::shared_ptr<CContText> pCont);
		void MergeConts();
		void RecalcSizes();
		void SetVertAlignType(const eVertAlignType& oType);

		bool IsShadingPresent(const CTextLine* pLine) const noexcept;
		bool IsCanBeDeleted() const;

		size_t GetLength() const;
	};
}
