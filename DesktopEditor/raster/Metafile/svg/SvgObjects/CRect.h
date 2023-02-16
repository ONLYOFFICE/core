#ifndef CRECT_H
#define CRECT_H

#include "CObjectBase.h"

namespace SVG
{
	class CRect : public CObjectBase
	{
	public:
		CRect(CObjectBase* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer, CDefs *pDefs) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		TRect    m_oRect;

		SvgDigit m_oRx;
		SvgDigit m_oRy;
	};
}

#endif // CRECT_H
