#ifndef CIMAGE_H
#define CIMAGE_H

#include "CObjectBase.h"

namespace SVG
{
	class CImage : public CRenderedObject
	{
		friend class CObject;
		CImage(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw,  const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	private:
		TBounds GetBounds(SvgMatrix* pTransform = nullptr) const override;

		TRect        m_oRect;
		std::wstring m_wsHref;
	};
}

#endif // CIMAGE_H
