#include "CStyle.h"

#include "CObjectBase.h"
#include "../SvgUtils.h"

namespace SVG
{
	CGeneralStyle::CGeneralStyle()
	{
		m_oCssCalculator.SetUnitMeasure(NSCSS::UnitMeasure::Pixel);
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

	void CGeneralStyle::SetStyle(CObjectBase &oObject)
	{

	}

	CStyle::CStyle()
	{}

	CStyle::~CStyle()
	{

	}

	void CStyle::AddOtherStyle(const std::pair<std::wstring, std::wstring> &oStyle, const unsigned int unLevel, const bool &bHardMode)
	{
		if (L"fill" == oStyle.first)
		{
			m_oBackground.SetColor(oStyle.second, unLevel, bHardMode);
		}
		else if (L"stroke" == oStyle.first)
		{
			m_oStroke.SetColor(oStyle.second, unLevel, bHardMode);
		}
		else if (L"stroke-width" == oStyle.first)
		{
			m_oStroke.SetWidth(ConvertUnitMeasure(oStyle.second, 0.0f), unLevel, bHardMode);
		}
		else if (L"transform" == oStyle.first)
		{
			m_oTransform.SetMatrix(oStyle.second, unLevel, bHardMode);
		}
		else if (L"x" == oStyle.first || L"cx" == oStyle.first)
		{
			m_oDisplay.SetX(ConvertUnitMeasure(oStyle.second, m_oDisplay.GetWidth().ToDouble(), NSCSS::ScalingDirectionX), unLevel, bHardMode);
		}
		else if (L"y" == oStyle.first || L"cy" == oStyle.first)
		{
			m_oDisplay.SetY(ConvertUnitMeasure(oStyle.second, m_oDisplay.GetWidth().ToDouble(), NSCSS::ScalingDirectionY), unLevel, bHardMode);
		}
		else if (L"dx" == oStyle.first)
		{
			m_oDisplay.AddX(ConvertUnitMeasure(oStyle.second, m_oDisplay.GetWidth().ToDouble(), NSCSS::ScalingDirectionX), unLevel, bHardMode);
		}
		else if (L"dy" == oStyle.first)
		{
			m_oDisplay.AddY(ConvertUnitMeasure(oStyle.second, m_oDisplay.GetWidth().ToDouble(), NSCSS::ScalingDirectionY), unLevel, bHardMode);
		}
	}
}
