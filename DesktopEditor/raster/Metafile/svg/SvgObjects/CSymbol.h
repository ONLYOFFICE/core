#ifndef CSYMBOL_H
#define CSYMBOL_H

#include "CContainer.h"

namespace SVG
{
	class CSymbol : public CGraphicsContainer
	{
		friend class CRenderedObject;
		CSymbol(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	};
}

#endif // CSYMBOL_H
