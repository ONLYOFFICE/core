#ifndef CUSE_H
#define CUSE_H

#include "CContainer.h"
#include "CObjectBase.h"

namespace SVG
{
	class CUse : public CSvgGraphicsObject
	{
	public:
		CUse(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL, const CSvgFile* pFile = NULL);
		virtual ~CUse();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false, const TSvgStyles* pOtherStyles = NULL) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		CSvgGraphicsObject *m_pUsedObject;

		SvgDigit m_oX;
		SvgDigit m_oY;
		SvgDigit m_oWidth;
		SvgDigit m_oHeight;

		std::wstring    m_wsHref;
		const CSvgFile *m_pFile;
	};
}
#endif // CUSE_H
