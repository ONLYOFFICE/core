#include "ParagraphStyleManager.h"


namespace NSDocxRenderer
{
	CParagraphStyleManager::CParagraphStyleManager()
	{
		// стандартные стили
		CParagraphStyle oNormal(L"Normal", L"Normal");
		CParagraphStyle oHeading1(L"Heading1", L"Heading 1");
		CParagraphStyle oHeading2(L"Heading2", L"Heading 2");

		oNormal.bIsDefault = true;

		oHeading1.wsBasedOn = oNormal.wsStyleId;
		oHeading1.nUiPriority = 9;

		oHeading2.wsBasedOn = oNormal.wsStyleId;
		oHeading2.nUiPriority = 9;

		m_arDefaultParagraphStyles.push_back(oNormal);
		m_arDefaultParagraphStyles.push_back(oHeading1);
		m_arDefaultParagraphStyles.push_back(oHeading2);

	}
	CParagraphStyleManager::~CParagraphStyleManager()
	{
		m_arDefaultParagraphStyles.clear();
	}

	std::wstring CParagraphStyleManager::GetDefaultParagraphStyleId(const CParagraph& oParagraph) const noexcept
	{
		if(oParagraph.m_arLines.size() > 1) return L"Normal";

		bool isHeading = true;
		for(auto& val : oParagraph.m_arLines[0]->m_arConts)
			if(val && val->m_pFontStyle->dFontSize <= m_dAvgFontSize + 1 && !val->m_pFontStyle->bBold)
				isHeading = false;

		return isHeading ? L"Heading1" : L"Normal";
	}
	double CParagraphStyleManager::GetAvgFontSize() const noexcept
	{
		return m_dAvgFontSize;
	}
	void CParagraphStyleManager::UpdateAvgFontSize(double dFontSize)
	{
		m_dAvgFontSize = (m_dAvgFontSize / (m_nN + 1)) * m_nN + (dFontSize / (m_nN + 1));
		m_nN++;
	}

	void CParagraphStyleManager::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		for(auto& val : m_arDefaultParagraphStyles)
			val.ToXml(oWriter);
	}
}

