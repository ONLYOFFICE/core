#include "StyleProperties.h"

#include <iostream>

#include "StaticFunctions.h"
#include "ConstValues.h"

namespace NSCSS
{
	namespace NSProperty
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
	void CValue<T>::Equation(CValue &oFirstValue, CValue &oSecondValue, const T* pDefaultValue)
	{
		if (oFirstValue.m_bImportant && !oSecondValue.m_bImportant && oFirstValue.Empty())
			oSecondValue.Clear(pDefaultValue);
		else if (oSecondValue.m_bImportant && !oFirstValue.m_bImportant && !oSecondValue.Empty())
			oFirstValue.Clear(pDefaultValue);
		else if (!oSecondValue.Empty())
		{
			if (oFirstValue.m_unLevel < oSecondValue.m_unLevel)
				oFirstValue.Clear(pDefaultValue);
			else
				oSecondValue.Clear(pDefaultValue);
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
	CValue<T> &CValue<T>::operator+=(const T &oValue)
	{
		m_oValue += oValue;

		return *this;
	}

	template<typename T>
	CValue<T> &CValue<T>::operator+=(const CValue<T> &oValue)
	{
		m_oValue    += oValue.m_oValue;
		m_unLevel    = std::max(m_unLevel, oValue.m_unLevel);
		m_bImportant = std::max(m_bImportant, oValue.m_bImportant);

		return *this;
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

	void CString::Clear(const  std::wstring* pDefaultValue)
	{
		if (NULL == pDefaultValue)
			m_oValue.clear();
		else
			m_oValue = std::move(*pDefaultValue);
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

	void CDigit::Clear(const  double* pDefaultValue)
	{
		if (NULL == pDefaultValue)
			m_oValue = 0.;
		else
			m_oValue = std::move(*pDefaultValue);
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

	bool CColor::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || (m_bImportant && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return false;

		if (wsValue[0] == L'#')
		{
			m_oValue = ConvertHEXtoRGB(wsValue.substr(1, wsValue.length() - 1));
			return true;
		}
		else if (wsValue.substr(0, 3) == L"rgb")
		{
			size_t unEnd = wsValue.find(L')', 4);

			if (std::wstring::npos == unEnd)
				return false;

			std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsNewValue.substr(4, unEnd - 3));

			if (3 > arValues.size())
				return false;

			m_oValue.chRed   = std::stoi(arValues[0]);
			m_oValue.chGreen = std::stoi(arValues[1]);
			m_oValue.chBlue  = std::stoi(arValues[2]);

			return true;
		}
		else
		{
			std::wstring sNewColor = wsValue;
			std::transform(sNewColor.begin(), sNewColor.end(), sNewColor.begin(), towlower);

			if (sNewColor == L"transparent")
				return true;

			const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSConstValues::NSMaps::mColors.find(sNewColor);
			if (oHEX != NSConstValues::NSMaps::mColors.end())
			{
				m_oValue = ConvertHEXtoRGB(oHEX->second);
				return true;
			}
		}

		return false;
	}

	bool CColor::Empty() const
	{
		return 0. == m_oValue.chRed && 0 == m_oValue.chGreen && 0 == m_oValue.chBlue;
	}

	void CColor::Clear(const  TRGB* pDefaultValue)
	{
		if (NULL == pDefaultValue)
			m_oValue.chRed = m_oValue.chGreen = m_oValue.chBlue = 0;
		else
			m_oValue = std::move(*pDefaultValue);
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

	CMatrix::CMatrix()
	    : CValue(Aggplus::CMatrix(0., 0., 0., 0., 0., 0.), 0, false), m_enType(TransformNone)
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

	void CMatrix::Clear(const  Aggplus::CMatrix* pDefaultValue)
	{
		if (NULL == pDefaultValue)
		{
			m_oValue.SetElements(1., 0., 0., 1., 0., 0.);
			m_enType = TransformNone;
		}
		else
		{
			m_oValue = std::move(*pDefaultValue);
			m_enType = TransformMatrix;
		}
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

	bool CStroke::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oColor.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CStroke::SetWidth(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oWidth.SetValue(wsValue, unLevel, bHardMode);
	}

	std::wstring CStroke::GetColor() const
	{
		return m_oColor.ToWString();
	}

	int CStroke::GetColorN() const
	{
		return m_oColor.ToInt();
	}

	double CStroke::GetWidth() const
	{
		return m_oWidth.ToDouble();
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

	void CBackground::InBorder()
	{
		m_bInBorder = true;
	}

	std::wstring CBackground::GetColor() const
	{
		return m_oColor.ToWString();
	}

	int CBackground::GetColorN() const
	{
		return m_oColor.ToInt();
	}

	std::wstring CBackground::GetColorURL() const
	{
		return m_oColor.GetURL();
	}

	bool CBackground::IsInBorder() const
	{
		return m_bInBorder;
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

	Aggplus::CMatrix CTransform::GetMatrix() const
	{
		return m_oMatrix.GetValue();
	}

	TransformType CTransform::GetType() const
	{
		return m_oMatrix.GetType();
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

	double CBorderSide::GetWidth() const
	{
		return (m_bBlock) ? 0. : m_oWidth.ToDouble();
	}

	std::wstring CBorderSide::GetStyle() const
	{
		if (m_bBlock)
			return std::wstring();
		else if (m_oWidth == 0.)
			return L"none";
		else if (m_oStyle == L"auto" || m_oStyle.Empty())
			return L"single";
		else
			return m_oStyle.ToWString();
	}

	std::wstring CBorderSide::GetColor() const
	{
		return (m_bBlock) ? std::wstring() : m_oColor.ToWString();
	}

	int CBorderSide::GetColorN() const
	{
		return (m_bBlock) ? 0 : m_oColor.ToInt();
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

	bool CBorder::SetAllSides(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		// TODO:: иногда будет не верный рельтат, если до этого одиен из элементом был с '!important'
		// Необходимо добавить обработку такого случая
		if (m_oLeft.SetValue(wsValue, unLevel, bHardMode))
		{
			m_oTop = m_oRight = m_oBottom = m_oLeft;
			return true;
		}

		return false;
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

	CBorderSide CBorder::GetLeftBorder() const
	{
		return m_oLeft;
	}

	CBorderSide CBorder::GetTopBorder() const
	{
		return m_oTop;
	}

	CBorderSide CBorder::GetRightBorder() const
	{
		return m_oRight;
	}

	CBorderSide CBorder::GetBottomBorder() const
	{
		return m_oBottom;
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

	double CText::GetIndent() const
	{
		return m_oIndent.ToDouble();
	}

	std::wstring CText::GetAlign() const
	{
		return m_oAlign.ToWString();
	}

	std::wstring CText::GetDecoration() const
	{
		return m_oDecoration.ToWString();
	}

	std::wstring CText::GetColor() const
	{
		return m_oColor.ToWString();
	}

	int CText::GetColorN() const
	{
		return m_oColor.ToInt();
	}

	// MARGIN
	CMargin::CMargin()
	    : m_bPermission(true)
	 {}

	void CMargin::Equation(CMargin &oFirstMargin, CMargin &oSecondMargin)
	{
		CDigit::Equation(oFirstMargin.m_oLeft,   oSecondMargin.m_oLeft);
		CDigit::Equation(oFirstMargin.m_oTop,    oSecondMargin.m_oTop);
		CDigit::Equation(oFirstMargin.m_oRight,  oSecondMargin.m_oRight);
		CDigit::Equation(oFirstMargin.m_oBottom, oSecondMargin.m_oBottom);
	}

	void CMargin::SetPermisson(bool bPermission)
	{
		m_bPermission = bPermission;
	}

	bool CMargin::AddMargin(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
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

	bool CMargin::AddLeft(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return AddValue(m_oLeft, wsValue, unLevel, bHardMode);
	}

	bool CMargin::AddTop(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return AddValue(m_oTop, wsValue, unLevel, bHardMode);
	}

	bool CMargin::AddRight(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return AddValue(m_oRight, wsValue, unLevel, bHardMode);
	}

	bool CMargin::AddBottom(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return AddValue(m_oBottom, wsValue, unLevel, bHardMode);
	}

	double CMargin::GetLeft() const
	{
		return m_oLeft.ToDouble();
	}

	double CMargin::GetTop() const
	{
		return m_oTop.ToDouble();
	}

	double CMargin::GetRight() const
	{
		return m_oRight.ToDouble();
	}

	double CMargin::GetBottom() const
	{
		return m_oBottom.ToDouble();
	}

	bool CMargin::AddValue(CDigit &oValue, const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (!m_bPermission)
			return false;

		CDigit oTempValue;

		if (!oTempValue.SetValue(wsValue, unLevel, bHardMode))
			return false;

		oValue += oTempValue;

		return true;
	}

	}
}
