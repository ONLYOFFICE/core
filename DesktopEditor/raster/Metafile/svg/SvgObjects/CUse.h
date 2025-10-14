#ifndef CUSE_H
#define CUSE_H

#include "CContainer.h"
#include "CObjectBase.h"

namespace SVG
{
	class CUse : public CRenderedObject
	{
		friend class CRenderedObject;
		CUse(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		virtual ~CUse();

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	private:
		TBounds GetBounds() const override;

		CRenderedObject *m_pUsedObject;

		SvgDigit m_oX;
		SvgDigit m_oY;
		SvgDigit m_oWidth;
		SvgDigit m_oHeight;

		std::wstring    m_wsHref;
	};
}
#endif // CUSE_H
