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
		else if (L"stroke" == oStyle.first)
		{
			const size_t unPositionImp = oStyle.second.find(L"!i");
			if (unPositionImp == std::wstring::npos)
			{
				m_pStroke.SetColor(oStyle.second, unLevel, bHardMode);
			}
			else if (unPositionImp != 0)
			{
				m_pStroke.SetColor(oStyle.second.substr(0, unPositionImp - 1), unLevel, true);
				m_pStroke.SetImportantColor(true);
			}
		}
		else if (L"stroke-width" == oStyle.first)
		{
			const size_t unPositionImp = oStyle.second.find(L"!i");
			if (unPositionImp == std::wstring::npos)
			{
				const double dValue = wcstof(ConvertUnitMeasure(oStyle.second, 0.0f).c_str(), NULL);
				m_pStroke.SetWidth(dValue, unLevel, bHardMode);
			}
			else if (unPositionImp != 0)
			{
				const double dValue = wcstof(ConvertUnitMeasure(oStyle.second.substr(0, unPositionImp - 1), 0.0f).c_str(), NULL);
				m_pStroke.SetWidth(dValue, unLevel, true);
				m_pStroke.SetImportantWidth(true);
			}
		}
		else if (L"transform" == oStyle.first)
		{
			const size_t unPositionImp = oStyle.second.find(L"!i");
			if (unPositionImp == std::wstring::npos)
			{
				m_pTransform.SetTransform(StrUtils::ReadDoubleValues(oStyle.second), m_pTransform.GetTransformType(oStyle.second), unLevel, bHardMode);
			}
			else if (unPositionImp != 0)
			{
				std::wstring wsTempValue = oStyle.second.substr(0, unPositionImp - 1);
				m_pTransform.SetTransform(StrUtils::ReadDoubleValues(wsTempValue), m_pTransform.GetTransformType(wsTempValue), unLevel, true);
				m_pTransform.SetImportantTransform(true);
			}
		}
	}

	NSCSS::NSConstValues::NSCssProperties::ColorType CStyle::GetFillType() const
	{
		return m_pBackground.GetColorType();
	}

	std::wstring CStyle::GetFill() const
	{
		return m_pBackground.GetColorHex();
	}

	int CStyle::GetFillN() const
	{
		return m_pBackground.GetColorN();
	}

	NSCSS::NSConstValues::NSCssProperties::ColorType CStyle::GetStrokeColorType() const
	{
		return m_pStroke.GetColorType();
	}

	double CStyle::GetStrokeWidth() const
	{
		return m_pStroke.GetWidth();
	}

	int CStyle::GetStrokeColorN() const
	{
		return m_pStroke.GetColorN();
	}

	void CStyle::GetTransform(double &dM11, double &dM12, double &dM21, double &dM22, double &dDx, double &dDy) const
	{
		m_pTransform.GetTransform(dM11, dM12, dM21, dM22, dDx, dDy);
	}

}
