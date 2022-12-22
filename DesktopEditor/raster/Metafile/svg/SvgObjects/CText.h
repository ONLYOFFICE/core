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
		CText(CObjectBase* pParent = NULL, CGeneralStyle* pBaseStyle = NULL, NSFonts::IFontManager* pFontManager = NULL);
		virtual ~CText();

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) override;
	private:
		void ApplyStyle(IRenderer* pRenderer) override;

		double GetWidth() const;

		NSFonts::IFontManager* m_pFontManager;

		Point        m_oCoord;
		std::wstring m_wsText;

		std::vector<CTspan*> m_arChildrens;

		friend class CTspan;
	};

	class CTspan : public CObjectBase
	{
	public:
		CTspan(CObjectBase* pParent);
		virtual ~CTspan();

		double GetWidth() const;

		bool ReadFromXmlNode(XmlUtils::CXmlNode& oNode) override;
		bool Draw(IRenderer* pRenderer) override;
	private:
		void ApplyStyle(IRenderer* pRenderer) override;

		void InheritData(const CText& oText);

		NSFonts::IFontManager* m_pFontManager;

		Point        m_oCoord;
		std::wstring m_wsText;
	};
}

#endif // CTEXT_H
