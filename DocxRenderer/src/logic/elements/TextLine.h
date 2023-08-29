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
		CTextLine* m_pLine {nullptr};  //Если не nullptr, то есть привязка к vatSubscript или vatSuperscript;
		CShape* m_pDominantShape {nullptr};
		UINT m_iNumDuplicates {0};

	public:
		CTextLine();
		virtual ~CTextLine();
		virtual void Clear() override final;
		virtual void AddContent(CBaseItem* pObj) override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

		void CheckLineToNecessaryToUse();
		void MergeConts();
		void RecalcSizes();
		void SetVertAlignType(const eVertAlignType& oType);
		bool IsShadingPresent(const CTextLine* pLine);
	};
}
