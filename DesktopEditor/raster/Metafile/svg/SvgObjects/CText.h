#ifndef CTEXT_H
#define CTEXT_H

#include "../../../graphics/pro/Fonts.h"

#include "CPath.h"
#include "CContainer.h"
#include "CObjectBase.h"
#include "../SvgTypes.h"

namespace SVG
{
	class CTSpan : public CRenderedObject, public CContainer<CTSpan>
	{
	public:
		CTSpan(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, bool bCheckText = true);
		CTSpan(const std::wstring& wsText, const Point& oPosition, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, bool bCheckText = true);
		virtual ~CTSpan();

		static CTSpan* Create(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
		static CTSpan* Create(const std::wstring& wsValue, const Point& oPosition, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, bool bCheckText = true);

		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

		bool AddObject(CTSpan* pObject) override;

		void InheritStyles(const CTSpan* pTSpan);
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile* pFile, int& nTypePath, const CRenderedObject* pContexObject = NULL) const override;
		void ApplyFont(IRenderer* pRenderer, double& dX, double& dY) const;

		bool UseExternalFont(const CSvgFile* pFile, double dX, double dY, IRenderer* pRenderer, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const;

		TBounds GetBounds() const override;

		double GetWidth() const;
		void CorrectFontFamily(std::wstring& wsFontFamily) const;

		void CalculatePosition(double& dX, double& dY) const;

		void Normalize(IRenderer* pRenderer, double& dX, double& dY, double& dFontHeight) const;
		void SetPosition(const Point& oPosition);

		std::vector<CTSpan> Split() const;

		NSFonts::IFontManager* m_pFontManager;

		SvgDigit     m_oX;
		SvgDigit     m_oY;

		std::wstring m_wsText;

		SvgFont m_oFont;
		SvgText m_oText;

		friend class CText;
		friend class CTextPath;
	};

	class CText : public CTSpan
	{
	public:
		CText(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);

		static CText* Create(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	};

	class CTextPath : public CText
	{
	public:
		CTextPath(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, const CSvgFile* pFile = NULL);

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

		static CTextPath* Create(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, const CSvgFile* pFile = NULL);
	private:
		void DrawGlyph(CTSpan* pTSpan, CMovingPath& oMovingPath, IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode, const CRenderedObject* pContexObject = NULL) const;

		const CPath        *m_pPath;
	};
}

#endif // CTEXT_H
