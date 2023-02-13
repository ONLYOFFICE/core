#ifndef CIMAGE_H
#define CIMAGE_H

#include "CObjectBase.h"

namespace SVG
{
	class CImage : public CObjectBase
	{
	public:
		CImage(CObjectBase* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer, CDefs *pDefs) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TRect        m_oRect;
		std::wstring m_wsHref;
	};
}

#endif // CIMAGE_H
