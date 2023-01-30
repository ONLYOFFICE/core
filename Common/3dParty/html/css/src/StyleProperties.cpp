#include "StyleProperties.h"

#include <iostream>

#include "StaticFunctions.h"
#include "ConstValues.h"

namespace NSCSS
{
	static bool CutImportant(std::wstring& wsValue)
	{
		size_t unBegin = wsValue.find(L"!important");

		if (std::wstring::npos == unBegin)
			return false;

		wsValue.erase(unBegin, 10);

		return true;
	}

	template<typename T>
	CValue<T>::CValue(T oValue, unsigned int unLevel, bool bImportant)
	    : m_oValue(oValue), m_unLevel(unLevel), m_bImportant(bImportant)
	{}

	template<typename T>
	void CValue<T>::Equation(CValue &oFirstValue, CValue &oSecondValue)
	{
		if (oFirstValue.m_bImportant && !oSecondValue.m_bImportant && oFirstValue.Empty())
			oSecondValue.Clear();
		else if (oSecondValue.m_bImportant && !oFirstValue.m_bImportant && !oSecondValue.Empty())
			oFirstValue.Clear();
		else if (!oSecondValue.Empty())
		{
			if (oFirstValue.m_unLevel < oSecondValue.m_unLevel)
				oFirstValue.Clear();
			else
				oSecondValue.Clear();
		}
	}

	CString::CString()
	    : CValue(L"", 0, false)
	{}

	CString::CString(const std::wstring &wsValue, unsigned int unLevel, bool bImportant)
	    : CValue(wsValue, unLevel, bImportant)
	{}

	void CString::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || (m_bImportant && !bHardMode))
			return;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return;

		m_oValue     = wsNewValue;
		m_unLevel    = unLevel;
		m_bImportant = bImportant;
	}

	void CString::SetValue(const std::wstring &wsValue, const std::vector<std::wstring> &arValiableValues, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || arValiableValues.empty() || (m_bImportant && !bHardMode))
			return;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return;

		if (arValiableValues.end() != std::find(arValiableValues.begin(), arValiableValues.end(), wsNewValue))\
		{
			m_oValue     = wsNewValue;
			m_unLevel    = unLevel;
			m_bImportant = bImportant;
		}
	}

	void CString::SetValue(const std::wstring &wsValue, const std::map<std::wstring, std::wstring> &arValiableValues, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || arValiableValues.empty() || (m_bImportant && !bHardMode))
			return;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return;

		std::map<std::wstring, std::wstring>::const_iterator oFoundValue = arValiableValues.find(wsNewValue);

		if (arValiableValues.end() != oFoundValue)
		{
			m_oValue     = oFoundValue->second;
			m_unLevel    = unLevel;
			m_bImportant = bImportant;
		}
	}

	bool CString::Empty() const
	{
		return m_oValue.empty();
	}

	void CString::Clear()
	{
		m_oValue.clear();
	}

	int CString::ToInt() const
	{
		return std::stoi(m_oValue);
	}

	double CString::ToDouble() const
	{
		return std::stod(m_oValue);
	}

	std::wstring CString::ToWString() const
	{
		return m_oValue;
	}

	CDigit::CDigit()
	    : CValue(0., 0, false)
	{}

	CDigit::CDigit(double dValue, unsigned int unLevel, bool bImportant)
	    : CValue(dValue, unLevel, bImportant)
	{}

	bool CDigit::Empty() const
	{
		return 0. == m_oValue;
	}

	void CDigit::Clear()
	{
		m_oValue = 0.;
	}

	int CDigit::ToInt() const
	{
		return static_cast<int>(m_oValue + 0.5);
	}

	double CDigit::ToDouble() const
	{
		return m_oValue;
	}

	std::wstring CDigit::ToWString() const
	{
		return std::to_wstring(m_oValue);
	}

	void CDigit::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || (m_bImportant && !bHardMode))
			return;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return;

		double dValue = 0;

		std::wistringstream(wsValue) >> dValue;

		m_oValue     = dValue;
		m_unLevel    = unLevel;
		m_bImportant = bImportant;
	}

	TRGB CColor::ConvertHEXtoRGB(const std::wstring &wsValue)
	{
		TRGB oRGB;

		if (wsValue.length() == 6)
			swscanf_s(wsValue.c_str(), L"%2hhx%2hhx%2hhx", &oRGB.chRed, &oRGB.chGreen, &oRGB.chBlue);
		else if (wsValue.length() == 3)
		{
			swscanf_s(wsValue.c_str(), L"%1hhx%1hhx%1hhx", &oRGB.chRed, &oRGB.chGreen, &oRGB.chBlue);
			oRGB.chRed   *= 17;
			oRGB.chGreen *= 17;
			oRGB.chBlue  *= 17;
		}

		return oRGB;
	}

	std::wstring CColor::ConvertRGBtoHEX(const TRGB &oValue)
	{
		wchar_t arTemp[6];

		swprintf(arTemp, sizeof(arTemp), L"%2hhX%2hhX%2hhX", oValue.chRed, oValue.chGreen, oValue.chBlue);

		return std::wstring(arTemp);
	}

	CColor::CColor()
	    : CValue({0, 0, 0}, 0, false)
	{}

	CColor::CColor(const TRGB &oValue, unsigned int unLevel, bool bImportant)
	    : CValue(oValue, unLevel, bImportant)
	{}

	void CColor::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || (m_bImportant && !bHardMode))
			return;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return;

		if (wsValue[0] == L'#')
		{
			m_oValue = ConvertHEXtoRGB(wsValue.substr(1, wsValue.length() - 1));
		}
		else if (wsValue.substr(0, 3) == L"rgb")
		{
			size_t unEnd = wsValue.find(L')', 4);

			if (std::wstring::npos == unEnd)
				return;

			std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsNewValue.substr(4, unEnd - 3));

			if (3 > arValues.size())
				return;

			m_oValue.chRed   = std::stoi(arValues[0]);
			m_oValue.chGreen = std::stoi(arValues[1]);
			m_oValue.chBlue  = std::stoi(arValues[2]);
		}
		else
		{
			std::wstring sNewColor = wsValue;
			std::transform(sNewColor.begin(), sNewColor.end(), sNewColor.begin(), towlower);

			if (sNewColor == L"transparent")
				return;

			const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSConstValues::NSMaps::mColors.find(sNewColor);
			if (oHEX != NSConstValues::NSMaps::mColors.end())
				m_oValue = ConvertHEXtoRGB(oHEX->second);
		}
	}

	bool CColor::Empty() const
	{
		return 0. == m_oValue.chRed && 0 == m_oValue.chGreen && 0 == m_oValue.chBlue;
	}

	void CColor::Clear()
	{
		m_oValue.chRed = m_oValue.chGreen = m_oValue.chBlue = 0;
	}

	int CColor::ToInt() const
	{
		return RGB_TO_INT(m_oValue.chRed, m_oValue.chGreen, m_oValue.chBlue);
	}

	double CColor::ToDouble() const
	{
		return RGB_TO_INT(m_oValue.chRed, m_oValue.chGreen, m_oValue.chBlue);
	}

	std::wstring CColor::ToWString() const
	{
		return ConvertRGBtoHEX(m_oValue);
	}

	TRGB CColor::ToRGB() const
	{
		return m_oValue;
	}

	std::wstring CColor::GetURL() const
	{
		return m_wsURL;
	}

	// DISPLAY
	CDisplay::CDisplay()
	    : m_oDisplay(L"inline", 0)
	{}

	void CDisplay::Equation(CDisplay &oFirstDisplay, CDisplay &oSecondDisplay)
	{
		CDigit::Equation(oFirstDisplay.m_oX, oSecondDisplay.m_oX);
		CDigit::Equation(oFirstDisplay.m_oY, oSecondDisplay.m_oY);
		CDigit::Equation(oFirstDisplay.m_oWidth, oSecondDisplay.m_oWidth);
		CDigit::Equation(oFirstDisplay.m_oHeight, oSecondDisplay.m_oHeight);

		CString::Equation(oFirstDisplay.m_oVAlign, oSecondDisplay.m_oVAlign);
		CString::Equation(oFirstDisplay.m_oHAlign, oSecondDisplay.m_oHAlign);

		CString::Equation(oFirstDisplay.m_oDisplay, oSecondDisplay.m_oDisplay);
	}

	void CDisplay::SetX(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		m_oX.SetValue(wsValue, unLevel, bHardMode);
	}

	void CDisplay::SetY(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		m_oY.SetValue(wsValue, unLevel, bHardMode);
	}

	void CDisplay::SetWidth(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		m_oWidth.SetValue(wsValue, unLevel, bHardMode);
	}

	void CDisplay::SetHeight(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		m_oHeight.SetValue(wsValue, unLevel, bHardMode);
	}

	void CDisplay::SetHAlign(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		m_oHAlign.SetValue(wsValue, {L"center", L"left", L"start", L"right", L"end"}, unLevel, bHardMode);
	}

	void CDisplay::SetVAlign(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		m_oVAlign.SetValue(wsValue, {std::make_pair(L"top", L"top"), std::make_pair(L"baseline", L"top"), std::make_pair(L"text-top", L"top"), std::make_pair(L"bottom", L"bottom"), std::make_pair(L"text-bottom", L"bottom"), std::make_pair(L"middle", L"middle")}, unLevel, bHardMode);
	}

	void CDisplay::SetDisplay(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		m_oHAlign.SetValue(wsValue, NSConstValues::arDisplayValues, unLevel, bHardMode);
	}

	double CDisplay::GetX() const
	{
		return m_oX.ToDouble();
	}

	double CDisplay::GetY() const
	{
		return m_oY.ToDouble();
	}

	double CDisplay::GetWidth() const
	{
		return m_oWidth.ToDouble();
	}

	double CDisplay::GetHeight() const
	{
		return m_oHeight.ToDouble();
	}

	std::wstring CDisplay::GetHAlign() const
	{
		return m_oHAlign.ToWString();
	}

	std::wstring CDisplay::GetVAlign() const
	{
		return m_oVAlign.ToWString();
	}

	std::wstring CDisplay::GetDisplay() const
	{
		return m_oDisplay.ToWString();
	}

	// STROKE
	CStroke::CStroke()
	{}

	void CStroke::Equation(CStroke &oFirstStroke, CStroke &oSecondStroke)
	{
		CColor::Equation(oFirstStroke.m_oColor, oSecondStroke.m_oColor);
		CDigit::Equation(oFirstStroke.m_oWidth, oSecondStroke.m_oWidth);
	}

	void CStroke::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		m_oColor.SetValue(wsValue, unLevel, bHardMode);
	}

	void CStroke::SetWidth(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		m_oWidth.SetValue(wsValue, unLevel, bHardMode);
	}

	std::wstring CStroke::GetColor() const
	{
		return m_oColor.ToWString();
	}

	double CStroke::GetWidth() const
	{
		return m_oWidth.ToDouble();
	}
}
