#include "PenSettings.h"

namespace OFD
{
CPenSettings::CPenSettings(CXmlReader& oLiteReader)
	: m_dLineWidth(0.353), m_eCap(ECap::Butt),
	  m_eJoin(EJoin::Miter), m_dMiterLimit(4.234), m_dDashOffset(0.)
{
	if (0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	std::wstring wsAttributeName;

	do
	{
		wsAttributeName = oLiteReader.GetName();

		if (L"LineWidth" == wsAttributeName)
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

void CPenSettings::Apply(IRenderer* pRenderer) const
{
	pRenderer->put_PenSize(m_dLineWidth);
	pRenderer->put_PenAlpha(m_uchAlpha);
	pRenderer->put_BrushAlpha1(m_uchAlpha);

	BYTE nCapStyle = 0;
	switch (m_eCap)
	{
		case ECap::Butt:   nCapStyle = Aggplus::LineCapFlat;   break;
		case ECap::Round:  nCapStyle = Aggplus::LineCapRound;  break;
		case ECap::Square: nCapStyle = Aggplus::LineCapSquare; break;
	}

	pRenderer->put_PenLineStartCap(nCapStyle);
	pRenderer->put_PenLineEndCap(nCapStyle);

	BYTE nJoinStyle = 0;
	switch (m_eJoin)
	{
		case EJoin::Miter: nJoinStyle = Aggplus::LineJoinMiter; break;
		case EJoin::Round: nJoinStyle = Aggplus::LineJoinRound; break;
		case EJoin::Bevel: nJoinStyle = Aggplus::LineJoinBevel; break;
	}

	pRenderer->put_PenLineJoin(nJoinStyle);

	if (!m_arDashPattern.empty())
	{
		pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
		pRenderer->put_PenDashOffset(m_dDashOffset);
		pRenderer->PenDashPattern((double*)m_arDashPattern.data(), m_arDashPattern.size());
	}
}
}
