#include "StyleProperties.h"

#include <iostream>

#include "StaticFunctions.h"
#include "ConstValues.h"

namespace NSCSS
{
	namespace NSProperties
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
	CValue<T>::CValue(const T& oValue, unsigned int unLevel, bool bImportant)
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

	template<typename T>
	bool CValue<T>::operator==(const T &oValue) const
	{
		return m_oValue == oValue;
	}

	template<typename T>
	bool CValue<T>::operator>=(const T &oValue) const
	{
		return m_oValue >= oValue;
	}

	template<typename T>
	bool CValue<T>::operator<=(const T &oValue) const
	{
		return m_oValue <= oValue;
	}

	template<typename T>
	bool CValue<T>::operator>(const T &oValue) const
	{
		return m_oValue > oValue;
	}

	template<typename T>
	bool CValue<T>::operator<(const T &oValue) const
	{
		return m_oValue < oValue;
	}

	template<typename T>
	CValue<T>& CValue<T>::operator=(const CValue &oValue)
	{
		m_oValue     = oValue.m_oValue;
		m_unLevel    = oValue.m_unLevel;
		m_bImportant = oValue.m_bImportant;

		return *this;
	}

	template<typename T>
	CValue<T>& CValue<T>::operator=(const T &oValue)
	{
		m_oValue = oValue.m_oValue;

		return *this;
	}

	template<typename T>
	CValue<T> &CValue<T>::operator+=(const CValue<T> &oValue)
	{
		if (m_unLevel > oValue.m_unLevel || (m_bImportant && !oValue.m_bImportant) || oValue.Empty())
			return *this;

		m_oValue     = oValue.m_oValue;
		m_unLevel    = std::max(m_unLevel, oValue.m_unLevel);
		m_bImportant = std::max(m_bImportant, oValue.m_bImportant);

		return *this;
	}

	template<typename T>
	bool CValue<T>::operator==(const CValue<T> &oValue) const
	{
		return m_oValue == oValue.m_oValue;
	}

	CString::CString()
	    : CValue(L"", 0, false)
	{}

	CString::CString(const std::wstring &wsValue, unsigned int unLevel, bool bImportant)
	    : CValue(wsValue, unLevel, bImportant)
	{}

	bool CString::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || (m_bImportant && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return false;

		m_oValue     = wsNewValue;
		m_unLevel    = unLevel;
		m_bImportant = bImportant;

		return true;
	}

	bool CString::SetValue(const std::wstring &wsValue, const std::vector<std::wstring> &arValiableValues, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || arValiableValues.empty() || (m_bImportant && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return false;

		if (arValiableValues.end() != std::find(arValiableValues.begin(), arValiableValues.end(), wsNewValue))\
		{
			m_oValue     = wsNewValue;
			m_unLevel    = unLevel;
			m_bImportant = bImportant;

			return true;
		}

		return false;
	}

	bool CString::SetValue(const std::wstring &wsValue, const std::map<std::wstring, std::wstring> &arValiableValues, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || arValiableValues.empty() || (m_bImportant && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return false;

		std::map<std::wstring, std::wstring>::const_iterator oFoundValue = arValiableValues.find(wsNewValue);

		if (arValiableValues.end() != oFoundValue)
		{
			m_oValue     = oFoundValue->second;
			m_unLevel    = unLevel;
			m_bImportant = bImportant;

			return true;
		}

		return false;
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

	CString &CString::operator+=(const CString &oString)
	{
		if (m_unLevel > oString.m_unLevel || (m_bImportant && !oString.m_bImportant) || oString.m_oValue.empty())
			return *this;

		m_oValue     = oString.m_oValue;
		m_unLevel    = oString.m_unLevel;
		m_bImportant = oString.m_bImportant;

		return *this;
	}

	CDigit::CDigit()
	    : CValue(DBL_MIN, 0, false)
	{}

	CDigit::CDigit(double dValue, unsigned int unLevel, bool bImportant)
	    : CValue(dValue, unLevel, bImportant)
	{}

	bool CDigit::Empty() const
	{
		return DBL_MIN == m_oValue;
	}

	void CDigit::Clear()
	{
		m_oValue = DBL_MIN;
	}

	int CDigit::ToInt() const
	{
		if (DBL_MIN == m_oValue)
			return 0;

		return static_cast<int>(m_oValue + 0.5);
	}

	double CDigit::ToDouble() const
	{
		if (DBL_MIN == m_oValue)
			return 0.;

		return m_oValue;
	}

	std::wstring CDigit::ToWString() const
	{
		if (DBL_MIN == m_oValue)
			return std::wstring();

		return std::to_wstring(m_oValue);
	}

	CDigit CDigit::operator+(const CDigit &oDigit) const
	{
		CDigit oTemp;

		oTemp.m_oValue     = m_oValue + oDigit.m_oValue;
		oTemp.m_unLevel    = std::max(m_unLevel, oDigit.m_unLevel);
		oTemp.m_bImportant = std::max(m_bImportant, oDigit.m_bImportant);

		return oTemp;
	}

	CDigit &CDigit::operator+=(const CDigit &oDigit)
	{
		if (m_unLevel > oDigit.m_unLevel || (m_bImportant && !oDigit.m_bImportant) || DBL_MIN == oDigit.m_oValue)
			return *this;

		m_oValue     = oDigit.m_oValue;
		m_unLevel    = oDigit.m_unLevel;
		m_bImportant = oDigit.m_bImportant;

		return *this;
	}

	CDigit &CDigit::operator+=(double dValue)
	{
		m_oValue += dValue;
		return *this;
	}

	CDigit &CDigit::operator*=(double dValue)
	{
		m_oValue *= dValue;
		return *this;
	}

	bool CDigit::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || (m_bImportant && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return false;

		double dValue = 0;

		std::wistringstream(wsValue) >> dValue;

		m_oValue     = dValue;
		m_unLevel    = unLevel;
		m_bImportant = bImportant;

		return true;
	}

	bool TRGB::Empty() const
	{
		return 0 == uchRed && 0 == uchGreen && 0 == uchBlue;
	}

	bool TRGB::operator==(const TRGB &oRGB) const
	{
		return uchRed   == oRGB.uchRed   &&
		       uchGreen == oRGB.uchGreen &&
		       uchBlue  == oRGB.uchBlue;
	}

	TRGB CColor::ConvertHEXtoRGB(const std::wstring &wsValue)
	{
		TRGB oRGB;

		if (wsValue.length() == 6)
			swscanf_s(wsValue.c_str(), L"%2hhx%2hhx%2hhx", &oRGB.uchRed, &oRGB.uchGreen, &oRGB.uchBlue);
		else if (wsValue.length() == 3)
		{
			swscanf_s(wsValue.c_str(), L"%1hhx%1hhx%1hhx", &oRGB.uchRed, &oRGB.uchGreen, &oRGB.uchBlue);
			oRGB.uchRed   *= 17;
			oRGB.uchGreen *= 17;
			oRGB.uchBlue  *= 17;
		}

		return oRGB;
	}

	std::wstring CColor::ConvertRGBtoHEX(const TRGB &oValue)
	{
		wchar_t arTemp[6];

		swprintf(arTemp, sizeof(arTemp), L"%2hhX%2hhX%2hhX", oValue.uchRed, oValue.uchGreen, oValue.uchBlue);

		return std::wstring(arTemp);
	}

	std::wstring CColor::CutURL(const std::wstring &wsValue)
	{
		size_t unStartURL = wsValue.find(L"url(");
		size_t unEndURL   = wsValue.find(L')', unStartURL);

		if (std::wstring::npos != unStartURL && std::wstring::npos != unEndURL && (6 < unEndURL - unStartURL))
			return wsValue.substr(unStartURL + 5, unEndURL - unStartURL - 6);

		return std::wstring();
	}

	CColor::CColor()
	    : CValue({}, 0, false)
	{}

	bool CColor::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (2 > wsValue.length() || (m_bImportant && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return false;

		if (wsValue[0] == L'#')
		{
			m_oValue.SetHEX(wsValue.substr(1, wsValue.length() - 1));
			return true;
		}
		else if (10 <= wsValue.length() && wsValue.substr(0, 3) == L"rgb")
		{
			size_t unEnd = wsValue.find(L')', 4);

			if (std::wstring::npos == unEnd)
				return false;

			std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsNewValue.substr(4, unEnd - 3));

			if (3 > arValues.size())
				return false;

			m_oValue.SetRGB(std::stoi(arValues[0]), std::stoi(arValues[0]), std::stoi(arValues[2]));
			return true;
		}
		else if (5 <= wsValue.length() && wsValue.substr(0, 3) == L"url")
		{
			m_oValue.SetUrl(CutURL(wsValue));
			return true;
		}
		else if (L"none" == wsValue)
		{
			m_oValue.SetNone();
			return true;
		}
		else
		{
			if (wsNewValue == L"transparent")
				return true;

			const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSConstValues::NSMaps::mColors.find(wsNewValue);
			if (oHEX != NSConstValues::NSMaps::mColors.end())
			{
				m_oValue.SetHEX(oHEX->second);
				return true;
			}
		}

		return false;
	}

	bool CColor::Empty() const
	{
		return m_oValue.Empty();
	}

	void CColor::Clear()
	{
		m_oValue.Clear();
	}

	ColorType CColor::GetType() const
	{
		return m_oValue.m_enType;
	}

	int CColor::ToInt() const
	{
		switch(m_oValue.m_enType)
		{
			case ColorRGB:
			{
				TRGB* pRGB = static_cast<TRGB*>(m_oValue.m_pColor);
				return RGB_TO_INT(pRGB->uchRed, pRGB->uchGreen, pRGB->uchBlue);
			}
			case ColorHEX:
			{
				std::wstring *pValue = static_cast<std::wstring*>(m_oValue.m_pColor);
				TRGB oRGB = ConvertHEXtoRGB(*pValue);
				return RGB_TO_INT(oRGB.uchRed, oRGB.uchGreen, oRGB.uchBlue);
			}
		}

		return 0;
	}

	double CColor::ToDouble() const
	{
		return ToInt();
	}

	std::wstring CColor::ToWString() const
	{
		switch(m_oValue.m_enType)
		{
			case ColorRGB: return ConvertRGBtoHEX(*static_cast<TRGB*>(m_oValue.m_pColor));
			case ColorHEX: case ColorUrl: return *static_cast<std::wstring*>(m_oValue.m_pColor);
		}

		return std::wstring();
	}

	TRGB CColor::ToRGB() const
	{
		switch(m_oValue.m_enType)
		{
			case ColorRGB: return *static_cast<TRGB*>(m_oValue.m_pColor);
			case ColorHEX: return ConvertHEXtoRGB(*static_cast<std::wstring*>(m_oValue.m_pColor));
		}

		return TRGB();
	}

	CMatrix::CMatrix()
	    : CValue(Aggplus::CMatrix(1., 0., 0., 1., 0., 0.), 0, false), m_enType(TransformNone)
	{}

	CMatrix::CMatrix(const Aggplus::CMatrix &oValue, unsigned int unLevel, bool bImportant)
	    : CValue(oValue, unLevel, bImportant), m_enType(TransformNone)
	{}

	bool CMatrix::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (std::wstring::npos != wsValue.find(L"matrix"))
			m_enType = TransformMatrix;
		else if (std::wstring::npos != wsValue.find(L"translate"))
			m_enType = TransformTranslate;
		else if (std::wstring::npos != wsValue.find(L"scale"))
			m_enType = TransformScale;
		else if (std::wstring::npos != wsValue.find(L"rotate"))
			m_enType = TransformRotate;
		else
			return false;

		std::vector<double> arValues = NS_STATIC_FUNCTIONS::ReadDoubleValues(wsValue);

		switch (m_enType)
		{
			case TransformMatrix:
			{
				if (6 != arValues.size())
					return false;

				m_oValue.SetElements(arValues[0], arValues[1], arValues[2], arValues[3], arValues[4], arValues[5]);
				break;
			}
			case TransformTranslate:
			{
				if (2 != arValues.size())
					return false;

				m_oValue.Shear(arValues[0], arValues[1]);
				break;
			}
			case TransformScale:
			{
				if (2 != arValues.size())
					return false;

				m_oValue.Scale(arValues[0], arValues[1]);
				break;
			}
			case TransformRotate:
			{
				if (3 != arValues.size())
					return false;

				m_oValue.RotateAt(arValues[0], arValues[1], arValues[2]);
				break;
			}
		}

		return true;
	}

	bool CMatrix::Empty() const
	{
		return m_oValue.IsIdentity();
	}

	void CMatrix::Clear()
	{
		m_oValue.SetElements(1., 0., 0., 1., 0., 0.);
		m_enType = TransformNone;
	}

	int CMatrix::ToInt() const
	{
		return 0.;
	}

	double CMatrix::ToDouble() const
	{
		return 0.;
	}

	std::wstring CMatrix::ToWString() const
	{
		if (TransformNone == m_enType)
			return std::wstring();

		std::wstring wsValue;

		switch (m_enType)
		{
			case TransformMatrix:
			{
				wsValue = L"matrix(";
				wsValue += std::to_wstring(m_oValue.sx())  + L',';
				wsValue += std::to_wstring(m_oValue.shx()) + L',';
				wsValue += std::to_wstring(m_oValue.shy()) + L',';
				wsValue += std::to_wstring(m_oValue.sy())  + L',';
				wsValue += std::to_wstring(m_oValue.tx())  + L',';
				wsValue += std::to_wstring(m_oValue.ty())  + L')';
				break;
			}
			case TransformTranslate:
			{
				wsValue = L"translate(";
				wsValue += std::to_wstring(m_oValue.tx())  + L',';
				wsValue += std::to_wstring(m_oValue.ty())  + L')';
				break;
			}
			case TransformScale:
			{
				wsValue = L"scale(";
				wsValue += std::to_wstring(m_oValue.sx())  + L',';
				wsValue += std::to_wstring(m_oValue.sy())  + L')';
				break;
			}
			case TransformRotate:
			{
				//TODO:: добавить реализацию
				wsValue = L"rotate(";
				break;
			}
		}

		return wsValue;
	}

	Aggplus::CMatrix CMatrix::GetValue() const
	{
		return m_oValue;
	}

	TransformType CMatrix::GetType() const
	{
		return m_enType;
	}

	bool CMatrix::operator==(const CMatrix &oMatrix) const
	{
		return Aggplus::CMatrix::IsEqual(&m_oValue, &oMatrix.m_oValue);
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

	bool CDisplay::SetX(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oX.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CDisplay::SetY(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oY.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CDisplay::AddX(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		double dX = m_oX.ToDouble();

		if (m_oX.SetValue(wsValue, unLevel, bHardMode))
		{
			m_oX += dX;
			return true;
		}

		return false;
	}

	bool CDisplay::AddY(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		double dY = m_oY.ToDouble();

		if (m_oY.SetValue(wsValue, unLevel, bHardMode))
		{
			m_oY += dY;
			return true;
		}

		return false;
	}

	bool CDisplay::SetWidth(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oWidth.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CDisplay::SetHeight(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oHeight.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CDisplay::SetHAlign(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oHAlign.SetValue(wsValue, {L"center", L"left", L"start", L"right", L"end"}, unLevel, bHardMode);
	}

	bool CDisplay::SetVAlign(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oVAlign.SetValue(wsValue, {std::make_pair(L"top", L"top"), std::make_pair(L"baseline", L"top"), std::make_pair(L"text-top", L"top"), std::make_pair(L"bottom", L"bottom"), std::make_pair(L"text-bottom", L"bottom"), std::make_pair(L"middle", L"middle")}, unLevel, bHardMode);
	}

	bool CDisplay::SetDisplay(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oHAlign.SetValue(wsValue, NSConstValues::arDisplayValues, unLevel, bHardMode);
	}

	const CDigit& CDisplay::GetX() const
	{
		return m_oX;
	}

	const CDigit& CDisplay::GetY() const
	{
		return m_oY;
	}

	const CDigit& CDisplay::GetWidth() const
	{
		return m_oWidth;
	}

	const CDigit& CDisplay::GetHeight() const
	{
		return m_oHeight;
	}

	const CString& CDisplay::GetHAlign() const
	{
		return m_oHAlign;
	}

	const CString& CDisplay::GetVAlign() const
	{
		return m_oVAlign;
	}

	const CString& CDisplay::GetDisplay() const
	{
		return m_oDisplay;
	}

	bool CDisplay::Empty() const
	{
		return m_oX.Empty() && m_oY.Empty() && m_oWidth.Empty() && m_oHeight.Empty() &&
		       m_oHeight.Empty() && m_oVAlign.Empty() && m_oDisplay.Empty();
	}

	CDisplay &CDisplay::operator+=(const CDisplay &oDisplay)
	{
		m_oX       += oDisplay.m_oX;
		m_oY       += oDisplay.m_oY;
		m_oWidth   += oDisplay.m_oWidth;
		m_oHeight  += oDisplay.m_oHeight;
		m_oHAlign  += oDisplay.m_oHAlign;
		m_oVAlign  += oDisplay.m_oVAlign;
		m_oDisplay += oDisplay.m_oDisplay;

		return *this;
	}

	bool CDisplay::operator==(const CDisplay &oDisplay) const
	{
		return m_oX       == oDisplay.m_oX      &&
		       m_oY       == oDisplay.m_oY      &&
		       m_oWidth   == oDisplay.m_oWidth  &&
		       m_oHeight  == oDisplay.m_oHeight &&
		       m_oHAlign  == oDisplay.m_oHAlign &&
		       m_oVAlign  == oDisplay.m_oVAlign &&
		       m_oDisplay == oDisplay.m_oDisplay;
	}

	// STROKE
	CStroke::CStroke()
	{}

	void CStroke::Equation(CStroke &oFirstStroke, CStroke &oSecondStroke)
	{
		CColor::Equation(oFirstStroke.m_oColor, oSecondStroke.m_oColor);
		CDigit::Equation(oFirstStroke.m_oWidth, oSecondStroke.m_oWidth);
	}

	bool CStroke::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oColor.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CStroke::SetWidth(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oWidth.SetValue(wsValue, unLevel, bHardMode);
	}

	const CColor& CStroke::GetColor() const
	{
		return m_oColor;
	}

	const CDigit& CStroke::GetWidth() const
	{
		return m_oWidth;
	}

	bool CStroke::Empty() const
	{
		return m_oColor.Empty() && m_oWidth.Empty();
	}

	CStroke &CStroke::operator+=(const CStroke &oStroke)
	{
		m_oColor += oStroke.m_oColor;
		m_oWidth += oStroke.m_oWidth;

		return *this;
	}

	bool CStroke::operator==(const CStroke &oStroke) const
	{
		return m_oColor == oStroke.m_oColor &&
		       m_oWidth == oStroke.m_oWidth;
	}

	// BACKGROUND
	CBackground::CBackground() : m_bInBorder(false)
	{}

	void CBackground::Equation(CBackground &oFirstBackground, CBackground &oSecondBackground)
	{
		CColor::Equation(oFirstBackground.m_oColor, oSecondBackground.m_oColor);
	}

	bool CBackground::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oColor.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CBackground::SetBackground(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		const std::vector<std::wstring> &arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsValue, L" ");

		for (const std::wstring& wsValue : arValues)
		{
			if (SetColor(wsValue, unLevel, bHardMode))
				return true;
		}

		return false;
	}

	void CBackground::InBorder()
	{
		m_bInBorder = true;
	}

	const CColor& CBackground::GetColor() const
	{
		return m_oColor;
	}

	bool CBackground::IsInBorder() const
	{
		return m_bInBorder;
	}

	bool CBackground::Empty() const
	{
		return m_oColor.Empty();
	}

	CBackground &CBackground::operator=(const CBackground &oBackground)
	{
		m_oColor    = oBackground.m_oColor;
		m_bInBorder = oBackground.m_bInBorder;

		return *this;
	}

	CBackground &CBackground::operator+=(const CBackground &oBackground)
	{
		m_oColor += oBackground.m_oColor;

		if (oBackground.m_bInBorder)
			m_bInBorder = true;

		return *this;
	}

	bool CBackground::operator==(const CBackground &oBackground) const
	{
		return m_oColor    == oBackground.m_oColor &&
		       m_bInBorder == oBackground.m_bInBorder;
	}

	// TRANSFORM
	CTransform::CTransform()
	{}

	void CTransform::Equation(CTransform &oFirstTransform, CTransform &oSecondTransform)
	{
		CMatrix::Equation(oFirstTransform.m_oMatrix, oSecondTransform.m_oMatrix);
	}

	bool CTransform::SetMatrix(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oMatrix.SetValue(wsValue, unLevel, bHardMode);
	}

	const CMatrix& CTransform::GetMatrix() const
	{
		return m_oMatrix;
	}

	bool CTransform::Empty() const
	{
		return m_oMatrix.Empty();
	}

	CTransform &CTransform::operator+=(const CTransform &oTransform)
	{
		m_oMatrix += oTransform.m_oMatrix;

		return *this;
	}

	bool CTransform::operator==(const CTransform &oTransform) const
	{
		return m_oMatrix == oTransform.m_oMatrix;
	}

	// BORDER SIDE
	CBorderSide::CBorderSide()
	    : m_bBlock(false)
	{}

	void CBorderSide::Equation(CBorderSide &oFirstBorderSide, CBorderSide &oSecondBorderSide)
	{
		CDigit::Equation (oFirstBorderSide.m_oWidth, oSecondBorderSide.m_oWidth);
		CString::Equation(oFirstBorderSide.m_oStyle, oSecondBorderSide.m_oStyle);
		CColor::Equation (oFirstBorderSide.m_oColor, oSecondBorderSide.m_oColor);
	}

	bool CBorderSide::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty())
			return false;

		const std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsValue, L" ");
		for (const std::wstring& sValue : arValues)
		{
			if (SetColor(sValue, unLevel, bHardMode))
				continue;
			else if(SetStyle(sValue, unLevel, bHardMode))
				continue;
			else if (SetWidth(sValue,unLevel,bHardMode))
				continue;
			else
				return false;
		}

		return true;
	}

	bool CBorderSide::SetWidth(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oWidth.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CBorderSide::SetStyle(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oStyle.SetValue(wsValue, {std::make_pair(L"dotted", L"dotted"), std::make_pair(L"dashed", L"dashed"), std::make_pair(L"solid", L"single"),
		                                   std::make_pair(L"double", L"double"), std::make_pair(L"groove", L"threeDEmboss"), std::make_pair(L"ridge", L"threeDEngrave"),
		                                   std::make_pair(L"inset", L"thinThickMediumGap"), std::make_pair(L"outset", L"thickThinMediumGap")}, unLevel, bHardMode);
	}

	bool CBorderSide::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oColor.SetValue(wsValue, unLevel, bHardMode);
	}

	void CBorderSide::Block()
	{
		m_bBlock = true;
	}

	void CBorderSide::Unblock()
	{
		m_bBlock = false;
	}

	bool CBorderSide::IsBlock() const
	{
		return m_bBlock;
	}

	const CDigit& CBorderSide::GetWidth() const
	{
		return m_oWidth;
	}

	const CString& CBorderSide::GetStyle() const
	{
//		if (m_bBlock)
//			return std::wstring();
//		else if (m_oWidth == 0.)
//			return L"none";
//		else if (m_oStyle == L"auto" || m_oStyle.Empty())
//			return L"single";
//		else
//			return m_oStyle.ToWString();
		return m_oStyle;
	}

	const CColor& CBorderSide::GetColor() const
	{
		return m_oColor;
	}

	bool CBorderSide::Empty() const
	{
		return m_oWidth.Empty() && m_oStyle.Empty() && m_oColor.Empty();
	}

	CBorderSide &CBorderSide::operator+=(const CBorderSide &oBorderSide)
	{
		m_oWidth += oBorderSide.m_oWidth;
		m_oStyle += oBorderSide.m_oStyle;
		m_oColor += oBorderSide.m_oColor;

		if (oBorderSide.m_bBlock)
			m_bBlock = true;

		return *this;
	}

	bool CBorderSide::operator==(const CBorderSide &oBorderSide) const
	{
		return m_oWidth == oBorderSide.m_oWidth &&
		       m_oStyle == oBorderSide.m_oStyle &&
		       m_oColor == oBorderSide.m_oColor;
	}

	// BORDER
	CBorder::CBorder()
	{}

	void CBorder::Equation(CBorder &oFirstBorder, CBorder &oSecondBorder)
	{
		CBorderSide::Equation(oFirstBorder.m_oLeft,   oSecondBorder.m_oLeft);
		CBorderSide::Equation(oFirstBorder.m_oTop,    oSecondBorder.m_oTop);
		CBorderSide::Equation(oFirstBorder.m_oRight,  oSecondBorder.m_oRight);
		CBorderSide::Equation(oFirstBorder.m_oBottom, oSecondBorder.m_oBottom);
	}

	bool CBorder::SetSides(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft  .SetValue(wsValue, unLevel, bHardMode) ||
		       m_oTop   .SetValue(wsValue, unLevel, bHardMode) ||
		       m_oRight .SetValue(wsValue, unLevel, bHardMode) ||
		       m_oBottom.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetWidth(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft  .SetWidth(wsValue, unLevel, bHardMode) ||
		       m_oTop   .SetWidth(wsValue, unLevel, bHardMode) ||
		       m_oRight .SetWidth(wsValue, unLevel, bHardMode) ||
		       m_oBottom.SetWidth(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetStyle(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft  .SetStyle(wsValue, unLevel, bHardMode) ||
		       m_oTop   .SetStyle(wsValue, unLevel, bHardMode) ||
		       m_oRight .SetStyle(wsValue, unLevel, bHardMode) ||
		       m_oBottom.SetStyle(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft  .SetColor(wsValue, unLevel, bHardMode) ||
		       m_oTop   .SetColor(wsValue, unLevel, bHardMode) ||
		       m_oRight .SetColor(wsValue, unLevel, bHardMode) ||
		       m_oBottom.SetColor(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetLeftSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetWidthLeftSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft.SetWidth(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetStyleLeftSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft.SetStyle(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetColorLeftSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft.SetColor(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetTopSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oTop.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetWidthTopSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oTop.SetWidth(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetStyleTopSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oTop.SetStyle(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetColorTopSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oTop.SetColor(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetRightSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oRight.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetWidthRightSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oRight.SetWidth(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetStyleRightSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oRight.SetStyle(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetColorRightSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oRight.SetColor(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetBottomSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oBottom.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetWidthBottomSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oBottom.SetWidth(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetStyleBottomSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oBottom.SetStyle(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetColorBottomSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oBottom.SetColor(wsValue, unLevel, bHardMode);
	}

	void CBorder::Block()
	{
		m_oLeft  .Block();
		m_oTop   .Block();
		m_oRight .Block();
		m_oBottom.Block();
	}

	void CBorder::Unblock()
	{
		m_oLeft  .Unblock();
		m_oTop   .Unblock();
		m_oRight .Unblock();
		m_oBottom.Unblock();
	}

	bool CBorder::Empty() const
	{
		return m_oLeft.Empty()  && m_oTop.Empty() &&
		       m_oRight.Empty() && m_oBottom.Empty();
	}

	bool CBorder::EqualSides() const
	{
		return m_oLeft == m_oTop && m_oTop == m_oRight && m_oRight == m_oBottom;
	}

	const CBorderSide& CBorder::GetLeftBorder() const
	{
		return m_oLeft;
	}

	const CBorderSide& CBorder::GetTopBorder() const
	{
		return m_oTop;
	}

	const CBorderSide& CBorder::GetRightBorder() const
	{
		return m_oRight;
	}

	const CBorderSide& CBorder::GetBottomBorder() const
	{
		return m_oBottom;
	}

	CBorder &CBorder::operator+=(const CBorder &oBorder)
	{
		m_oLeft   += oBorder.m_oLeft;
		m_oTop    += oBorder.m_oTop;
		m_oRight  += oBorder.m_oRight;
		m_oBottom += oBorder.m_oBottom;

		return *this;
	}

	bool CBorder::operator==(const CBorder &oBorder) const
	{
		return m_oLeft   == oBorder.m_oLeft  &&
		       m_oTop    == oBorder.m_oTop   &&
		       m_oRight  == oBorder.m_oRight &&
		       m_oBottom == oBorder.m_oBottom;
	}

	// TEXT
	CText::CText()
	{}

	void CText::Equation(CText &oFirstText, CText &oSecondText)
	{
		CDigit ::Equation(oFirstText.m_oIndent,     oSecondText.m_oIndent);
		CString::Equation(oFirstText.m_oAlign,      oSecondText.m_oAlign);
		CString::Equation(oFirstText.m_oDecoration, oSecondText.m_oDecoration);
		CColor ::Equation(oFirstText.m_oColor,      oSecondText.m_oColor);
	}

	bool CText::SetIndent(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (m_oIndent.SetValue(wsValue, unLevel, bHardMode))
		{
			m_oIndent *= 10.;
			return true;
		}

		return false;
	}

	bool CText::SetAlign(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oAlign.SetValue(wsValue, {std::make_pair(L"center", L"center"), std::make_pair(L"justify", L"both"), std::make_pair(L"left",  L"left"), std::make_pair(L"start", L"left"), std::make_pair(L"right", L"right"), std::make_pair(L"end", L"right")}, unLevel, bHardMode);
	}

	bool CText::SetDecoration(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oDecoration.SetValue(wsValue, {std::make_pair(L"underline", L"single"), std::make_pair(L"line-through", L"line-through"), std::make_pair(L"none", L"none")}, unLevel, bHardMode);
	}

	bool CText::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oColor.SetValue(wsValue, unLevel, bHardMode);
	}

	const CDigit& CText::GetIndent() const
	{
		return m_oIndent;
	}

	const CString& CText::GetAlign() const
	{
		return m_oAlign;
	}

	const CString& CText::GetDecoration() const
	{
		return m_oDecoration;
	}

	const CColor& CText::GetColor() const
	{
		return m_oColor;
	}

	bool CText::Empty() const
	{
		return m_oIndent.Empty()     && m_oAlign.Empty() &&
		       m_oDecoration.Empty() && m_oColor.Empty();
	}

	CText &CText::operator+=(const CText &oText)
	{
		m_oIndent     += oText.m_oIndent;
		m_oAlign      += oText.m_oAlign;
		m_oDecoration += oText.m_oDecoration;
		m_oColor      += oText.m_oColor;

		return *this;
	}

	bool CText::operator==(const CText &oText) const
	{
		return m_oIndent     == oText.m_oIndent     &&
		       m_oAlign      == oText.m_oAlign      &&
		       m_oDecoration == oText.m_oDecoration &&
		       m_oColor      == oText.m_oColor;
	}

	// MARGIN
	CIndent::CIndent()
	    : m_bPermission(true)
	 {}

	void CIndent::Equation(CIndent &oFirstMargin, CIndent &oSecondMargin)
	{
		CDigit::Equation(oFirstMargin.m_oLeft,   oSecondMargin.m_oLeft);
		CDigit::Equation(oFirstMargin.m_oTop,    oSecondMargin.m_oTop);
		CDigit::Equation(oFirstMargin.m_oRight,  oSecondMargin.m_oRight);
		CDigit::Equation(oFirstMargin.m_oBottom, oSecondMargin.m_oBottom);
	}

	void CIndent::SetPermisson(bool bPermission)
	{
		m_bPermission = bPermission;
	}

	bool CIndent::AddValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		const std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsValue, L" ");
		switch (arValues.size())
		{
			case 1:
			{
				// TODO:: иногда будет не верный рельтат, если до этого одиен из элементом был с '!important'
				// Необходимо добавить обработку такого случая
				if (AddValue(m_oLeft, arValues[0], unLevel, bHardMode))
				{
					m_oTop = m_oRight = m_oBottom = m_oLeft;
					return true;
				}
				break;
			}
			case 2:
			{
				if (AddValue(m_oTop,  arValues[0], unLevel, bHardMode) &&
				    AddValue(m_oLeft, arValues[1], unLevel, bHardMode))
				{
					m_oBottom = m_oTop;
					m_oRight  = m_oLeft;

					return true;
				}
				break;
			}
			case 3:
			{
				if (AddValue(m_oTop,    arValues[0], unLevel, bHardMode) &&
				    AddValue(m_oLeft,   arValues[1], unLevel, bHardMode) &&
				    AddValue(m_oBottom, arValues[2], unLevel, bHardMode))
				{
					m_oRight = m_oLeft;

					return true;
				}

				break;
			}
			case 4:
			{
				if (AddValue(m_oTop,    arValues[0], unLevel, bHardMode) &&
				    AddValue(m_oRight,  arValues[1], unLevel, bHardMode) &&
				    AddValue(m_oBottom, arValues[2], unLevel, bHardMode) &&
				    AddValue(m_oLeft,   arValues[3], unLevel, bHardMode))
					return true;

				break;
			}
		}

		return false;
	}

	bool CIndent::AddLeft(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return AddValue(m_oLeft, wsValue, unLevel, bHardMode);
	}

	bool CIndent::AddTop(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return AddValue(m_oTop, wsValue, unLevel, bHardMode);
	}

	bool CIndent::AddRight(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return AddValue(m_oRight, wsValue, unLevel, bHardMode);
	}

	bool CIndent::AddBottom(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return AddValue(m_oBottom, wsValue, unLevel, bHardMode);
	}

	const CDigit& CIndent::GetLeft() const
	{
		return m_oLeft;
	}

	const CDigit& CIndent::GetTop() const
	{
		return m_oTop;
	}

	const CDigit& CIndent::GetRight() const
	{
		return m_oRight;
	}

	const CDigit& CIndent::GetBottom() const
	{
		return m_oBottom;
	}

	bool CIndent::Empty() const
	{
		return m_oLeft.Empty() && m_oTop.Empty() && m_oRight.Empty() && m_oBottom.Empty();
	}

	CIndent &CIndent::operator+=(const CIndent &oMargin)
	{
		m_oLeft   += oMargin.m_oLeft;
		m_oTop    += oMargin.m_oTop;
		m_oRight  += oMargin.m_oRight;
		m_oBottom += oMargin.m_oBottom;

		return *this;
	}

	bool CIndent::operator==(const CIndent &oMargin) const
	{
		return m_oLeft   == oMargin.m_oLeft  &&
		       m_oTop    == oMargin.m_oTop   &&
		       m_oRight  == oMargin.m_oRight &&
		       m_oBottom == oMargin.m_oBottom;
	}

	bool CIndent::AddValue(CDigit &oValue, const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (!m_bPermission)
			return false;

		CDigit oTempValue;

		if (!oTempValue.SetValue(wsValue, unLevel, bHardMode))
			return false;

		oValue += oTempValue;

		return true;
	}

	// FONT
	CFont::CFont()
	    : m_oSize(24., 0)
	{}

	void CFont::Equation(CFont &oFirstFont, CFont &oSecondFont)
	{
		CDigit ::Equation(oFirstFont.m_oSize,       oSecondFont.m_oSize);
		CDigit ::Equation(oFirstFont.m_oLineHeight, oSecondFont.m_oLineHeight);
		CString::Equation(oFirstFont.m_oFamily,     oSecondFont.m_oFamily);
		CString::Equation(oFirstFont.m_oStretch,    oSecondFont.m_oStretch);
		CString::Equation(oFirstFont.m_oStyle,      oSecondFont.m_oStyle);
		CString::Equation(oFirstFont.m_oVariant,    oSecondFont.m_oVariant);
		CString::Equation(oFirstFont.m_oWeight,     oSecondFont.m_oWeight);
	}

	bool CFont::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		//TODO:: скорее всего стоит переделать парсинг строки со шрифтом
		const std::vector<std::wstring> arValues = NSCSS::NS_STATIC_FUNCTIONS::GetWordsW(wsValue, L" ,/");

		unsigned short ushPosition = 0;

		for (const std::wstring &wsWord : arValues)
		{
			if (ushPosition < 4)
			{
				if (SetStyle(wsWord, unLevel, bHardMode) ||
				    L"normal" == wsWord || L"italic" == wsWord || L"oblique" == wsWord || L"inherit" == wsWord)
				{
					++ushPosition;
					continue;
				}
			}

			if (ushPosition < 4 && m_oVariant.Empty() && (SetVariant(wsWord, unLevel, bHardMode) ||
			                                              L"normal" == wsWord || L"small-caps" == wsWord || L"inherit" == wsWord))
			{
				++ushPosition;
				continue;
			}

			if (ushPosition < 4 && m_oWeight.Empty() && SetWeight(wsWord, unLevel, bHardMode))
			{
				++ushPosition;
				continue;
			}

			if (ushPosition < 4 && m_oStretch.Empty() && SetStretch(wsWord, unLevel, bHardMode))
			{
				++ushPosition;
				continue;
			}

			if (SetSize(wsValue, unLevel, bHardMode))
			{
				ushPosition = 5;
				continue;
			}

			if (ushPosition == 5 && SetLineHeight(wsValue, unLevel, bHardMode))
			{
				++ushPosition;
				continue;
			}

			if (ushPosition >= 5 && SetFamily(wsValue, unLevel, bHardMode))
				continue;
		}

		if (ushPosition > 5)
			return true;
		else
			Clear();

		return false;
	}

	bool CFont::SetSize(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oSize.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CFont::SetLineHeight(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (m_oLineHeight.SetValue(wsValue, unLevel, bHardMode))
		{
			m_oLineHeight *= 10.;
			return true;
		}

		return false;
	}

	bool CFont::SetFamily(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		std::wstring wsNewFamily(wsValue);

		if (wsNewFamily.end() == wsNewFamily.erase(std::remove(wsNewFamily.begin(), wsNewFamily.end(), L'\''), wsNewFamily.end()) &&
		    wsNewFamily.end() == wsNewFamily.erase(std::remove(wsNewFamily.begin(), wsNewFamily.end(), L'"'),  wsNewFamily.end()))
			return false;

		std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(wsNewFamily, L",");

		for (std::vector<std::wstring>::iterator iWord = arWords.begin(); iWord != arWords.end(); ++iWord)
		{
			if ((*iWord).empty())
				continue;

			return m_oFamily.SetValue(*iWord, unLevel, bHardMode);
		}

		return false;
	}

	bool CFont::SetStretch(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oStretch.SetValue(wsValue, {L"ultra-condensed", L"extra-condensed", L"condensed", L"semi-condensed", L"normal",
		                                     L"semi-expanded", L"expanded", L"extra-expanded", L"ultra-expanded"}, unLevel, bHardMode);
	}

	bool CFont::SetStyle(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oStyle.SetValue(wsValue, {L"normal", L"italic", L"oblique"}, unLevel, bHardMode);
	}

	bool CFont::SetVariant(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oVariant.SetValue(wsValue, std::vector<std::wstring>{L"normal", L"small-caps"}, unLevel, bHardMode);
	}

	bool CFont::SetWeight(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oWeight.SetValue(wsValue, {std::make_pair(L"normal", L"normal"), std::make_pair(L"300", L"normal"), std::make_pair(L"400", L"normal"), std::make_pair(L"500", L"normal"),
		                                    std::make_pair(L"bold", L"bold"), std::make_pair(L"bolder", L"bold"), std::make_pair(L"600", L"bold"),
		                                    std::make_pair(L"700", L"bold"), std::make_pair(L"800", L"bold"), std::make_pair(L"900", L"bold")}, unLevel, bHardMode);
	}

	void CFont::Clear()
	{
		m_oSize = CDigit(24., 0);

		m_oLineHeight.Clear();
		m_oFamily    .Clear();
		m_oStretch   .Clear();
		m_oStyle     .Clear();
		m_oVariant   .Clear();
		m_oWeight    .Clear();
	}

	const CDigit &CFont::GetSize() const
	{
		return m_oSize;
	}

	const CDigit &CFont::GetLineHeight() const
	{
		return m_oLineHeight;
	}

	const CString &CFont::GetFamily() const
	{
		return m_oFamily;
	}

	const CString &CFont::GetStretch() const
	{
		return m_oStretch;
	}

	const CString &CFont::GetStyle() const
	{
		return m_oStyle;
	}

	const CString &CFont::GetVariant() const
	{
		return m_oVariant;
	}

	const CString &CFont::GetWeight() const
	{
		return m_oWeight;
	}

	bool CFont::Empty() const
	{
		return m_oSize.Empty() && m_oLineHeight.Empty() && m_oFamily.Empty() &&
		       m_oStretch.Empty() && m_oStyle.Empty() && m_oVariant.Empty() && m_oWeight.Empty();
	}

	CFont &CFont::operator+=(const CFont &oFont)
	{
		m_oSize       += oFont.m_oSize;
		m_oLineHeight += oFont.m_oLineHeight;
		m_oFamily     += oFont.m_oFamily;
		m_oStretch    += oFont.m_oStretch;
		m_oStyle      += oFont.m_oStyle;
		m_oVariant    += oFont.m_oVariant;
		m_oWeight     += oFont.m_oWeight;

		return *this;
	}

	bool CFont::operator==(const CFont &oFont) const
	{
		return m_oSize       == oFont.m_oSize       &&
		       m_oLineHeight == oFont.m_oLineHeight &&
		       m_oFamily     == oFont.m_oFamily     &&
		       m_oStretch    == oFont.m_oStretch    &&
		       m_oStyle      == oFont.m_oStyle      &&
		       m_oVariant    == oFont.m_oVariant    &&
		        m_oWeight     == oFont.m_oWeight;
	}

	CColorValue::CColorValue()
	    : m_enType(ColorEmpty), m_pColor(NULL)
	{}

	CColorValue::CColorValue(const CColorValue &oColorValue)
	    : m_enType()
	{
		switch(oColorValue.m_enType)
		{
			case ColorRGB: SetRGB(*static_cast<TRGB*>(oColorValue.m_pColor)); break;
			case ColorHEX: SetHEX(*static_cast<std::wstring*>(oColorValue.m_pColor)); break;
			case ColorUrl: SetUrl(*static_cast<std::wstring*>(oColorValue.m_pColor)); break;
			default: m_enType = oColorValue.m_enType; break;
		}

	}

	CColorValue::~CColorValue()
	{
		Clear();
	}

	void CColorValue::SetRGB(unsigned char uchR, unsigned char uchG, unsigned char uchB)
	{
		Clear();

		m_pColor = new TRGB{uchR, uchG, uchB};

		if (NULL == m_pColor)
		{
			m_enType = ColorEmpty;
			return;
		}

		m_enType = ColorRGB;
	}

	void CColorValue::SetRGB(const TRGB &oRGB)
	{
		Clear();

		m_pColor = new TRGB{oRGB};

		if (NULL == m_pColor)
		{
			m_enType = ColorEmpty;
			return;
		}

		m_enType = ColorRGB;
	}

	void CColorValue::SetHEX(const std::wstring &wsValue)
	{
		Clear();

		m_pColor = new std::wstring(wsValue);

		if (NULL == m_pColor)
		{
			m_enType = ColorEmpty;
			return;
		}

		m_enType = ColorHEX;
	}

	void CColorValue::SetUrl(const std::wstring &wsValue)
	{
		Clear();

		m_pColor = new std::wstring(wsValue);

		if (NULL == m_pColor)
		{
			m_enType = ColorEmpty;
			return;
		}

		m_enType = ColorUrl;
	}

	void CColorValue::SetNone()
	{
		Clear();

		m_enType = ColorNone;
	}

	bool CColorValue::Empty() const
	{
		return (ColorEmpty == m_enType) || (ColorRGB == m_enType && static_cast<TRGB*>(m_pColor)->Empty()) ||
		        ((ColorHEX == m_enType || ColorUrl == m_enType) && static_cast<std::wstring*>(m_pColor)->empty());
	}

	std::wstring CColorValue::GetColor() const
	{
		return *(std::wstring*)m_pColor;
	}

	bool CColorValue::operator==(const CColorValue &oColorValue) const
	{
		return (m_enType == oColorValue.m_enType) && ((ColorEmpty == m_enType) || (ColorNone == m_enType) ||
		                                              (ColorRGB == m_enType && *static_cast<std::wstring*>(m_pColor) == *static_cast<std::wstring*>(oColorValue.m_pColor)) || ((ColorHEX == m_enType || ColorUrl == m_enType) && *static_cast<std::wstring*>(m_pColor) == *static_cast<std::wstring*>(oColorValue.m_pColor)));
	}

	CColorValue &CColorValue::operator=(const CColorValue &oColorValue)
	{
		switch(oColorValue.m_enType)
		{
			case ColorRGB: SetRGB(*static_cast<TRGB*>(oColorValue.m_pColor)); break;
			case ColorHEX: SetHEX(*static_cast<std::wstring*>(oColorValue.m_pColor)); break;
			case ColorUrl: SetUrl(*static_cast<std::wstring*>(oColorValue.m_pColor)); break;
			default: m_enType = oColorValue.m_enType; break;
		}

		return *this;
	}

	void CColorValue::Clear()
	{
		switch (m_enType)
		{
			case ColorRGB:
			{
				TRGB *pRGB = static_cast<TRGB*>(m_pColor);
				RELEASEOBJECT(pRGB);
				break;
			}
			case ColorHEX: case ColorUrl:
			{
				std::wstring* pValue = static_cast<std::wstring*>(m_pColor);
				RELEASEOBJECT(pValue);
				break;
			}
		}

		m_enType = ColorEmpty;
	}
	}
}
