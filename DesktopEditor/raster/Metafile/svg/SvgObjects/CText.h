#ifndef CTEXT_H
#define CTEXT_H

#include "../../../graphics/pro/Fonts.h"

#include "CObjectBase.h"
#include "../SvgTypes.h"

namespace SVG
{
	class CTspan;

	class CText : public CObjectBase
	{
	public:
		CText(CObjectBase* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
		virtual ~CText();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath) const override;

		void ApplyFont(IRenderer* pRenderer) const;

		double GetWidth() const;

		NSFonts::IFontManager* m_pFontManager;

		SvgDigit     m_oX;
		SvgDigit     m_oY;

		std::wstring m_wsText;

		std::vector<CTspan*> m_arChildrens;

		SvgFont m_oFont;

		friend class CTspan;
	};

	class CTspan : public CObjectBase
	{
	public:
		CTspan(CObjectBase* pParent);
		virtual ~CTspan();

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		double GetWidth() const;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) const override;
	private:
		void ApplyStyle(IRenderer* pRenderer, int& nTypePath) const override;

		void InheritData(const CText& oText);

		NSFonts::IFontManager* m_pFontManager;

		Point        m_oCoord;
		std::wstring m_wsText;
	};
}

#endif // CTEXT_H
