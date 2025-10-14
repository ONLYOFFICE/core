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
		friend class CRenderedObject;
	protected:
		CTSpan(CSvgReader& oReader, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, const Point& oPosition = {});
		CTSpan(const std::wstring& wsText, const Point& oPosition, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, bool bCheckText = true);
		CTSpan(const CTSpan& oTSpan, double dX, const std::wstring& wsText);
	public:
		virtual ~CTSpan();

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;
		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		void ReadChildrens(CSvgReader& oReader, const CSvgCalculator* pSvgCalculator) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

		bool AddObject(CTSpan* pObject) override;

		void InheritStyles(const CTSpan* pTSpan);
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile* pFile, int& nTypePath, const CRenderedObject* pContexObject = NULL) const override;
		void ApplyFont(IRenderer* pRenderer, double& dX, double& dY) const;

		bool UseExternalFont(const CSvgFile* pFile, double dX, double dY, IRenderer* pRenderer, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const;

		TBounds GetBounds() const override;

		void ReadText(CSvgReader& oReader, const CSvgCalculator* pSvgCalculator, bool bCheckText);

		double GetWidth() const;
		void CorrectFontFamily(std::wstring& wsFontFamily) const;

		void CalculatePosition(double& dX, double& dY) const;

		void Normalize(IRenderer* pRenderer, double& dX, double& dY, double& dFontHeight) const;
		void SetPosition(const Point& oPosition);

		void SetPositionFromParent(CRenderedObject* pParent);

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
		friend class CRenderedObject;
	protected:
		CText(CSvgReader& oReader, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
	public:
		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	};

	class CTextPath : public CText
	{
		friend class CRenderedObject;
		CTextPath(CSvgReader& oReader, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	private:
		void DrawGlyph(CTSpan* pTSpan, CMovingPath& oMovingPath, IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode, const CRenderedObject* pContexObject = NULL) const;

		std::wstring m_wsHref;
	};
}

#endif // CTEXT_H
