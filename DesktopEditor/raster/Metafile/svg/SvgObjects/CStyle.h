#ifndef CSTYLE_H
#define CSTYLE_H

#include <string>
#include "../SvgUtils.h"
#include "../../../../../Common/3dParty/html/css/src/CCssCalculator.h"

namespace SVG
{
	static const ColorParser m_oColorParser;

	class CStyle;

	class CGeneralStyle
	{
	public:
		CGeneralStyle();
		~CGeneralStyle();

		void AddStyle(const std::wstring& wsStyle);
		CStyle GetStyle(const std::vector<NSCSS::CNode>& arNodes) const;

	private:
		NSCSS::CCssCalculator m_oCssCalculator;
	};

	class CStyle : public NSCSS::CCompiledStyle
	{
	public:
		CStyle();
		~CStyle();

		virtual void AddOtherStyle(const std::pair<std::wstring, std::wstring>& oStyle, const unsigned int unLevel, const bool& bHardMode = true);

		std::wstring GetFill() const;
	};
}

#endif // CSTYLE_H
