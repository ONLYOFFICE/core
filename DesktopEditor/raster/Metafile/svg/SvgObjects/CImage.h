#ifndef CIMAGE_H
#define CIMAGE_H

#include "CObjectBase.h"

namespace SVG
{
	class CImage : public CSvgGraphicsObject
	{
	public:
		CImage(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		TRect        m_oRect;
		std::wstring m_wsHref;
	};
}

#endif // CIMAGE_H
