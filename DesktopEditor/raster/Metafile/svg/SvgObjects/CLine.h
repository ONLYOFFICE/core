#ifndef CLINE_H
#define CLINE_H

#include "CObjectBase.h"

namespace SVG
{
	class CLine : public CSvgGraphicsObject
	{
	public:
		CLine(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false, const TSvgStyles* pOtherStyles = NULL) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const;

		TBounds GetBounds() const override;

		SvgDigit m_oX1;
		SvgDigit m_oY1;
		SvgDigit m_oX2;
		SvgDigit m_oY2;
	};
}

#endif // CLINE_H
