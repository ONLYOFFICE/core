#ifndef CDEFS_H
#define CDEFS_H

#include "CContainer.h"

namespace SVG
{
	class CDefObject : public CSvgObject<CSvgGraphicsObject>
	{
	public:
		CDefObject(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
		virtual ~CDefObject();

		virtual bool Apply(IRenderer* pRenderer, const CDefs* pDefs, const TBounds &oObjectBounds);
	};


	class CDefs : public CContainer<CDefObject>
	{
	public:
		CDefs();

		CDefObject* GetDef(const std::wstring& wsId) const;
	};
}

#endif // CDEFS_H
