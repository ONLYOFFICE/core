#ifndef CCLIPPATH_H
#define CCLIPPATH_H

#include "CDefs.h"

namespace SVG
{
	class CClipPath : public CGraphicsContainer, public CDefObject
	{
	public:
		CClipPath(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL, NSFonts::IFontManager *pFontManager = NULL);

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		bool Apply(IRenderer* pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds) override;
	};
}

#endif // CCLIPPATH_H
