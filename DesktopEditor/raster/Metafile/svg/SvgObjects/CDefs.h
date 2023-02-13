#ifndef CDEFS_H
#define CDEFS_H

#include "CContainer.h"

namespace SVG
{
	class CDefs : public CContainer
	{
	public:
		CDefs(CObjectBase* pParent = NULL);
		
		CObjectBase* GetDef(const std::wstring& wsId) const;
	};
}

#endif // CDEFS_H
