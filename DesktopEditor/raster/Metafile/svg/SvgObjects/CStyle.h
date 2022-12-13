#ifndef CSTYLE_H
#define CSTYLE_H

#include <string>
#include "../SvgUtils.h"
#include "../../../../../Common/3dParty/html/css/src/CCssCalculator.h"

namespace SVG
{
	static ColorParser m_oColorParser;

	class CStyle
	{
	public:
		CStyle();
		~CStyle();

		void SetStyle(const std::vector<NSCSS::CNode>& arXmlNodes);

		void SetStyle(const std::wstring& wsStyle);

		void SetStrokeColor(const std::wstring& wsStrokeColor);
		long GetStrokeColor() const;

		long GetFillL() const;

	private:
		NSCSS::CCssCalculator m_oCssCalculator;
		long m_lStrokeColor;
		long m_lFillColor;
	};
}

#endif // CSTYLE_H
