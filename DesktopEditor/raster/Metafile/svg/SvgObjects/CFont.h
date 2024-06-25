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
		wchar_t m_wchUnicode;
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
	private:
		void ParseGlyphs(XmlUtils::CXmlNode& oNode);

		TFontArguments m_oArguments;

		std::map<wchar_t, CGlyph*> m_mGlyphs;
		CPath *m_pMissingGlyph;
	};
}

#endif // CFONT_H
