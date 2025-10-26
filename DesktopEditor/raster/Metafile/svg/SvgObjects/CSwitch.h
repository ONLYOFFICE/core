#ifndef CSWITCH_H
#define CSWITCH_H

#include "CContainer.h"

namespace SVG
{
	class CSwitch : public CRenderedObject, public CContainer<CRenderedObject>
	{
		friend class CObject;
		CSwitch(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

		TBounds GetBounds(SvgMatrix* pTransform = nullptr) const override;
	};
}
#endif // CSWITCH_H
