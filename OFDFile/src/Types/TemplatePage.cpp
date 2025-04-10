#include "TemplatePage.h"

#include "../Page.h"
#include "../Utils/Utils.h"

namespace OFD
{
CTemplatePage::CTemplatePage(CXmlReader& oXmlReader, const std::wstring& wsRootPath)
	: IOFDElement(oXmlReader), m_eZOrder(EZOrder::Background), m_pPage(nullptr)
{
	if ("ofd:TemplatePage" != oXmlReader.GetNameA() || 0 == oXmlReader.GetAttributesCount() || !oXmlReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;
	do
	{
		sAttributeName = oXmlReader.GetNameA();

		if ("BaseLoc" == sAttributeName)
			m_pPage = CPage::Read(CombinePaths(wsRootPath, oXmlReader.GetText()));
		else if ("ZOrder" == sAttributeName)
			m_eZOrder = GetZOrderFromString(oXmlReader.GetTextA());
	} while (oXmlReader.MoveToNextAttribute());

	oXmlReader.MoveToElement();
}

CTemplatePage::~CTemplatePage()
{
	if (nullptr != m_pPage)
		delete m_pPage;
}

EZOrder CTemplatePage::GetZOrder() const
{
	return m_eZOrder;
}

const CPage* CTemplatePage::GetPage() const
{
	return m_pPage;
}

EZOrder GetZOrderFromString(const std::string& sValue)
{
	if ("Body" == sValue)
		return EZOrder::Body;
	if ("Background" == sValue)
		return EZOrder::Background;

	return EZOrder::Body;
}

}
