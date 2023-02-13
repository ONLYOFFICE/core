#ifndef CPATTERN_H
#define CPATTERN_H

#include "CContainer.h"

namespace SVG
{
	class CPattern : public CContainer
	{
	public:
		CPattern(CObjectBase* pParent = NULL, NSFonts::IFontManager *pFontManager = NULL);

		bool DrawDef(IRenderer* pRenderer, const TRect& oParentRect, CDefs *pDefs) const;
	private:
		NSFonts::IFontManager *m_pFontManager;
	};
}

#endif // CPATTERN_H
