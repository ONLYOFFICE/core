#ifndef CRECT_H
#define CRECT_H

#include "CObjectBase.h"

namespace SVG
{
	class CRect : public CSvgGraphicsObject
	{
	public:
		CRect(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const override;

	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		TRect    m_oRect;

		SvgDigit m_oRx;
		SvgDigit m_oRy;
	};
}

#endif // CRECT_H
