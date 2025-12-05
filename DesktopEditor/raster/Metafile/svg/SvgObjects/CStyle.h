#ifndef CSTYLE_H
#define CSTYLE_H

#include <string>
#include "../../../../../Common/3dParty/html/css/src/CCssCalculator_Private.h"

namespace SVG
{
	class CObject;
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
