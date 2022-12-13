#include "CStyle.h"

#include "../SvgUtils.h"

namespace SVG
{
	CStyle::CStyle()
	{

	}

	CStyle::~CStyle()
	{

	}

	void CStyle::SetStyle(const std::vector<NSCSS::CNode> &arXmlNodes)
	{
		NSCSS::CCompiledStyle oCompiledStyle = m_oCssCalculator.GetCompiledStyle(arXmlNodes);

		m_lStrokeColor = m_oColorParser.ColorFromString(L'#' + oCompiledStyle.m_pText.GetColor());
		m_lFillColor = m_oColorParser.ColorFromString(L'#' + oCompiledStyle.m_pBackground.GetColorHex());
	}

	void CStyle::SetStyle(const std::wstring &wsStyle)
	{
		std::wstring::size_type unFind = wsStyle.find(L"stroke:");

		if (std::wstring::npos != unFind)
		{
			m_lStrokeColor = m_oColorParser.ColorFromString(L"rgb(200, 0, 0)");
		}
	}

	void CStyle::SetStrokeColor(const std::wstring &wsStrokeColor)
	{
		m_lStrokeColor = m_oColorParser.ColorFromString(wsStrokeColor);
	}

	long CStyle::GetStrokeColor() const
	{
		return m_lStrokeColor;
	}

	long CStyle::GetFillL() const
	{
		return m_lFillColor;
	}
}
