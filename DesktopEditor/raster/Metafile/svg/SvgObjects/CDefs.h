#ifndef CDEFS_H
#define CDEFS_H

#include "CContainer.h"

namespace SVG
{
	class IDefObject
	{
	public:
		virtual bool Apply(IRenderer* pRenderer, CDefs* pDefs, const double dParentWidth, const double dParentHeight) = 0;
	private:
		virtual void Update(CDefs* pDefs, const double dParentWidth, const double dParentHeight) = 0;
	};

	class CDefs : public CContainer
	{
	public:
		CDefs(CObjectBase* pParent = NULL);
		
		IDefObject* GetDef(const std::wstring& wsId) const;
	};
}

#endif // CDEFS_H
