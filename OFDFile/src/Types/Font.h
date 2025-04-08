#ifndef FONT_H
#define FONT_H

#include "../IOFDElement.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"

namespace OFD
{
class CFont : public IOFDElement
{
public:
	CFont(CXmlReader& oXmlReader, const std::wstring& wsRootPath);

	void Apply(IRenderer* pRenderer, NSFonts::IFontManager* pFontManager, const double& dFontSize) const;
private:
	std::wstring m_wsFontName;
	std::wstring m_wsFamilyName;
	std::wstring m_wsCharset;
	bool         m_bItalic;
	bool         m_bBold;
	bool         m_bSerif;
	bool         m_bFixedWidth;

	std::wstring m_wsFilePath;
};
}

#endif // FONT_H
