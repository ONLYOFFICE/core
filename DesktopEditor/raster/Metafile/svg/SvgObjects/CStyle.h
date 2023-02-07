#ifndef CSTYLE_H
#define CSTYLE_H

#include <string>
#include "../SvgUtils.h"
#include "../../../../../Common/3dParty/html/css/src/CCssCalculator_Private.h"
#include "CObjectBase.h"

namespace SVG
{
	class CSvgCalculator
	{
		NSCSS::CCssCalculator_Private *m_pInternal;
	public:
		CSvgCalculator();
		~CSvgCalculator();

		void AddStyles(const std::wstring& wsStyles);

		void SetData(CObjectBase& oSvgObject) const;
	};
}

#endif // CSTYLE_H
