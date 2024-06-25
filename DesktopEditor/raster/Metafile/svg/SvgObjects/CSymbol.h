#ifndef CSYMBOL_H
#define CSYMBOL_H

#include "CContainer.h"
#include "../../../graphics/pro/Fonts.h"

namespace SVG
{
	class CSymbol : public CGraphicsContainer
	{
	public:
		CSymbol(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager *pFontManager = NULL);

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const override;
	};
}

#endif // CSYMBOL_H
