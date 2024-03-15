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
		std::wstring m_sId;

	public:
		CStyleUsed(const CCompiledStyle& oStyle, bool bIsPStyle);

		bool operator==(const CStyleUsed& oUsedStyle) const;

		std::wstring getId();
		void setId(const std::wstring& sId);
	};

	class CSSCALCULATOR_EXPORT CDocumentStyle
	{
		typedef NSConstValues::NSProperties::BasicProperties     BProperties;
		typedef NSConstValues::NSProperties::ParagraphProperties PProperties;
		typedef NSConstValues::NSProperties::RunnerProperties    RProperties;
	
		std::list<std::wstring> m_arStandardStylesUsed;
		std::list<std::wstring> m_arStandardStyles;
		std::list<CStyleUsed>   m_arStyleUsed;

		std::wstring m_sStyle;
		std::wstring m_sId;

		void CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles, CXmlElement& oElement);
		void CreateStandardStyle  (const std::wstring& sNameStyle, CXmlElement& oElement);
		void ConvertStyle         (const NSCSS::CCompiledStyle& oStyle, CXmlElement& oElement, bool bIsPStyle);

		void SetRStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement);
		void SetPStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement);

		void SetAllBorderStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement);
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

		static std::wstring CalculateBorderStyle(const NSCSS::NSProperties::CBorderSide& oBorder);
	};
}
#endif // CDOCUMENTSTYLE_H
