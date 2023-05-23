#ifndef CMASK_H
#define CMASK_H

#include "CClipPath.h"

namespace SVG
{
	class CMask : public CClipPath
	{
	public:
		CMask(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL, NSFonts::IFontManager *pFontManager = NULL);

		bool Apply(IRenderer* pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds) override;
	};
}

#endif // CMASK_H
