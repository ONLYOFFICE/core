#ifndef CDEFS_H
#define CDEFS_H

#include "CContainer.h"

namespace SVG
{
	class IDefObject
	{
	public:
		virtual bool Apply(IRenderer* pRenderer, CDefs* pDefs, const TBounds &oObjectBounds) = 0;
	};

	class CDefs
	{
	public:
		CDefs();
		
		CContainer& GetContainer();

		IDefObject* GetDef(const std::wstring& wsId) const;
	private:
		CContainer m_oContainer;
	};
}

#endif // CDEFS_H
