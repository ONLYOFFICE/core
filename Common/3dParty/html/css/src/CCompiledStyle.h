#ifndef CCOMPILEDSTYLE_H
#define CCOMPILEDSTYLE_H

#include <map>
#include <set>
#include <vector>
#include <string>

#include "StyleProperties.h"

namespace NSCSS
{
	class CCompiledStyle
	{
		friend class CStyle;
		std::set<std::wstring> m_arParentsStyles;
		std::wstring m_sId;

		unsigned short int m_nDpi;
		UnitMeasure m_UnitMeasure;

		double m_dCoreFontSize;
	public:
		NSProperties::CFont       m_oFont;
		NSProperties::CIndent     m_oMargin;
		NSProperties::CIndent     m_oPadding;
		NSProperties::CBackground m_oBackground;
		NSProperties::CText       m_oText;
		NSProperties::CBorder     m_oBorder;
		NSProperties::CDisplay    m_oDisplay;
		NSProperties::CTransform  m_oTransform;

		CCompiledStyle();
		CCompiledStyle(const CCompiledStyle& oStyle);

		virtual ~CCompiledStyle();

		void SetDpi(const unsigned short& uiDpi);
		void SetUnitMeasure(const UnitMeasure& enUnitMeasure);

		bool Empty() const;

		void AddPropSel(const std::wstring& sProperty, const std::wstring& sValue, const unsigned int unLevel, const bool& bHardMode = true);
		void AddStyle(const std::map<std::wstring, std::wstring>& mStyle, const unsigned int unLevel, const bool& bHardMode = true);
		void AddStyle(const std::wstring& sStyle, const unsigned int unLevel, const bool& bHardMode = false);
		virtual void AddOtherStyle(const std::pair<std::wstring, std::wstring>& oStyle, const unsigned int unLevel, const bool& bHardMode = true) {};
		void AddParent(const std::wstring& sParentName);

		std::vector<std::wstring> GetParentsName() const;
		std::set<std::wstring> GetParentsNamesSet() const;

		void SetID(const std::wstring& sId);
		std::wstring GetId() const;

		bool HaveThisParent(const std::wstring& wsParentName) const;

		CCompiledStyle& operator+= (const CCompiledStyle& oElement);
		CCompiledStyle& operator=  (const CCompiledStyle& oElement);
		bool operator==            (const CCompiledStyle& oElement) const;

		static void StyleEquation(CCompiledStyle& oFirstStyle, CCompiledStyle& oSecondStyle);
	};
}

#endif // CCOMPILEDSTYLE_H
