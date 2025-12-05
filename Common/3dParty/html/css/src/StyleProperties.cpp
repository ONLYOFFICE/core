#include "StyleProperties.h"

#include "StaticFunctions.h"
#include "ConstValues.h"
#include <cfloat>
#include <cmath>
#include <functional>
#include <wchar.h>

namespace NSCSS
{
	namespace NSProperties
	{
	#define CHECK_CONDITIONS (m_bImportant || unLevel < m_unLevel)

	static bool CutImportant(std::wstring& wsValue)
	{
		size_t unBegin = wsValue.find(L"!important");

		if (std::wstring::npos == unBegin)
			return false;

		wsValue.erase(unBegin, 10);

		return true;
	}

	CString::CString()
	{}

	CString::CString(const std::wstring &wsValue, unsigned int unLevel, bool bImportant)
	    : CValueOptional(wsValue, unLevel, bImportant)
	{}

	bool CString::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || (CHECK_CONDITIONS && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		const bool bImportant{CutImportant(wsNewValue)};

		if (m_bImportant && !bImportant)
			return false;

		if (UINT_MAX == unLevel)
			m_unLevel++;
		else
			m_unLevel = unLevel;

		m_oValue     = wsNewValue;
		m_bImportant = bImportant;

		return true;
	}

	bool CString::SetValue(const std::wstring &wsValue, const std::vector<std::wstring> &arValiableValues, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || arValiableValues.empty() || (CHECK_CONDITIONS && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		const bool bImportant{CutImportant(wsNewValue)};

		if (m_bImportant && !bImportant)
			return false;

		if (arValiableValues.end() != std::find(arValiableValues.begin(), arValiableValues.end(), wsNewValue))
		{
			if (UINT_MAX == unLevel)
				m_unLevel++;
			else
				m_unLevel = unLevel;

			m_oValue     = wsNewValue;
			m_bImportant = bImportant;

			return true;
		}

		return false;
	}

	bool CString::SetValue(const std::wstring &wsValue, const std::map<std::wstring, std::wstring> &arValiableValues, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || arValiableValues.empty() || (CHECK_CONDITIONS && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		const bool bImportant{CutImportant(wsNewValue)};

		if (m_bImportant && !bImportant)
			return false;

		std::map<std::wstring, std::wstring>::const_iterator oFoundValue = arValiableValues.find(wsNewValue);

		if (arValiableValues.end() != oFoundValue)
		{
			if (UINT_MAX == unLevel)
				m_unLevel++;
			else
				m_unLevel = unLevel;

			m_oValue     = oFoundValue->second;
			m_bImportant = bImportant;

			return true;
		}

		return false;
	}

	bool CString::Empty() const
	{
		return CValueOptional::Empty() || m_oValue.value().empty();
	}

	int CString::ToInt() const
	{
		if (CValueOptional::Empty())
			return 0;

		return std::stoi(m_oValue.value());
	}

	double CString::ToDouble() const
	{
		if (CValueOptional::Empty())
			return 0.;

		return std::stod(m_oValue.value());
	}

	std::wstring CString::ToWString() const
	{
		if (CValueOptional::Empty())
			return std::wstring();

		return m_oValue.value();
	}

	double CDigit::ConvertValue(double dPrevValue, UnitMeasure enUnitMeasure) const
	{
		if (CValueOptional::Empty())
			return 0.;

		switch(m_enUnitMeasure)
		{
			case Percent:    return dPrevValue * m_oValue.value() / 100.;
			case Pixel:      return CUnitMeasureConverter::ConvertPx(m_oValue.value(), enUnitMeasure, 96);
			case Point:      return CUnitMeasureConverter::ConvertPt(m_oValue.value(), enUnitMeasure, 96);
			case Cantimeter: return CUnitMeasureConverter::ConvertCm(m_oValue.value(), enUnitMeasure, 96);
			case Millimeter: return CUnitMeasureConverter::ConvertMm(m_oValue.value(), enUnitMeasure, 96);
			case Inch:       return CUnitMeasureConverter::ConvertIn(m_oValue.value(), enUnitMeasure, 96);
			case Peak:       return CUnitMeasureConverter::ConvertPc(m_oValue.value(), enUnitMeasure, 96);
			case Twips:      return CUnitMeasureConverter::ConvertTw(m_oValue.value(), enUnitMeasure, 96);
			case Em:
			case Rem:        return m_oValue.value() * dPrevValue;
			case None:       return m_oValue.value();
		}
	}

	template<typename Operation>
	CDigit CDigit::ApplyOperation(const CDigit& oDigit, Operation operation) const
	{
		if (!Empty() && oDigit.Empty())
			return *this;

		if (Empty() && !oDigit.Empty())
			return oDigit;

		CDigit oTemp;

		oTemp.SetValue(operation(m_oValue.value(), oDigit.m_oValue.value()));

		oTemp.m_unLevel    = std::max(m_unLevel, oDigit.m_unLevel);
		oTemp.m_bImportant = std::max(m_bImportant, oDigit.m_bImportant);

		return oTemp;
	}

	CDigit::CDigit()
	    : m_enUnitMeasure(None)
	{}

	CDigit::CDigit(const double& dValue)
	    : CValueOptional(dValue, 0, false), m_enUnitMeasure(None)
	{}

	CDigit::CDigit(const double& dValue, unsigned int unLevel, bool bImportant)
	    : CValueOptional(dValue, unLevel, bImportant), m_enUnitMeasure(None)
	{}

	bool CDigit::Zero() const
	{
		if (CValueOptional::Empty())
			return false;

		return (std::abs(0. - m_oValue.value()) <= DBL_EPSILON);
	}

	void CDigit::Clear()
	{
		CValueOptional::Clear();
		m_enUnitMeasure = None;
	}
	
	void CDigit::ConvertTo(UnitMeasure enUnitMeasure, double dPrevValue)
	{
		m_oValue = ConvertValue(dPrevValue, enUnitMeasure);
		m_enUnitMeasure = enUnitMeasure;
	}

	int CDigit::ToInt() const
	{
		if (CValueOptional::Empty())
			return 0;

		return static_cast<int>(m_oValue.value() + 0.5);
	}

	double CDigit::ToDouble() const
	{
		return m_oValue.value_or(0.);
	}

	std::wstring CDigit::ToWString() const
	{
		if (CValueOptional::Empty())
			return std::wstring();

		return std::to_wstring(m_oValue.value());
	}

	int CDigit::ToInt(UnitMeasure enUnitMeasure, double dPrevValue) const
	{
		if (CValueOptional::Empty())
			return 0;

		return static_cast<int>(ConvertValue(dPrevValue, enUnitMeasure) + 0.5);
	}

	double CDigit::ToDouble(UnitMeasure enUnitMeasure, double dPrevValue) const
	{
		if (CValueOptional::Empty())
			return 0.;

		return ConvertValue(dPrevValue, enUnitMeasure);
	}

	std::wstring CDigit::ToWString(UnitMeasure enUnitMeasure, double dPrevValue) const
	{
		if (CValueOptional::Empty())
			return std::wstring();

		return std::to_wstring(ConvertValue(dPrevValue, enUnitMeasure));
	}

	UnitMeasure CDigit::GetUnitMeasure() const
	{
		return m_enUnitMeasure;
	}

	bool CDigit::operator==(const double &dValue) const
	{
		return (!CValueOptional::Empty()) && (std::fabs(dValue - m_oValue.value() <= DBL_EPSILON));
	}

	bool CDigit::operator==(const CDigit &oDigit) const
	{
		if (Empty() && oDigit.Empty())
			return true;

		if ((!Empty() && oDigit.Empty()) ||
		    (Empty() && !oDigit.Empty()))
			return false;

		return (m_enUnitMeasure == oDigit.m_enUnitMeasure &&
		        (std::fabs(oDigit.m_oValue.value() - m_oValue.value()) <= DBL_EPSILON));
	}

	bool CDigit::operator!=(const double &oValue) const
	{
		return (CValueOptional::Empty()) || (std::fabs(oValue - m_oValue.value()) > DBL_EPSILON);
	}

	bool CDigit::operator!=(const CDigit &oDigit) const
	{
		return !(*this == oDigit);
	}

	CDigit CDigit::operator+(const CDigit &oDigit) const
	{
		return ApplyOperation(oDigit, std::plus<double>());
	}

	CDigit CDigit::operator-(const CDigit &oDigit) const
	{
		return ApplyOperation(oDigit, std::minus<double>());
	}

	CDigit CDigit::operator*(const CDigit &oDigit) const
	{
		return ApplyOperation(oDigit, std::multiplies<double>());
	}

	CDigit CDigit::operator/(const CDigit &oDigit) const
	{
		return ApplyOperation(oDigit, std::divides<double>());
	}

	CDigit CDigit::operator*(double dValue) const
	{
		if (CValueOptional::Empty())
			return *this;

		CDigit oTemp(*this);

		oTemp *= dValue;

		return oTemp;
	}

	CDigit &CDigit::operator+=(const CDigit &oDigit)
	{
		if (m_unLevel > oDigit.m_unLevel || (m_bImportant && !oDigit.m_bImportant) || oDigit.Empty())
			return *this;

		if (Empty())
		{
			*this = oDigit;
			return *this;
		}
		else if (NSCSS::Percent == oDigit.m_enUnitMeasure && !Empty())
			*this *= oDigit.m_oValue.value() / 100.;
		else
			m_oValue.value() += oDigit.ToDouble(m_enUnitMeasure);

		m_unLevel       = oDigit.m_unLevel;
		m_bImportant    = oDigit.m_bImportant;

		return *this;
	}

	CDigit &CDigit::operator+=(double dValue)
	{
		if (Empty())
			m_oValue = dValue;
		else
			m_oValue.value() += dValue;

		return *this;
	}

	CDigit &CDigit::operator-=(double dValue)
	{
		if (Empty())
			m_oValue = -dValue;
		else
			m_oValue.value() -= dValue;

		return *this;
	}

	CDigit &CDigit::operator*=(double dValue)
	{
		if (!Empty())
			m_oValue.value() *= dValue;

		return *this;
	}

	CDigit &CDigit::operator/=(double dValue)
	{
		if (!Empty())
			m_oValue.value() /= dValue;

		return *this;
	}

	CDigit &CDigit::operator =(double dValue)
	{
		m_oValue = dValue;
		return *this;
	}

	bool CDigit::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || (CHECK_CONDITIONS && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		const bool bImportant{CutImportant(wsNewValue)}; //TODO:: иногда мы знаем, что "!important" точно не встретится
		                                                 // возможно стоит добавить ещё метод
		if (m_bImportant && !bImportant)
			return false;

		double dValue;

		if (!CUnitMeasureConverter::GetValue(wsValue, dValue, m_enUnitMeasure))
			return false;

		m_oValue = dValue;

		if (UINT_MAX == unLevel)
			m_unLevel++;
		else
			m_unLevel = unLevel;

		m_bImportant    = bImportant;

		return true;
	}
	
	bool CDigit::SetValue(const CDigit &oValue)
	{
		if (oValue.Empty() || m_unLevel > oValue.m_unLevel || (m_bImportant && !oValue.m_bImportant))
			return false;

		if (Empty())
		{
			m_oValue = oValue.m_oValue;
			m_enUnitMeasure = oValue.m_enUnitMeasure;
		}
		else if (NSCSS::Percent == oValue.m_enUnitMeasure)
		{
			if (m_unLevel == oValue.m_unLevel)
				m_oValue = oValue.m_oValue;
			else
				m_oValue.value() *= oValue.m_oValue.value() / 100.;
		}
		else
			m_oValue = oValue.ToDouble(m_enUnitMeasure);

		m_unLevel       = oValue.m_unLevel;
		m_bImportant    = oValue.m_bImportant;

		return true;
	}

	bool CDigit::SetValue(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		if (CHECK_CONDITIONS && !bHardMode)
			return false;

		m_oValue        = dValue;
		m_enUnitMeasure = enUnitMeasure;

		if (UINT_MAX == unLevel)
			m_unLevel++;
		else
			m_unLevel = unLevel;

		return true;
	}

	bool TRGB::Empty() const
	{
		return 0 == uchRed && 0 == uchGreen && 0 == uchBlue;
	}

	int TRGB::ToInt() const
	{
		return RGB_TO_INT(uchRed, uchGreen, uchBlue);
	}

	bool TRGB::operator==(const TRGB &oRGB) const
	{
		return uchRed   == oRGB.uchRed   &&
		       uchGreen == oRGB.uchGreen &&
		       uchBlue  == oRGB.uchBlue;
	}

	bool TRGB::operator!=(const TRGB& oRGB) const
	{
		return uchRed   != oRGB.uchRed   ||
		       uchGreen != oRGB.uchGreen ||
		       uchBlue  != oRGB.uchBlue;
	}

	CColorValue::CColorValue()
		: m_eType(EColorType::ColorNone), m_oValue(std::monostate())
	{}

	CColorValue::CColorValue(const CColorValue& oValue)
		: m_eType(oValue.m_eType), m_oValue(oValue.m_oValue)
	{}

	CColorValue::CColorValue(const std::wstring& wsValue)
	    : m_eType(EColorType::ColorHEX), m_oValue(wsValue)
	{}

	CColorValue::CColorValue(const TRGB& oValue)
		: m_eType(EColorType::ColorRGB), m_oValue(oValue)
	{}

	CColorValue::CColorValue(const CURL& oValue)
		: m_eType(EColorType::ColorUrl), m_oValue(oValue)
	{}

	EColorType CColorValue::GetType() const
	{
		return m_eType;
	}

	bool CColorValue::operator==(const CColorValue& oValue) const
	{
		return m_eType == oValue.m_eType && m_oValue == oValue.m_oValue;
	}

	CColorValueContextStroke::CColorValueContextStroke()
	{
		m_eType = EColorType::ColorContextStroke;
	}

	CColorValueContextFill::CColorValueContextFill()
	{
		m_eType = EColorType::ColorContextFill;
	}

	TRGB CColor::ConvertHEXtoRGB(const std::wstring &wsValue)
	{
		TRGB oRGB;

		if (wsValue.length() == 6)
			swscanf(wsValue.c_str(), L"%2hhx%2hhx%2hhx", &oRGB.uchRed, &oRGB.uchGreen, &oRGB.uchBlue);
		else if (wsValue.length() == 3)
		{
			swscanf(wsValue.c_str(), L"%1hhx%1hhx%1hhx", &oRGB.uchRed, &oRGB.uchGreen, &oRGB.uchBlue);
			oRGB.uchRed   *= 17;
			oRGB.uchGreen *= 17;
			oRGB.uchBlue  *= 17;
		}

		return oRGB;
	}

	std::wstring CColor::ConvertRGBtoHEX(const TRGB &oValue)
	{
		const int tempLen = 7;
		wchar_t arTemp[tempLen];

		swprintf(arTemp, tempLen, L"%02X%02X%02X", oValue.uchRed, oValue.uchGreen, oValue.uchBlue);

		return std::wstring(arTemp, 6);
	}

	CColor::CColor()
		: m_oOpacity(1.)
	{}

	void CColor::SetEmpty(unsigned int unLevel)
	{
		Clear();
		m_unLevel = unLevel;
	}
	
	void CColor::SetRGB(unsigned char uchR, unsigned char uchG, unsigned char uchB)
	{
		Clear();

		m_oValue = CColorValue(TRGB{uchR, uchG, uchB});
	}
	
	void CColor::SetRGB(const TRGB &oRGB)
	{
		Clear();

		m_oValue = CColorValue(oRGB);
	}
	
	void CColor::SetHEX(const std::wstring &wsValue)
	{
		if (6 != wsValue.length() && 3 != wsValue.length())
			return;

		Clear();

		if (6 == wsValue.length())
			m_oValue = CColorValue(std::wstring(wsValue));
		else
			m_oValue = CColorValue(std::wstring({wsValue[0], wsValue[0], wsValue[1], wsValue[1], wsValue[2], wsValue[2]}));
	}
	
	bool CColor::SetUrl(const std::wstring &wsValue)
	{
		if (wsValue.empty())
			return false;

		CURL oURL;

		if (!oURL.SetValue(wsValue))
			return false;

		Clear();

		m_oValue = CColorValue(oURL);

		return true;
	}
	
	void CColor::SetNone()
	{
		Clear();
		m_oValue = CColorValue();
	}

	char NormalizeNegativeColorValue(INT nValue)
	{
		if (nValue > 255)
			return 0xff;
		else if (nValue < 0)
			return (char)(std::abs(nValue) % 255);

		return (char)nValue;
	}

	bool CColor::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if ((CHECK_CONDITIONS && !bHardMode) || (wsValue.empty() && unLevel == m_unLevel))
			return false;

		if (wsValue.empty())
		{
			SetEmpty(unLevel);
			return false;
		}

		std::wstring wsNewValue(wsValue);

		const bool bImportant = CutImportant(wsNewValue);

		std::transform(wsNewValue.begin(), wsNewValue.end(), wsNewValue.begin(), std::towlower);
		NS_STATIC_FUNCTIONS::RemoveSpaces(wsNewValue);

		if (m_bImportant && !bImportant)
			return false;

		bool bResult{false};

		if (wsNewValue[0] == L'#')
		{
			SetHEX(wsNewValue.substr(1, wsNewValue.length() - 1));
			bResult = true;
		}
		else if (L"none" == wsNewValue || wsNewValue == L"transparent")
		{
			SetNone();
			bResult = true;
		}
		else if (L"context-stroke" == wsNewValue)
		{
			Clear();
			m_oValue = CColorValueContextStroke();
		}
		else if (L"context-fill" == wsNewValue)
		{
			Clear();
			m_oValue = CColorValueContextFill();
		}
		else if (10 <= wsNewValue.length() && wsNewValue.substr(0, 3) == L"rgb")
		{
			size_t unEnd = wsNewValue.find(L')', 4);

			if (std::wstring::npos == unEnd)
				return false;

			std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsNewValue.substr(4, unEnd - 4), false, L" (),");

			if (3 > arValues.size())
				return false;

			const char chRed   = NormalizeNegativeColorValue(std::ceil(NS_STATIC_FUNCTIONS::CalculatePersentage(arValues[0], 255)));
			const char chGreen = NormalizeNegativeColorValue(std::ceil(NS_STATIC_FUNCTIONS::CalculatePersentage(arValues[1], 255)));
			const char chBlue  = NormalizeNegativeColorValue(std::ceil(NS_STATIC_FUNCTIONS::CalculatePersentage(arValues[2], 255)));

			SetRGB(chRed, chGreen, chBlue);

			if (wsNewValue.substr(0, 4) == L"rgba" && 4 == arValues.size())
				m_oOpacity.SetValue(arValues[3], unLevel, bHardMode);

			bResult = true;
		}
		else if (5 <= wsNewValue.length() && SetUrl(wsValue))
			bResult = true;
		else
		{
			const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSConstValues::COLORS.find(wsNewValue);
			if (oHEX != NSConstValues::COLORS.end())
			{
				SetHEX(oHEX->second);
				bResult = true;
			}
		}

		if (!bResult)
			return false;

		m_bImportant = bImportant;

		if (UINT_MAX == unLevel)
			m_unLevel++;
		else
			m_unLevel = unLevel;

		return true;
	}

	bool CColor::SetOpacity(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oOpacity.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CColor::None() const
	{
		return !Empty() && ColorNone == m_oValue->GetType();
	}

	bool CColor::Url() const
	{
		return !Empty() && ColorUrl == m_oValue->GetType();
	}

	void CColor::Clear()
	{
		m_oOpacity.Clear();
		CValueOptional::Clear();
	}

	EColorType CColor::GetType() const
	{
		if (Empty())
			return EColorType::ColorNone;

		return m_oValue->GetType();
	}

	double CColor::GetOpacity() const
	{
		if (m_oOpacity.Empty())
			return 1.;

		if (UnitMeasure::Percent == m_oOpacity.GetUnitMeasure())
			return m_oOpacity.ToDouble() / 100.;

		if (UnitMeasure::None == m_oOpacity.GetUnitMeasure() && m_oOpacity.ToDouble() <= 1.)
			return m_oOpacity.ToDouble();

		return 1.;
	}

	int CColor::ToInt() const
	{
		if (Empty())
			return 0;

		switch(m_oValue->GetType())
		{
			case ColorRGB:
				return std::get<TRGB>(m_oValue->m_oValue).ToInt();
			case ColorHEX:
				return ConvertHEXtoRGB(std::get<std::wstring>(m_oValue->m_oValue)).ToInt();
			default:
				return 0;
		}
	}

	double CColor::ToDouble() const
	{
		return ToInt();
	}

	std::wstring CColor::ToWString() const
	{
		if (Empty())
			return std::wstring();

		switch(m_oValue->GetType())
		{
			case ColorRGB: return ConvertRGBtoHEX(std::get<TRGB>(m_oValue->m_oValue));
			case ColorHEX: return std::get<std::wstring>(m_oValue->m_oValue);
			case ColorUrl: return  std::get<CURL>(m_oValue->m_oValue).GetValue();
			default: return std::wstring();
		}
	}

	std::wstring CColor::ToHEX() const
	{
		if (Empty())
			return std::wstring();

		switch(m_oValue->GetType())
		{
			case ColorRGB:
				return ConvertRGBtoHEX(std::get<TRGB>(m_oValue->m_oValue));
			case ColorHEX:
				return std::get<std::wstring>(m_oValue->m_oValue);
			default:
				return std::wstring();
		}
	}

	std::wstring CColor::EquateToColor(const std::vector<std::pair<TRGB, std::wstring>> &arColors) const
	{
		if (arColors.empty() || Empty())
			return L"none";

		TRGB oCurrentColor;

		switch(m_oValue->GetType())
		{
			case ColorRGB: oCurrentColor = std::get<TRGB>(m_oValue->m_oValue); break;
			case ColorHEX: oCurrentColor = ConvertHEXtoRGB(std::get<std::wstring>(m_oValue->m_oValue)); break;
			default: return L"none";
		}

		std::wstring wsSelectedColor;
		double dMinDistance = DBL_MAX;
		double dDistance;

		for (const std::pair<TRGB, std::wstring>& oColor : arColors)
		{
			dDistance = sqrt(pow(oCurrentColor.uchRed - oColor.first.uchRed, 2) + pow(oCurrentColor.uchGreen - oColor.first.uchGreen, 2) + pow(oCurrentColor.uchBlue - oColor.first.uchBlue, 2));

			if (dDistance < dMinDistance)
			{
				dMinDistance = dDistance;
				wsSelectedColor = oColor.second;
			}
		}

		return wsSelectedColor;
	}

	TRGB CColor::ToRGB() const
	{
		if (Empty())
			return TRGB();

		switch(m_oValue->GetType())
		{
			case ColorRGB: return std::get<TRGB>(m_oValue->m_oValue);
			case ColorHEX: return ConvertHEXtoRGB(std::get<std::wstring>(m_oValue->m_oValue));
			default: return TRGB();
		}
	}

	std::vector<std::wstring> CMatrix::CutTransforms(const std::wstring &wsValue) const
	{
		if (wsValue.length() < 8)
			return std::vector<std::wstring>();

//		std::wregex oRegex(L"(matrix|translate|scale|rotate)[(](([0-9]*\\.?[0-9]*)(\\s|,)*)+[)]");

		std::wstring::const_iterator citBegin = std::find_if(wsValue.begin(), wsValue.end(), std::iswalpha);
		std::wstring::const_iterator citEnd;

		std::vector<std::wstring> arValues;

		while (wsValue.end() != citBegin)
		{
			citEnd = std::find_if(citBegin, wsValue.end(), [](wchar_t wChar){return L')' == wChar;});
			arValues.push_back(std::wstring(citBegin, citEnd + 1));
			citBegin = citEnd + 1;
			citBegin =  std::find_if(citBegin, wsValue.end(), std::iswalpha);
		}

		return arValues;
	}

	CMatrix::CMatrix()
	{}

	CMatrix::CMatrix(const MatrixValues &oValue, unsigned int unLevel, bool bImportant)
	    : CValueBase(oValue, unLevel, bImportant)
	{}

	bool CMatrix::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if ((CHECK_CONDITIONS && !bHardMode) || (wsValue.empty() && unLevel == m_unLevel))
			return false;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		std::transform(wsNewValue.begin(), wsNewValue.end(), wsNewValue.begin(), std::towlower);

		if (m_bImportant && !bImportant)
			return false;

		std::vector<std::wstring> arTransforms = CutTransforms(wsNewValue);

		Clear();

		TransformType enType;

		for (const std::wstring& wsTransform : arTransforms)
		{
			if (std::wstring::npos != wsTransform.find(L"matrix"))
				enType = TransformMatrix;
			else if (std::wstring::npos != wsTransform.find(L"translate"))
				enType = TransformTranslate;
			else if (std::wstring::npos != wsTransform.find(L"scale"))
				enType = TransformScale;
			else if (std::wstring::npos != wsTransform.find(L"rotate"))
				enType = TransformRotate;
			else if (std::wstring::npos != wsTransform.find(L"skewx"))
				enType = TransformSkewX;
			else if (std::wstring::npos != wsTransform.find(L"skewy"))
				enType = TransformSkewY;
			else
			{
				Clear();
				return false;
			}

			std::vector<double> arValues = NS_STATIC_FUNCTIONS::ReadDoubleValues(wsTransform);

			if (arValues.empty())
				return false;

			if (!AddValue(arValues, enType))
			{
				Clear();
				return false;
			}
		}

		m_unLevel    = unLevel;
		m_bImportant = bImportant;

		return true;
	}

	bool CMatrix::SetMatrix(const Aggplus::CMatrix &oValue)
	{
		m_oValue.clear();
		m_oValue.push_back(std::make_pair(std::vector<double>{oValue.sx(), oValue.shy(), oValue.shx(), oValue.sy(), oValue.tx(), oValue.ty()}, TransformMatrix));
		return true;
	}

	bool CMatrix::AddValue(const std::vector<double> &arValues, TransformType enType)
	{
		switch (enType)
		{
			case TransformMatrix:
			{
				if (6 != arValues.size())
					return false;

				m_oValue.push_back(std::make_pair(arValues, TransformMatrix));
				break;
			}
			case TransformTranslate:
			{
				if (1 == arValues.size())
					m_oValue.push_back(std::make_pair(std::vector<double>{arValues[0], 0.}, TransformTranslate));
				else if (2 == arValues.size())
					m_oValue.push_back(std::make_pair(arValues, TransformTranslate));
				else
					return false;

				break;
			}
			case TransformScale:
			{
				if (1 == arValues.size())
					m_oValue.push_back(std::make_pair(std::vector<double>{arValues[0], arValues[0]}, TransformScale));
				else if (2 == arValues.size())
					m_oValue.push_back(std::make_pair(arValues, TransformScale));
				else
					return false;

				break;
			}
			case TransformRotate:
			{
				if (1 == arValues.size())
					m_oValue.push_back(std::make_pair(std::vector<double>{arValues[0], 0., 0.}, TransformRotate));
				else if (3 == arValues.size())
					m_oValue.push_back(std::make_pair(arValues, TransformRotate));
				else
					return false;

				break;
			}
			case TransformSkewX:
			case TransformSkewY:
			{
				if (arValues.empty())
					return false;
				
				m_oValue.push_back(std::make_pair(std::vector<double>{arValues[0]}, enType));
				
				break;
			}
			default:
				break;
		}

		return true;
	}

	bool CMatrix::Empty() const
	{
		return m_oValue.empty();
	}

	void CMatrix::Clear()
	{
		m_oValue.clear();
		m_unLevel    = 0;
		m_bImportant = false;
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
		if (Empty())
			return std::wstring();

		Aggplus::CMatrix oMatrix = GetFinalValue();

		TransformType enType = (1 == m_oValue.size()) ? m_oValue[0].second : TransformMatrix;
		std::wstring wsValue;

		switch (enType)
		{
			case TransformMatrix:
			{
				wsValue = L"matrix(";
				wsValue += std::to_wstring(oMatrix.sx())  + L',';
				wsValue += std::to_wstring(oMatrix.shx()) + L',';
				wsValue += std::to_wstring(oMatrix.shy()) + L',';
				wsValue += std::to_wstring(oMatrix.sy())  + L',';
				wsValue += std::to_wstring(oMatrix.tx())  + L',';
				wsValue += std::to_wstring(oMatrix.ty())  + L')';
				break;
			}
			case TransformTranslate:
			{
				if (0. == oMatrix.tx() && 0. == oMatrix.ty())
					return std::wstring();

				wsValue = L"translate(";
				wsValue += std::to_wstring(oMatrix.tx())  + L',';
				wsValue += std::to_wstring(oMatrix.ty())  + L')';
				break;
			}
			case TransformScale:
			{
				if (1. == oMatrix.sx() && 1. == oMatrix.sy())
					return std::wstring();

				wsValue = L"scale(";
				wsValue += std::to_wstring(oMatrix.sx())  + L',';
				wsValue += std::to_wstring(oMatrix.sy())  + L')';
				break;
			}
			case TransformRotate:
			{
				//TODO:: добавить реализацию
				wsValue = L"rotate(";
				break;
			}
			default:
				break;
		}

		return wsValue;
	}

	Aggplus::CMatrix CMatrix::GetFinalValue(TransformType oWithoutType) const
	{
		Aggplus::CMatrix oMatrix;

		for (const std::pair<std::vector<double>, TransformType>& oElement : m_oValue)
		{
			if (oWithoutType == oElement.second)
				continue;

			switch(oElement.second)
			{
				case TransformMatrix:
				{
					Aggplus::CMatrix oTempMatrix(oElement.first[0], oElement.first[1], oElement.first[2], oElement.first[3], oElement.first[4], oElement.first[5]);

					oMatrix.Multiply(&oTempMatrix);
					break;
				}
				case TransformTranslate:
				{
					oMatrix.Translate(oElement.first[0], oElement.first[1]);
					break;
				}
				case TransformScale:
				{
					oMatrix.Scale(oElement.first[0], oElement.first[1]);
					break;
				}
				case TransformRotate:
				{
					oMatrix.RotateAt(oElement.first[0], -oElement.first[1], -oElement.first[2]);
					break;
				}
				case TransformSkewX:
				{
					oMatrix.Shear(oElement.first[0] * 3.14 / 180.0, 0);
					break;
				}
				case TransformSkewY:
				{
					oMatrix.Shear(0, oElement.first[0] * 3.14 / 180.0);
					break;
				}
				default: break;
			}
		}

		return oMatrix;
	}

	void CMatrix::ApplyTranform(Aggplus::CMatrix &oMatrix, Aggplus::MatrixOrder order) const
	{
		for (const std::pair<std::vector<double>, TransformType>& oElement : m_oValue)
		{
			switch(oElement.second)
			{
				case TransformMatrix:
				{
					Aggplus::CMatrix oTempMatrix(oElement.first[0], oElement.first[1], oElement.first[2], oElement.first[3], oElement.first[4], oElement.first[5]);

					oMatrix.Multiply(&oTempMatrix, order);
					break;
				}
				case TransformTranslate:
				{
					oMatrix.Translate(oElement.first[0], oElement.first[1], order);
					break;
				}
				case TransformScale:
				{
					oMatrix.Scale(oElement.first[0], oElement.first[1], order);
					break;
				}
				case TransformRotate:
				{
					oMatrix.RotateAt(oElement.first[0], -oElement.first[1], -oElement.first[2], order);
					break;
				}
				case TransformSkewX:
				{
					oMatrix.Shear(oElement.first[0] * 3.14 / 180.0, 0, order);
					break;
				}
				case TransformSkewY:
				{
					oMatrix.Shear(0, oElement.first[0] * 3.14 / 180.0, order);
					break;
				}
				default: break;
			}
		}

	}

	bool CMatrix::operator==(const CMatrix &oMatrix) const
	{
		if (Empty() && oMatrix.Empty())
			return true;

		if (m_oValue.size() != oMatrix.m_oValue.size())
			return false;

		for (unsigned int unIndex = 0; unIndex < m_oValue.size(); ++unIndex)
		{
			if (m_oValue[unIndex].second != oMatrix.m_oValue[unIndex].second ||
			    m_oValue[unIndex].first != oMatrix.m_oValue[unIndex].first)
				return false;
		}

		return true;
	}

	CMatrix& CMatrix::operator+=(const CMatrix& oMatrix)
	{
		m_oValue.insert(m_oValue.end(), oMatrix.m_oValue.begin(), oMatrix.m_oValue.end());
		return *this;
	}

	CMatrix& CMatrix::operator-=(const CMatrix& oMatrix)
	{
		for (MatrixValues::const_reverse_iterator itElement = oMatrix.m_oValue.crbegin(); itElement < oMatrix.m_oValue.crend(); ++itElement)
		{
			if (!m_oValue.empty() && m_oValue.back() == *itElement)
				m_oValue.pop_back();
		}

		return *this;
	}

	// DISPLAY
	CDisplay::CDisplay()
	{
		m_eWhiteSpace.SetMapping({{L"normal",   EWhiteSpace::Normal  },
		                          {L"nowrap",   EWhiteSpace::Nowrap  },
		                          {L"pre",      EWhiteSpace::Pre     },
		                          {L"pre-line", EWhiteSpace::Pre_Line},
		                          {L"pre-wrap", EWhiteSpace::Pre_Wrap}},
		                                        EWhiteSpace::Normal);
	}

	void CDisplay::Equation(CDisplay &oFirstDisplay, CDisplay &oSecondDisplay)
	{
		CDigit::Equation(oFirstDisplay.m_oX, oSecondDisplay.m_oX);
		CDigit::Equation(oFirstDisplay.m_oY, oSecondDisplay.m_oY);
		CDigit::Equation(oFirstDisplay.m_oWidth, oSecondDisplay.m_oWidth);
		CDigit::Equation(oFirstDisplay.m_oHeight, oSecondDisplay.m_oHeight);

		CString::Equation(oFirstDisplay.m_oVAlign, oSecondDisplay.m_oVAlign);
		CString::Equation(oFirstDisplay.m_oHAlign, oSecondDisplay.m_oHAlign);

		CString::Equation(oFirstDisplay.m_oDisplay, oSecondDisplay.m_oDisplay);

		CEnum::Equation(oFirstDisplay.m_eWhiteSpace, oSecondDisplay.m_eWhiteSpace);
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
		return m_oVAlign.SetValue(wsValue, {std::make_pair(L"top", L"top"), std::make_pair(L"baseline", L"top"), std::make_pair(L"text-top", L"top"), std::make_pair(L"bottom", L"bottom"), std::make_pair(L"text-bottom", L"bottom"), std::make_pair(L"middle", L"center")}, unLevel, bHardMode);
	}

	bool CDisplay::SetDisplay(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oDisplay.SetValue(wsValue, NSConstValues::DISPLAY_VALUES, unLevel, bHardMode);
	}

	bool CDisplay::SetWhiteSpace(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_eWhiteSpace.SetValue(wsValue, unLevel, bHardMode);
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

	const CEnum& CDisplay::GetWhiteSpace() const
	{
		return m_eWhiteSpace;
	}

	bool CDisplay::Empty() const
	{
		return m_oX.Empty() && m_oY.Empty() && m_oWidth.Empty() && m_oHeight.Empty() &&
		       m_oHeight.Empty() && m_oVAlign.Empty() && m_oDisplay.Empty() &&
		       (m_eWhiteSpace.Empty() || m_eWhiteSpace == EWhiteSpace::Normal);
	}

	void CDisplay::Clear()
	{
		m_oX.Clear();
		m_oY.Clear();
		m_oWidth.Clear();
		m_oHeight.Clear();

		m_oHAlign.Clear();
		m_oVAlign.Clear();

		m_oDisplay.Clear();

		m_eWhiteSpace.Clear();
	}

	CDisplay &CDisplay::operator+=(const CDisplay &oDisplay)
	{
		m_oX          += oDisplay.m_oX;
		m_oY          += oDisplay.m_oY;
		m_oWidth       = oDisplay.m_oWidth;
		m_oHeight      = oDisplay.m_oHeight;
		m_oHAlign     += oDisplay.m_oHAlign;
		m_oVAlign     += oDisplay.m_oVAlign;
		m_oDisplay    += oDisplay.m_oDisplay;
		m_eWhiteSpace += oDisplay.m_eWhiteSpace;

		return *this;
	}

	bool CDisplay::operator==(const CDisplay &oDisplay) const
	{
		return m_oX          == oDisplay.m_oX       &&
		       m_oY          == oDisplay.m_oY       &&
		       m_oWidth      == oDisplay.m_oWidth   &&
		       m_oHeight     == oDisplay.m_oHeight  &&
		       m_oHAlign     == oDisplay.m_oHAlign  &&
		       m_oVAlign     == oDisplay.m_oVAlign  &&
		       m_oDisplay    == oDisplay.m_oDisplay &&
		       m_eWhiteSpace == oDisplay.m_eWhiteSpace.ToInt();
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
	CBackground::CBackground()
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
		const std::vector<std::wstring> &arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsValue, false, L" ");

		for (const std::wstring& wsValue : arValues)
		{
			if (SetColor(wsValue, unLevel, bHardMode))
				return true;
		}

		return false;
	}

	const CColor& CBackground::GetColor() const
	{
		return m_oColor;
	}
	
	void CBackground::Clear()
	{
		m_oColor.Clear();
	}

	bool CBackground::Empty() const
	{
		return m_oColor.Empty();
	}

	bool CBackground::IsNone() const
	{
		return EColorType::ColorNone == m_oColor.GetType();
	}

	CBackground &CBackground::operator=(const CBackground &oBackground)
	{
		m_oColor    = oBackground.m_oColor;

		return *this;
	}

	CBackground &CBackground::operator+=(const CBackground &oBackground)
	{
		m_oColor += oBackground.m_oColor;

		return *this;
	}

	bool CBackground::operator==(const CBackground &oBackground) const
	{
		return m_oColor    == oBackground.m_oColor;
	}

	// TRANSFORM
	CTransform::CTransform()
	{}

	void CTransform::Clear()
	{
		m_oMatrix.Clear();
	}

	void CTransform::Equation(CTransform &oFirstTransform, CTransform &oSecondTransform)
	{
		CMatrix::Equation(oFirstTransform.m_oMatrix, oSecondTransform.m_oMatrix);
	}

	bool CTransform::SetMatrix(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oMatrix.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CTransform::SetMatrix(const Aggplus::CMatrix &oMatrix)
	{
		m_oMatrix.SetMatrix(oMatrix);
		return true;
	}

	void CTransform::Translate(double dOffsetX, double dOffsetY)
	{
		m_oMatrix.AddValue({dOffsetX, dOffsetY}, TransformTranslate);
	}

	void CTransform::Scale(double dScaleX, double dScaleY)
	{
		m_oMatrix.AddValue({dScaleX, dScaleY}, TransformScale);
	}

	void CTransform::Rotate(double dValue)
	{
		m_oMatrix.AddValue({dValue}, TransformRotate);
	}

	void CTransform::RotateAt(double dValue, double dX, double dY)
	{
		m_oMatrix.AddValue({dValue, dX, dY}, TransformRotate);
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

	CBorderSide::CBorderSide(const CBorderSide& oBorderSide)
		: m_oWidth(oBorderSide.m_oWidth), m_oStyle(oBorderSide.m_oStyle), m_oColor(oBorderSide.m_oColor), m_bBlock(oBorderSide.m_bBlock)
	{}

	void CBorderSide::Clear()
	{
		m_oWidth.Clear();
		m_oStyle.Clear();
		m_oColor.Clear();
	}

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

		if (L"none" == wsValue)
		{
			SetNone(unLevel, bHardMode);
			return true;
		}

		const std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::ParseCSSPropertie(wsValue);
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
		const std::map<std::wstring, std::wstring> arAbsoluteBorderValues =
			{{L"thin",    L"2px"},
			 {L"medium",  L"4px"},
			 {L"thick",   L"6px"}};

		size_t unFoundPos = std::wstring::npos;
		std::wstring wsNewValue(wsValue);

		for (const std::pair<std::wstring, std::wstring> oAbsValue : arAbsoluteBorderValues)
		{
			unFoundPos = wsNewValue.find(oAbsValue.first);
			if (std::wstring::npos != unFoundPos)
				wsNewValue.replace(unFoundPos, oAbsValue.first.length(), oAbsValue.second);
		}

		return m_oWidth.SetValue(wsNewValue, unLevel, bHardMode);
	}

	bool CBorderSide::SetWidth(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oWidth.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CBorderSide::SetStyle(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oStyle.SetValue(wsValue, {std::make_pair(L"dotted", L"dotted"), std::make_pair(L"dashed", L"dashed"), std::make_pair(L"solid", L"single"),
		                                   std::make_pair(L"double", L"double"), std::make_pair(L"groove", L"threeDEmboss"), std::make_pair(L"ridge", L"threeDEngrave"),
		                                   std::make_pair(L"inset", L"inset"), std::make_pair(L"outset", L"outset")}, unLevel, bHardMode);
	}

	bool CBorderSide::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oColor.SetValue(wsValue, unLevel, bHardMode);
	}

	void CBorderSide::SetNone(unsigned int unLevel, bool bHardMode)
	{
		SetColor(L"#ffffff", unLevel, bHardMode);
		SetStyle(L"solid", unLevel, bHardMode);
		SetWidth(L"0",unLevel,bHardMode);
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
		return m_oWidth.Empty() || m_oColor.None();
	}

	bool CBorderSide::Zero() const
	{
		return m_oWidth.Zero();
	}

	bool CBorderSide::Valid() const
	{
		return !m_oWidth.Empty() && !m_oWidth.Zero();
	}

	CBorderSide &CBorderSide::operator+=(const CBorderSide &oBorderSide)
	{
		m_oWidth = oBorderSide.m_oWidth;
		m_oStyle = oBorderSide.m_oStyle;
		m_oColor = oBorderSide.m_oColor;

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

	bool CBorderSide::operator!=(const CBorderSide& oBorderSide) const
	{
		return m_oWidth != oBorderSide.m_oWidth ||
		       m_oStyle != oBorderSide.m_oStyle ||
		       m_oColor != oBorderSide.m_oColor;
	}

	CBorderSide &CBorderSide::operator =(const CBorderSide &oBorderSide)
	{
		m_oWidth = oBorderSide.m_oWidth;
		m_oStyle = oBorderSide.m_oStyle;
		m_oColor = oBorderSide.m_oColor;

		return *this;
	}

	// BORDER
	CBorder::CBorder()
	{
		m_enCollapse.SetMapping({{L"collapse", BorderCollapse::Collapse}, {L"separate", BorderCollapse::Separate}}, BorderCollapse::Separate);
	}

	void CBorder::Clear()
	{
		ClearLeftSide();
		ClearTopSide();
		ClearRightSide();
		ClearBottomSide();
	}

	void CBorder::ClearLeftSide()
	{
		m_oLeft.Clear();
	}

	void CBorder::ClearTopSide()
	{
		m_oTop.Clear();
	}

	void CBorder::ClearRightSide()
	{
		m_oRight.Clear();
	}

	void CBorder::ClearBottomSide()
	{
		m_oBottom.Clear();
	}

	void CBorder::Equation(CBorder &oFirstBorder, CBorder &oSecondBorder)
	{
		CBorderSide::Equation(oFirstBorder.m_oLeft,   oSecondBorder.m_oLeft);
		CBorderSide::Equation(oFirstBorder.m_oTop,    oSecondBorder.m_oTop);
		CBorderSide::Equation(oFirstBorder.m_oRight,  oSecondBorder.m_oRight);
		CBorderSide::Equation(oFirstBorder.m_oBottom, oSecondBorder.m_oBottom);
	}

	bool CBorder::SetSides(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		bool bResult = false;

		if (m_oLeft  .SetValue(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oTop   .SetValue(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oRight .SetValue(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oBottom.SetValue(wsValue, unLevel, bHardMode)) bResult = true;

		return bResult;
	}

	bool CBorder::SetWidth(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		bool bResult = false;
		
		if (m_oLeft  .SetWidth(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oTop   .SetWidth(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oRight .SetWidth(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oBottom.SetWidth(wsValue, unLevel, bHardMode)) bResult = true;

		return bResult;
	}

	bool CBorder::SetWidth(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		bool bResult = false;

		if (m_oLeft  .SetWidth(dValue, enUnitMeasure, unLevel, bHardMode)) bResult = true;
		if (m_oTop   .SetWidth(dValue, enUnitMeasure, unLevel, bHardMode)) bResult = true;
		if (m_oRight .SetWidth(dValue, enUnitMeasure, unLevel, bHardMode)) bResult = true;
		if (m_oBottom.SetWidth(dValue, enUnitMeasure, unLevel, bHardMode)) bResult = true;

		return bResult;
	}

	bool CBorder::SetStyle(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		bool bResult = false;
		
		if (m_oLeft  .SetStyle(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oTop   .SetStyle(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oRight .SetStyle(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oBottom.SetStyle(wsValue, unLevel, bHardMode)) bResult = true;

		return bResult;
	}

	bool CBorder::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		bool bResult = false;

		if (m_oLeft  .SetColor(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oTop   .SetColor(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oRight .SetColor(wsValue, unLevel, bHardMode)) bResult = true;
		if (m_oBottom.SetColor(wsValue, unLevel, bHardMode)) bResult = true;

		return bResult;
	}

	bool CBorder::SetCollapse(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_enCollapse.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetLeftSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetWidthLeftSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft.SetWidth(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetWidthLeftSide(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft.SetWidth(dValue, enUnitMeasure, unLevel, bHardMode);
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

	bool CBorder::SetWidthTopSide(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oTop.SetWidth(dValue, enUnitMeasure, unLevel, bHardMode);
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

	bool CBorder::SetWidthRightSide(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oRight.SetWidth(dValue, enUnitMeasure, unLevel, bHardMode);
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

	bool CBorder::SetWidthBottomSide(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oBottom.SetWidth(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CBorder::SetStyleBottomSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oBottom.SetStyle(wsValue, unLevel, bHardMode);
	}

	bool CBorder::SetColorBottomSide(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oBottom.SetColor(wsValue, unLevel, bHardMode);
	}

	void CBorder::SetNone(unsigned int unLevel, bool bHardMode)
	{
		m_oLeft  .SetNone(unLevel, bHardMode);
		m_oTop   .SetNone(unLevel, bHardMode);
		m_oRight .SetNone(unLevel, bHardMode);
		m_oBottom.SetNone(unLevel, bHardMode);
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

	bool CBorder::Zero() const
	{
		return m_oLeft.Zero()  && m_oTop.Zero() &&
		       m_oRight.Zero() && m_oBottom.Zero();
	}

	bool CBorder::EqualSides() const
	{
		return m_oLeft == m_oTop && m_oTop == m_oRight && m_oRight == m_oBottom;
	}

	const CEnum &CBorder::GetCollapse() const
	{
		return m_enCollapse;
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
		m_enCollapse = oBorder.m_enCollapse;

		if (oBorder.Empty())
			return *this;

		m_oLeft      = oBorder.m_oLeft;
		m_oTop       = oBorder.m_oTop;
		m_oRight     = oBorder.m_oRight;
		m_oBottom    = oBorder.m_oBottom;

		return *this;
	}

	bool CBorder::operator==(const CBorder &oBorder) const
	{
		return m_oLeft   == oBorder.m_oLeft  &&
		       m_oTop    == oBorder.m_oTop   &&
		       m_oRight  == oBorder.m_oRight &&
		       m_oBottom == oBorder.m_oBottom;
	}

	bool CBorder::operator!=(const CBorder& oBorder) const
	{
		return m_oLeft   != oBorder.m_oLeft  ||
		       m_oTop    != oBorder.m_oTop   ||
		       m_oRight  != oBorder.m_oRight ||
		       m_oBottom != oBorder.m_oBottom;
	}

	CBorder &CBorder::operator =(const CBorder &oBorder)
	{
		m_oLeft   = oBorder.m_oLeft;
		m_oTop    = oBorder.m_oTop;
		m_oRight  = oBorder.m_oRight;
		m_oBottom = oBorder.m_oBottom;

		m_enCollapse = oBorder.m_enCollapse;

		return *this;
	}

	// TEXT
	CText::CText()
	{}

	void CText::Equation(CText &oFirstText, CText &oSecondText)
	{
		CDigit ::Equation(oFirstText.m_oIndent,     oSecondText.m_oIndent);
		CString::Equation(oFirstText.m_oAlign,      oSecondText.m_oAlign);
//		CString::Equation(oFirstText.m_oDecoration, oSecondText.m_oDecoration);
		CColor ::Equation(oFirstText.m_oColor,      oSecondText.m_oColor);
		CColor ::Equation(oFirstText.m_oHighlight,  oSecondText.m_oHighlight);
	}

	bool CText::SetIndent(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oIndent.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CText::SetAlign(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oAlign.SetValue(wsValue, {std::make_pair(L"center", L"center"), std::make_pair(L"middle", L"center"), std::make_pair(L"justify", L"both"), std::make_pair(L"left",  L"left"), std::make_pair(L"start", L"left"), std::make_pair(L"right", L"right"), std::make_pair(L"end", L"right")}, unLevel, bHardMode);
	}

	bool CText::SetDecoration(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty())
			return false;

		for (const std::wstring& wsVal : NSCSS::NS_STATIC_FUNCTIONS::GetWordsW(wsValue))
		{
			if (m_oDecoration.m_oLine.SetValue(wsVal) ||
			    m_oDecoration.m_oStyle.SetValue(wsValue, {L"solid", L"double", L"dotted", L"dashed", L"wavy"}, unLevel, bHardMode) ||
			    m_oDecoration.m_oColor.SetValue(wsValue, unLevel, bHardMode))
				continue;
			else
				return false;
		}

		return true;
	}

	bool CText::SetColor(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oColor.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CText::SetHighlight(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oHighlight.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CText::SetBaselineShift(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oBaselineShift.SetValue(wsValue, unLevel, bHardMode);
	}

	const CDigit& CText::GetIndent() const
	{
		return m_oIndent;
	}

	const CString& CText::GetAlign() const
	{
		return m_oAlign;
	}

	const TTextDecoration& CText::GetDecoration() const
	{
		return m_oDecoration;
	}

	const CColor& CText::GetColor() const
	{
		return m_oColor;
	}

	const CColor& CText::GetHighlight() const
	{
		return m_oHighlight;
	}

	EBaselineShift CText::GetBaselineShiftType() const
	{
		return m_oBaselineShift.GetType();
	}

	double CText::GetBaselineShiftValue() const
	{
		return m_oBaselineShift.GetValue();
	}

	bool CText::Empty() const
	{
		return m_oIndent.Empty()     && m_oAlign.Empty() &&
		       m_oDecoration.m_oLine.Empty() && m_oColor.Empty() &&
		       m_oBaselineShift.Empty();
	}

	bool CText::Underline() const
	{
		return m_oDecoration.m_oLine.Underline();
	}

	bool CText::Overline() const
	{
		return m_oDecoration.m_oLine.Overline();
	}

	bool CText::LineThrough() const
	{
		return m_oDecoration.m_oLine.LineThrough();
	}

	CText &CText::operator+=(const CText &oText)
	{
		m_oIndent     += oText.m_oIndent;
		m_oAlign      += oText.m_oAlign;
		m_oDecoration += oText.m_oDecoration;
		m_oColor      += oText.m_oColor;
		m_oHighlight  += oText.m_oHighlight;

		return *this;
	}

	bool CText::operator==(const CText &oText) const
	{
		return m_oIndent     == oText.m_oIndent     &&
		       m_oAlign      == oText.m_oAlign      &&
		       m_oDecoration == oText.m_oDecoration &&
		       m_oColor      == oText.m_oColor      &&
		       m_oHighlight  == oText.m_oHighlight;
	}

	// MARGIN
	CIndent::CIndent()
		: m_bPermission(true)
	{}

	void CIndent::Clear()
	{
		m_oTop   .Clear();
		m_oRight .Clear();
		m_oBottom.Clear();
		m_oLeft  .Clear();
	}

	void CIndent::SetPermisson(bool bPermission)
	{
		m_bPermission = bPermission;
	}

	void CIndent::Equation(CIndent &oFirstMargin, CIndent &oSecondMargin)
	{
		CDigit::Equation(oFirstMargin.m_oLeft,   oSecondMargin.m_oLeft);
		CDigit::Equation(oFirstMargin.m_oTop,    oSecondMargin.m_oTop);
		CDigit::Equation(oFirstMargin.m_oRight,  oSecondMargin.m_oRight);
		CDigit::Equation(oFirstMargin.m_oBottom, oSecondMargin.m_oBottom);
	}

	bool CIndent::Equals() const
	{
		if (Empty())
			return true;

		return m_oLeft == m_oTop && m_oTop == m_oRight && m_oRight == m_oBottom;
	}

	bool CIndent::SetValues(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (!m_bPermission)
			return false;

		const std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsValue, false, L" ");
		switch (arValues.size())
		{
			case 1:
				return SetValues(arValues[0], arValues[0], arValues[0], arValues[0], unLevel, bHardMode);
			case 2:
				return SetValues(arValues[0], arValues[1], arValues[0], arValues[1], unLevel, bHardMode);
			case 3:
				return SetValues(arValues[0], arValues[1], arValues[2], arValues[1], unLevel, bHardMode);
			case 4:
				return SetValues(arValues[0], arValues[1], arValues[2], arValues[3], unLevel, bHardMode);
		}

		return false;
	}

	bool CIndent::SetTop(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oTop.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CIndent::SetRight(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oRight.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CIndent::SetBottom(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oBottom.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CIndent::SetLeft(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CIndent::SetValues(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		const bool bTopResult    = SetTop    (dValue, enUnitMeasure, unLevel, bHardMode);
		const bool bRightResult  = SetRight  (dValue, enUnitMeasure, unLevel, bHardMode);
		const bool bBottomResult = SetBottom (dValue, enUnitMeasure, unLevel, bHardMode);
		const bool bLeftResult   = SetLeft   (dValue, enUnitMeasure, unLevel, bHardMode);

		return bTopResult || bRightResult || bBottomResult || bLeftResult;
	}

	bool CIndent::SetTop(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oTop.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CIndent::SetRight(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oRight.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CIndent::SetBottom(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oBottom.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CIndent::SetLeft(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oLeft.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	void CIndent::UpdateAll(const double& dParentFontSize, const double& dCoreFontSize)
	{
		UpdateTop   (dParentFontSize, dCoreFontSize);
		UpdateRight (dParentFontSize, dCoreFontSize);
		UpdateBottom(dParentFontSize, dCoreFontSize);
		UpdateLeft  (dParentFontSize, dCoreFontSize);
	}

	void CIndent::UpdateTop(const double& dParentFontSize, const double& dCoreFontSize)
	{
		UpdateSide(m_oTop, dParentFontSize, dCoreFontSize);
	}

	void CIndent::UpdateRight(const double& dParentFontSize, const double& dCoreFontSize)
	{
		UpdateSide(m_oRight, dParentFontSize, dCoreFontSize);
	}

	void CIndent::UpdateBottom(const double& dParentFontSize, const double& dCoreFontSize)
	{
		UpdateSide(m_oBottom, dParentFontSize, dCoreFontSize);
	}

	void CIndent::UpdateLeft(const double& dParentFontSize, const double& dCoreFontSize)
	{
		UpdateSide(m_oLeft, dParentFontSize, dCoreFontSize);
	}

	const CDigit &CIndent::GetTop() const
	{
		return m_oTop;
	}

	const CDigit &CIndent::GetRight() const
	{
		return m_oRight;
	}

	const CDigit &CIndent::GetBottom() const
	{
		return m_oBottom;
	}

	const CDigit &CIndent::GetLeft() const
	{
		return m_oLeft;
	}

	bool CIndent::Empty() const
	{
		return m_oTop.Empty() && m_oRight.Empty() && m_oBottom.Empty() && m_oLeft.Empty();
	}

	bool CIndent::Zero() const
	{
		return m_oTop.Zero() && m_oRight.Zero() && m_oBottom.Zero() && m_oLeft.Zero();
	}

	CIndent &CIndent::operator+=(const CIndent &oIndent)
	{
		if (!oIndent.m_oTop.Empty())    m_oTop    = oIndent.m_oTop;
		if (!oIndent.m_oRight.Empty())  m_oRight  = oIndent.m_oRight;
		if (!oIndent.m_oBottom.Empty()) m_oBottom = oIndent.m_oBottom;
		if (!oIndent.m_oLeft.Empty())   m_oLeft   = oIndent.m_oLeft;

		return *this;
	}
	
	bool CIndent::operator==(const CIndent &oIndent) const
	{
		return m_oTop    == oIndent.m_oTop    &&
		       m_oRight  == oIndent.m_oRight  && 
		       m_oBottom == oIndent.m_oBottom &&
		       m_oLeft   == oIndent.m_oLeft;
	}
	
	bool CIndent::operator!=(const CIndent &oIndent) const
	{
		return m_oTop    != oIndent.m_oTop    ||
		       m_oRight  != oIndent.m_oRight  || 
		       m_oBottom != oIndent.m_oBottom ||
		       m_oLeft   != oIndent.m_oLeft;
	}

	bool CIndent::SetValues(const std::wstring &wsTopValue, const std::wstring &wsRightValue, const std::wstring &wsBottomValue, const std::wstring &wsLeftValue, unsigned int unLevel, bool bHardMode)
	{
		const bool bTopResult    = SetTop    (wsTopValue,    unLevel, bHardMode);
		const bool bRightResult  = SetRight  (wsRightValue,  unLevel, bHardMode);
		const bool bBottomResult = SetBottom (wsBottomValue, unLevel, bHardMode);
		const bool bLeftResult   = SetLeft   (wsLeftValue,   unLevel, bHardMode);
		
		return bTopResult || bRightResult || bBottomResult || bLeftResult;
	}
	
	void CIndent::UpdateSide(CDigit &oSide, const double& dParentFontSize, const double& dCoreFontSize)
	{
		if (oSide.Empty())
			return;

		if (NSCSS::Em == oSide.GetUnitMeasure())
			oSide.ConvertTo(NSCSS::Twips, dParentFontSize);
		else if (NSCSS::Rem == oSide.GetUnitMeasure())
			oSide.ConvertTo(NSCSS::Twips, dCoreFontSize);
	}

	// FONT
	CTextDecorationLine::CTextDecorationLine()
	    : m_bUnderline(false), m_bOverline(false), m_bLineThrough(false)
	{}

	bool CTextDecorationLine::Empty() const
	{
		return false == m_bUnderline && false == m_bOverline && false == m_bLineThrough;
	}

	bool CTextDecorationLine::SetValue(const std::wstring &wsValue)
	{
		if (L"underline" == wsValue)
		{
			m_bUnderline = true;
			return true;
		}
		else if (L"overline" == wsValue)
		{
			m_bOverline = true;
			return true;
		}
		else if (L"line-through" == wsValue)
		{
			m_bLineThrough = true;
			return true;
		}
		else if (L"none" == wsValue)
		{
			m_bUnderline = m_bOverline = m_bLineThrough = false;
			return true;
		}

		return false;
	}

	bool CTextDecorationLine::Underline() const
	{
		return m_bUnderline;
	}

	bool CTextDecorationLine::Overline() const
	{
		return m_bOverline;
	}

	bool CTextDecorationLine::LineThrough() const
	{
		return m_bLineThrough;
	}

	CTextDecorationLine &CTextDecorationLine::operator+=(const CTextDecorationLine &oTextDecoration)
	{
		if (oTextDecoration.m_bUnderline)
			m_bUnderline = true;
		if (oTextDecoration.m_bOverline)
			m_bOverline = true;
		if (oTextDecoration.m_bLineThrough)
			m_bLineThrough = true;

		return *this;
	}

	bool CTextDecorationLine::operator==(const CTextDecorationLine &oTextDecorationLine) const
	{
		return m_bUnderline   == oTextDecorationLine.m_bUnderline &&
		       m_bOverline    == oTextDecorationLine.m_bOverline  &&
		       m_bLineThrough == oTextDecorationLine.m_bLineThrough;
	}

	TTextDecoration &TTextDecoration::operator+=(const TTextDecoration &oTextDecoration)
	{
		m_oLine  += oTextDecoration.m_oLine;
		m_oStyle += oTextDecoration.m_oStyle;
		m_oColor += oTextDecoration.m_oColor;

		return *this;
	}

	bool TTextDecoration::operator==(const TTextDecoration &oTextDecoration) const
	{
		return m_oLine  == oTextDecoration.m_oLine  &&
		       m_oStyle == oTextDecoration.m_oStyle &&
		       m_oColor == oTextDecoration.m_oColor;
	}

	CBaselineShift::CBaselineShift()
	{
		m_eType.SetMapping({{L"baseline", EBaselineShift::Baseline}, {L"sub", EBaselineShift::Sub}, {L"super", EBaselineShift::Super}}, EBaselineShift::Baseline);
	}

	bool CBaselineShift::Empty() const
	{
		return m_eType.Empty() && m_oValue.Empty();
	}

	EBaselineShift CBaselineShift::GetType() const
	{
		if (m_oValue.Empty())
			return (EBaselineShift)m_eType.ToInt();

		if (UnitMeasure::Percent == m_oValue.GetUnitMeasure())
			return EBaselineShift::Percentage;

		return EBaselineShift::Length;
	}

	double CBaselineShift::GetValue() const
	{
		if (UnitMeasure::Percent == m_oValue.GetUnitMeasure())
			return m_oValue.ToDouble();

		return m_oValue.ToDouble(NSCSS::Pixel);
	}

	bool CBaselineShift::SetValue(const std::wstring& wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_eType.SetValue(wsValue, unLevel, bHardMode) || m_oValue.SetValue(wsValue, unLevel, bHardMode);
	}

	CFont::CFont()
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
		const std::vector<std::wstring> arValues = NSCSS::NS_STATIC_FUNCTIONS::GetWordsW(wsValue, false, L" ,/");

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

			if (ushPosition < 4 && m_oWeight.Empty() && (SetWeight(wsWord, unLevel, bHardMode) ||
			                                             L"normal" == wsWord || L"bold" == wsWord || L"bolder" == wsWord || L"lighter " == wsWord))
			{
				++ushPosition;
				continue;
			}

			if (ushPosition < 4 && m_oStretch.Empty() && SetStretch(wsWord, unLevel, bHardMode))
			{
				++ushPosition;
				continue;
			}

			if (ushPosition < 5 && SetSize(wsWord, unLevel, bHardMode))
			{
				ushPosition = 5;
				continue;
			}

			if (ushPosition == 5 && SetLineHeight(wsWord, unLevel, bHardMode))
			{
				++ushPosition;
				continue;
			}

			if (ushPosition >= 5 && SetFamily(wsWord, unLevel, bHardMode))
				continue;
		}

		if (ushPosition >= 5)
			return true;
		else
			Clear();

		return true;
	}

	bool CFont::SetSize(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		const std::vector<std::pair<std::wstring, std::wstring>> arAbsoluteFontValues =
		    {{L"xx-small", L"7.5pt"}, {L"xx-large", L"36pt" },
		     {L"x-small",  L"10pt" }, {L"x-large",  L"24pt"  },
		     {L"small",    L"12pt" }, {L"medium",   L"13.5pt"},
		     {L"large",    L"18pt" }};

		size_t unFoundPos = std::wstring::npos;
		std::wstring wsNewValue(wsValue);

		for (const std::pair<std::wstring, std::wstring>& oAbsValue : arAbsoluteFontValues)
		{
			unFoundPos = wsNewValue.find(oAbsValue.first);
			if (std::wstring::npos != unFoundPos)
				wsNewValue.replace(unFoundPos, oAbsValue.first.length(), oAbsValue.second);
		}

		return m_oSize.SetValue(wsNewValue, unLevel, bHardMode);
	}

	bool CFont:: SetSize(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oSize.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CFont::SetLineHeight(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oLineHeight.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CFont::SetFamily(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		// std::wstring wsNewFamily(wsValue);

//		if (wsNewFamily.end() == wsNewFamily.erase(std::remove(wsNewFamily.begin(), wsNewFamily.end(), L'\''), wsNewFamily.end()) &&
//		    wsNewFamily.end() == wsNewFamily.erase(std::remove(wsNewFamily.begin(), wsNewFamily.end(), L'"'),  wsNewFamily.end()))
//			return false;

		std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(wsValue, false, L"\"\',");

		for (std::vector<std::wstring>::iterator iWord = arWords.begin(); iWord != arWords.end(); ++iWord)
		{
			if ((*iWord).empty())
				continue;

			NSCSS::NS_STATIC_FUNCTIONS::RemoveSpaces(*iWord);

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

	void CFont::UpdateSize(const double& dParentFontSize, const double& dCoreFontSize)
	{
		if (NSCSS::Em == m_oSize.GetUnitMeasure() || NSCSS::Percent == m_oSize.GetUnitMeasure())
			m_oSize.ConvertTo(NSCSS::Point, dParentFontSize);
		else if (NSCSS::Rem == m_oSize.GetUnitMeasure())
			m_oSize.ConvertTo(NSCSS::Point, dCoreFontSize);
	}
	
	void CFont::UpdateLineHeight(const double& dParentFontSize, const double& dCoreFontSize)
	{
		if (NSCSS::Em == m_oLineHeight.GetUnitMeasure())
			m_oLineHeight.ConvertTo(NSCSS::Twips, dParentFontSize);
		else if (NSCSS::Rem == m_oLineHeight.GetUnitMeasure())
			m_oLineHeight.ConvertTo(NSCSS::Twips, dCoreFontSize);
	}

	bool CFont::Bold() const
	{
		return m_oWeight == L"bold";
	}

	bool CFont::Italic() const
	{
		return m_oStyle == L"italic";
	}

	void CFont::Clear()
	{
		m_oSize      .Clear();
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

	CDigit &CFont::GetLineHeight()
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
		m_oSize.SetValue(oFont.m_oSize);
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

	CURL::CURL() 
	{}

	bool CURL::Empty() const
	{
		return m_wsValue.empty();
	}

	bool CURL::LinkToId() const
	{
		return m_wsValue.length() > 1 && L'#' == m_wsValue.front();
	}
	
	void CURL::Clear()
	{
		m_wsValue.clear();
	}

	bool CURL::SetValue(const std::wstring &wsValue)
	{
		if (wsValue.empty())
			return false;

		std::wregex oRegex(L"url\\s*\\(\\s*(?:'|\"|)([#]?[^'\"()]+)(?:'|\"|)\\s*\\)");
		std::wsmatch oMatch;

		if (!std::regex_search(wsValue.cbegin(), wsValue.cend(), oMatch, oRegex) || oMatch[1].str().empty())
			return false;

		m_wsValue = oMatch[1].str();
		NS_STATIC_FUNCTIONS::RemoveSpaces(m_wsValue);

		return true;
	}
	
	std::wstring CURL::GetValue() const
	{
		return m_wsValue;
	}

	bool CURL::operator==(const CURL& oValue) const
	{
		return m_wsValue == oValue.m_wsValue;
	}

	bool CURL::operator!=(const CURL& oValue) const
	{
		return m_wsValue != oValue.m_wsValue;
	}

	CEnum::CEnum()
		: m_nDefaultValue(0)
	{}

	bool CEnum::SetValue(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty() || m_mMap.empty() || (CHECK_CONDITIONS && !bHardMode))
			return false;

		std::wstring wsNewValue = wsValue;

		bool bImportant = CutImportant(wsNewValue);

		if (m_bImportant && !bImportant)
			return false;

		std::map<std::wstring, int>::const_iterator oFound = m_mMap.find(wsNewValue);

		if (m_mMap.end() == oFound)
			return false;

		m_oValue     = oFound->second;
		m_unLevel    = unLevel;
		m_bImportant = bImportant;

		return true;
	}

	void CEnum::SetMapping(const std::map<std::wstring, int> &mMap, int nDefaulvalue)
	{
		m_mMap = mMap;

		if (-1 != nDefaulvalue)
			m_nDefaultValue = nDefaulvalue;
		else if (!mMap.empty())
			m_nDefaultValue = mMap.begin()->second;
	}

	int CEnum::ToInt() const
	{
		if (Empty())
			return m_nDefaultValue;

		return m_oValue.value();
	}

	double CEnum::ToDouble() const
	{
		if (Empty())
			return m_nDefaultValue;

		return (double)m_oValue.value();
	}

	std::wstring CEnum::ToWString() const
	{
		if (m_mMap.empty())
			return std::wstring();

		if (Empty())
			return std::find_if(m_mMap.begin(), m_mMap.end(), [this](const std::pair<std::wstring, int>& oValue){ return m_nDefaultValue == oValue.second; })->first;

		return std::find_if(m_mMap.begin(), m_mMap.end(), [this](const std::pair<std::wstring, int>& oValue){ return m_oValue == oValue.second; })->first;
	}

	CPage::CPage()
	{}

	bool CPage::SetMargin(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oMargin.SetValues(wsValue, unLevel, bHardMode);
	}

	bool CPage::SetSize(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		if (wsValue.empty())
			return false;

		std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(wsValue);

		if (1 == arValues.size())
			return m_oWidth.SetValue(arValues[0], unLevel, bHardMode) && m_oHeight.SetValue(arValues[0], unLevel, bHardMode);
		else if (2 == arValues.size())
			return m_oWidth.SetValue(arValues[0], unLevel, bHardMode) && m_oHeight.SetValue(arValues[1], unLevel, bHardMode);
	
		return false;
	}

	bool CPage::SetFooter(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oFooter.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CPage::SetHeader(const std::wstring &wsValue, unsigned int unLevel, bool bHardMode)
	{
		return m_oHeader.SetValue(wsValue, unLevel, bHardMode);
	}

	bool CPage::SetWidth(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oWidth.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CPage::SetHeight(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oHeight.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CPage::SetMargin(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oMargin.SetValues(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CPage::SetFooter(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oFooter.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	bool CPage::SetHeader(const double& dValue, UnitMeasure enUnitMeasure, unsigned int unLevel, bool bHardMode)
	{
		return m_oHeader.SetValue(dValue, enUnitMeasure, unLevel, bHardMode);
	}

	const CDigit &CPage::GetWidth() const
	{
		return m_oWidth;
	}

	const CDigit &CPage::GetHeight() const
	{
		return m_oHeight;
	}

	const CIndent &CPage::GetMargin() const
	{
		return m_oMargin;
	}

	const CDigit &CPage::GetFooter() const
	{
		return m_oFooter;
	}

	const CDigit &CPage::GetHeader() const
	{
		return m_oHeader;
	}
	}
}
