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
		friend class CObject;
	protected:
		CTSpan(CSvgReader& oReader, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL, const Point& oPosition = {});
		CTSpan(const CTSpan& oTSpan, double dX, const std::wstring& wsText);
		CTSpan(wchar_t wChar, const Point& oPosition, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
	public:
		virtual ~CTSpan();

		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;
		void SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode = false) override;

		void ReadChildrens(CSvgReader& oReader, CSvgFile* pSvgFile) override;

		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;

		bool AddObject(CTSpan* pObject) override;

		void InheritStyles(const CTSpan* pTSpan);
	private:
		void ApplyStyle(IRenderer* pRenderer, const TSvgStyles* pStyles, const CSvgFile* pFile, int& nTypePath, const CRenderedObject* pContexObject = NULL) const override;
		void ApplyFont(IRenderer* pRenderer, double& dX, double& dY, Aggplus::CMatrix& oOldMatrix) const;

		void UpdateFontSize();
		bool UseExternalFont(const CSvgFile* pFile, double dX, double dY, IRenderer* pRenderer, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const;

		TBounds GetBounds(SvgMatrix* pTransform = nullptr) const override;

		double GetWidth() const;
		void CorrectFontFamily(std::wstring& wsFontFamily) const;

		void CalculatePosition(double& dX, double& dY) const;

		void NormalizeFontSize(double& dFontHeight, double& dScaleX, double& dScaleY) const;
		void SetPosition(const Point& oPosition);

		void SetPositionFromParent(CRenderedObject* pParent);

		double GetFontHeight() const;

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
		friend class CObject;
	protected:
		CText(CSvgReader& oReader, CRenderedObject* pParent = NULL, NSFonts::IFontManager* pFontManager = NULL);
	public:
		bool Draw(IRenderer* pRenderer, const CSvgFile* pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pOtherStyles = NULL, const CRenderedObject* pContexObject = NULL) const override;
	};

	class CTextPath : public CText
	{
		friend class CObject;
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
