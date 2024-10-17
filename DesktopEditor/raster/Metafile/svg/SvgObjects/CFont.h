#ifndef CFONT_H
#define CFONT_H

#include "CPath.h"

namespace SVG
{
	class CGlyph : public CPath
	{
	public:
		CGlyph(XmlUtils::CXmlNode& oNode);

		wchar_t GetUnicode() const;
	private:
		wchar_t  m_wchUnicode;
		SvgDigit m_oHorizAdvX;

		friend class CFont;
	};

	class CFontFace
	{
	public:
		CFontFace(XmlUtils::CXmlNode& oNode);
	private:
		std::wstring m_wsSrcFaceName;
	};

	struct TFontArguments
	{
		std::wstring m_wsFontFamily;
		std::wstring m_wsFontVariant;
		std::wstring m_wsFontStyle;
		std::wstring m_wsFontWidght;
	};

	class CFont : public CAppliedObject
	{
	public:
		CFont(XmlUtils::CXmlNode& oNode);
		~CFont();

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) override;
		bool Draw(const std::wstring& wsText, const double& dX, const double& dY, const double& dFontHeight, IRenderer* pRenderer, const CSvgFile *pFile, CommandeMode oMode = CommandeModeDraw, const TSvgStyles* pStyles = NULL, const CRenderedObject* pContexObject = NULL) const;
	private:
		void ParseGlyphs(XmlUtils::CXmlNode& oNode);

		TFontArguments m_oArguments;

		typedef std::map<wchar_t, CGlyph*> MGlyphsMap;

		MGlyphsMap m_mGlyphs;
		CPath *m_pMissingGlyph;

		SvgDigit m_oHorizAdvX;
	};
}

#endif // CFONT_H
