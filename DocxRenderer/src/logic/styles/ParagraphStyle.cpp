#include "ParagraphStyle.h"

namespace NSDocxRenderer
{
	CParagraphStyle::CParagraphStyle()
	{
	}
	CParagraphStyle::CParagraphStyle(const std::wstring& wsStyleId, const std::wstring& wsName)
	{
		this->wsStyleId = wsStyleId;
		this->wsName = wsName;
	}
	CParagraphStyle::~CParagraphStyle()
	{
	}

	void CParagraphStyle::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		oWriter.WriteString(L"<w:style w:type=\"paragraph\" ");
		if(bIsDefault) oWriter.WriteString(L"w:default=\"1\" ");
		oWriter.WriteString(L"w:styleId=\"" + wsStyleId + L"\">");
		oWriter.WriteString(L"<w:name w:val=\"" + wsName + L"\"/>");

		if(!wsBasedOn.empty()) oWriter.WriteString(L"<w:basedOn w:val=\"" + wsBasedOn + L"\"/>");
		if(bIsUnhideWhenUsed) oWriter.WriteString(L"<w:unhideWhenUsed/>");
		if(bIsSemiHidden) oWriter.WriteString(L"<w:semiHidden/>");
		oWriter.WriteString(L"<w:uiPriority w:val=\"" + std::to_wstring(nUiPriority) + L"\"/>");
		oWriter.WriteString(L"<w:qFormat/>");

		oWriter.WriteString(L"</w:style>");
	}
}

