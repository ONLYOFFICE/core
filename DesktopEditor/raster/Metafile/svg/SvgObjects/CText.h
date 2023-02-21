#ifndef CTEXT_H
#define CTEXT_H

#include "../../../graphics/pro/Fonts.h"

#include "CObjectBase.h"
#include "../SvgTypes.h"

namespace SVG
{
	class CText : public CSvgGraphicsObject
	{
	public:
		CText(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
		virtual ~CText();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CDefs *pDefs, bool bIsClip = false) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const override;
		void ApplyFont(IRenderer* pRenderer, double& dX, double& dY) const;

		TBounds GetBounds() const override;

		double GetWidth() const;

		void Normalize();

		NSFonts::IFontManager* m_pFontManager;

		SvgDigit     m_oX;
		SvgDigit     m_oY;

		std::wstring m_wsText;

		SvgFont m_oFont;
		SvgText m_oText;
	};
}

#endif // CTEXT_H
