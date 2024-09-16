#ifndef CSWITCH_H
#define CSWITCH_H

#include "CContainer.h"

namespace SVG
{
	class CSwitch : public CRenderedObject, public CContainer<CRenderedObject>
	{
	public:
		CSwitch(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL);

		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

		TBounds GetBounds() const override;
	};
}
#endif // CSWITCH_H
