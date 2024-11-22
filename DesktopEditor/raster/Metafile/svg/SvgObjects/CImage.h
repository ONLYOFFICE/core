#ifndef CIMAGE_H
#define CIMAGE_H

#include "CObjectBase.h"

namespace SVG
{
	class CImage : public CRenderedObject
	{
	public:
		CImage(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL);

		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw,  const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	private:
		TBounds GetBounds() const override;

		TRect        m_oRect;
		std::wstring m_wsHref;
	};
}

#endif // CIMAGE_H
