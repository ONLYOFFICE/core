#ifndef CIMAGE_H
#define CIMAGE_H

#include "CObjectBase.h"

namespace SVG
{
	BEGIN_RENDERER_CHILDREN_H(Image)
		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw,  const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	private:
		TBounds GetBounds() const override;

		TRect        m_oRect;
		std::wstring m_wsHref;
	END_RENDERER_CHILDREN_H
}

#endif // CIMAGE_H
