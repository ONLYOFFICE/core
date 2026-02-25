#ifndef CDOCUMENTSTYLE_H
#define CDOCUMENTSTYLE_H

#include "../CssCalculator_global.h"
#include "../CCompiledStyle.h"
#include "CXmlElement.h"
#include <list>

namespace NSCSS
{
	class CStyleUsed
	{
		CCompiledStyle m_oStyle;
		bool m_bIsPStyle;
		std::wstring m_wsFinalId;

	public:
		CStyleUsed(const CCompiledStyle& oStyle, bool bIsPStyle);

		void SetFinalId(const std::wstring& wsFinalId);

		bool operator==(const CStyleUsed& oUsedStyle) const;

		std::wstring GetId() const;
	};

	static const std::vector<std::wstring> Names_Standard_Styles = {L"a", L"a-c", L"li", L"h1", L"h2", L"h3", L"h4", L"h5", L"h6", L"h1-c", L"h2-c", L"h3-c", L"h4-c", L"h5-c", L"h6-c"};

	class CSSCALCULATOR_EXPORT CDocumentStyle
	{
		typedef NSConstValues::NSProperties::BasicProperties     BProperties;
		typedef NSConstValues::NSProperties::ParagraphProperties PProperties;
		typedef NSConstValues::NSProperties::RunnerProperties    RProperties;
	
		std::vector<std::wstring> m_arStandardStylesUsed;
		std::vector<std::wstring> m_arStandardStyles;
		std::vector<CStyleUsed>   m_arStyleUsed;

		std::wstring m_sStyle;
		std::wstring m_sId;

		bool CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles, CXmlElement& oElement);
		bool CreateStandardStyle  (const std::wstring& sNameStyle, CXmlElement& oElement);
		bool ConvertStyle         (const NSCSS::CCompiledStyle& oStyle, CXmlElement& oElement, bool bIsPStyle);

		bool SetRStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement, bool bIsLite = false);
		bool SetPStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement, bool bIsLite = false);

		void SetBorderStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement, const PProperties& enBorderProperty);
	public:
		CDocumentStyle();
		~CDocumentStyle();

		bool WritePStyle(const NSCSS::CCompiledStyle& oStyle);
		bool WriteRStyle(const NSCSS::CCompiledStyle& oStyle);
		bool WriteLitePStyle(const NSCSS::CCompiledStyle& oStyle);
		bool WriteLiteRStyle(const NSCSS::CCompiledStyle& oStyle);

		void SetStyle(const std::wstring& sStyle);
		void SetId   (const std::wstring& sId);

		std::wstring GetStyle() const;
		std::wstring GetIdAndClear();

		void Clear();

		static std::wstring CalculateBorderStyle(const NSCSS::NSProperties::CBorderSide& oBorder, const NSCSS::NSProperties::CDigit* pPadding = NULL);
	};
}
#endif // CDOCUMENTSTYLE_H
