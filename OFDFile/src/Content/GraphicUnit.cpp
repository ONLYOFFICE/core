#include "GraphicUnit.h"
#include "../../../OOXML/Base/Unit.h"

namespace OFD
{
CGraphicUnit::CGraphicUnit(CXmlReader& oLiteReader)
	: m_bVisible(true), m_dLineWidth(0.353), m_eCap(ECap::Butt),
	  m_eJoin(EJoin::Miter), m_dMiterLimit(4.234), m_dDashOffset(0.),
	  m_uchAlpha(255)
{
	if (0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	std::wstring wsAttributeName;

	do
	{
		wsAttributeName = oLiteReader.GetName();

		if (L"Boundary" == wsAttributeName)
			m_oBoundary.Read(oLiteReader.GetTextA());
		else if (L"Name" == wsAttributeName)
			m_wsName = oLiteReader.GetText();
		else if (L"Visible" == wsAttributeName)
			m_bVisible = oLiteReader.GetBoolean(true);
		else if (L"CTM" == wsAttributeName)
			m_oCTM.Read(oLiteReader.GetTextA());
		else if (L"DrawParam" == wsAttributeName)
			m_unDrawParam = oLiteReader.GetUInteger(true);
		else if (L"LineWidth" == wsAttributeName)
			m_dLineWidth = oLiteReader.GetDouble(true);
		else if (L"Cap" == wsAttributeName)
		{
			const std::wstring wsValue{oLiteReader.GetText()};

			if (L"Butt" == wsValue)
				m_eCap = ECap::Butt;
			else if (L"Round" == wsValue)
				m_eCap = ECap::Round;
			else if (L"Square" == wsValue)
				m_eCap = ECap::Square;
		}
		else if (L"Join" == wsAttributeName)
		{
			const std::wstring wsValue{oLiteReader.GetText()};

			if (L"Miter" == wsValue)
				m_eJoin = EJoin::Miter;
			else if (L"Round" == wsValue)
				m_eJoin = EJoin::Round;
			else if (L"Bevel" == wsValue)
				m_eJoin = EJoin::Bevel;
		}
		else if (L"MiterLimit" == wsAttributeName)
			m_dMiterLimit = oLiteReader.GetDouble(true);
		else if (L"DashOffset" == wsAttributeName)
			m_dDashOffset = oLiteReader.GetDouble(true);
		else if (L"Alpha" == wsAttributeName)
			m_uchAlpha = oLiteReader.GetUInteger(true);
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();
}

}
