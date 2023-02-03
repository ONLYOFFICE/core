#ifndef CSTYLE_H
#define CSTYLE_H

#include <string>
#include "../SvgUtils.h"
#include "../../../../../Common/3dParty/html/css/src/CCssCalculator.h"

namespace SVG
{
	class CObjectBase;

	class CStyle : public NSCSS::CCompiledStyle
	{
	public:
		CStyle();
		~CStyle();

		virtual void AddOtherStyle(const std::pair<std::wstring, std::wstring>& oStyle, const unsigned int unLevel, const bool& bHardMode = true);

		NSCSS::NSProperties::CStroke    m_oStroke;
		NSCSS::NSProperties::CTransform m_oTransform;
	};

	class CGeneralStyle
	{
	public:
		CGeneralStyle();
		~CGeneralStyle();

		void AddStyle(const std::wstring& wsStyle);
		CStyle GetStyle(const std::vector<NSCSS::CNode>& arNodes) const;

		void SetStyle(CObjectBase& oObject);

	private:
		NSCSS::CCssCalculator m_oCssCalculator;
	};
}

#endif // CSTYLE_H
