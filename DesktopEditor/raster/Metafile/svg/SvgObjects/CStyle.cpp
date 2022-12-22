#include "CStyle.h"

#include "../SvgUtils.h"

namespace SVG
{
	CGeneralStyle::CGeneralStyle()
	{

	}

	CGeneralStyle::~CGeneralStyle()
	{

	}

	void CGeneralStyle::AddStyle(const std::wstring &wsStyle)
	{
		m_oCssCalculator.AddStyles(wsStyle);
	}

	CStyle CGeneralStyle::GetStyle(const std::vector<NSCSS::CNode>& arNodes) const
	{
		CStyle oStyle;
		m_oCssCalculator.GetCompiledStyle(oStyle, arNodes);

		return oStyle;
	}

	CStyle::CStyle()
	{
		m_pBackground.SavebWithoutProcessing();
	}

	CStyle::~CStyle()
	{

	}

	void CStyle::AddOtherStyle(const std::pair<std::wstring, std::wstring> &oStyle, const unsigned int unLevel, const bool &bHardMode)
	{
		if (L"fill" == oStyle.first)
		{
			const size_t unPositionImp = oStyle.second.find(L"!i");
			if (unPositionImp == std::wstring::npos)
			{
				m_pBackground.SetColor(oStyle.second, unLevel, bHardMode);
			}
			else if (unPositionImp != 0)
			{
				m_pBackground.SetColor(oStyle.second.substr(0, unPositionImp - 1), unLevel, true);
				m_pBackground.SetImportantBackground(true);
			}
		}
	}

	std::wstring CStyle::GetFill() const
	{
		return m_pBackground.GetColorHex();
	}

}
