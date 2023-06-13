#ifndef CCIRCLE_H
#define CCIRCLE_H

#include "CObjectBase.h"

namespace SVG
{
	class CCircle : public CSvgGraphicsObject
	{
	public:
		CCircle(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;

		TBounds GetBounds() const override;

		SvgDigit m_oCx;
		SvgDigit m_oCy;
		SvgDigit m_oR;
	};
}

#endif // CCIRCLE_H
