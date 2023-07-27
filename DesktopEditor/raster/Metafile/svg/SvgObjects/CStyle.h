#ifndef CSTYLE_H
#define CSTYLE_H

#include <string>
#include "../../../../../Common/3dParty/html/css/src/CCssCalculator_Private.h"
#include "CObjectBase.h"

#include <numeric>

namespace SVG
{
	class CSvgCalculator
	{
		NSCSS::CCssCalculator_Private *m_pInternal;
	public:
		CSvgCalculator();
		~CSvgCalculator();

		void AddStyles(const std::wstring& wsStyles);

		void Clear();

		void SetData(CObject* pSvgObject) const;
	};
}

#endif // CSTYLE_H
