#ifndef CSYMBOL_H
#define CSYMBOL_H

#include "CContainer.h"
#include "CDefs.h"

namespace SVG
{
	class CSymbol : public CGraphicsContainer, public CDefObject
	{
	public:
		CSymbol(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL, NSFonts::IFontManager *pFontManager = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const override;

		bool Apply(IRenderer* pRenderer, const CDefs* pDefs, const TBounds &oObjectBounds) override;
	};
}

#endif // CSYMBOL_H
