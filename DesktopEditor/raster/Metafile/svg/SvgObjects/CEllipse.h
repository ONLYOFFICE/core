#ifndef CELLIPSE_H
#define CELLIPSE_H


#include "CObjectBase.h"

namespace SVG
{
	class CEllipse : public CSvgGraphicsObject
	{
	public:
		CEllipse(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		SvgDigit m_oCx;
		SvgDigit m_oCy;
		SvgDigit m_oRx;
		SvgDigit m_oRy;
	};
}

#endif // CELLIPSE_H
