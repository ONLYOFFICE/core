#include "Font.h"

#include "../Utils/Utils.h"

namespace OFD
{
CFont::CFont(CXmlReader& oXmlReader, const std::wstring& wsRootPath)
	: IOFDElement(oXmlReader),
	  m_wsCharset(L"unicode"), m_bItalic(false), m_bBold(false),
	  m_bSerif(false), m_bFixedWidth(false)
{
	if (0 != oXmlReader.GetAttributesCount() && oXmlReader.MoveToFirstAttribute())
	{
		std::string sArgumentName;

		do
		{
			sArgumentName = oXmlReader.GetNameA();

			if ("FontName" == sArgumentName)
				m_wsFontName = oXmlReader.GetText();
			else if ("FamilyName" == sArgumentName)
				m_wsFamilyName = oXmlReader.GetText();
			else if ("Charset" == sArgumentName)
				m_wsCharset = oXmlReader.GetText();
			else if ("Italic" == sArgumentName)
				m_bItalic = oXmlReader.GetBoolean(true);
			else if ("Bold" == sArgumentName)
				m_bBold = oXmlReader.GetBoolean(true);
			else if ("Serif" == sArgumentName)
				m_bSerif = oXmlReader.GetBoolean(true);
			else if ("FixedWidth" == sArgumentName)
				m_bFixedWidth = oXmlReader.GetBoolean(true);
		} while (oXmlReader.MoveToNextAttribute());
	}

	oXmlReader.MoveToElement();

	if (oXmlReader.IsEmptyNode())
		return;

	const int nDepth = oXmlReader.GetDepth();

	while (oXmlReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:FontFile" == oXmlReader.GetNameA())
		{
			m_wsFilePath = CombinePaths(wsRootPath, oXmlReader.GetText2());
			break;
		}
	}
}

void CFont::Apply(IRenderer* pRenderer, NSFonts::IFontManager* pFontManager, const double& dFontSize) const
{
	if (nullptr == pRenderer)
		return;

	int nFontStyle = 0;

	if (m_bBold)
		nFontStyle |= 0x01;
	if (m_bItalic)
		nFontStyle |= 0x02;

	pRenderer->put_FontStyle(nFontStyle);
	pRenderer->put_FontName(m_wsFontName);

	if (!m_wsFilePath.empty() && nullptr != pFontManager)
	{
		double dDpiX = 96, dDpiY = 96;

		pRenderer->get_DpiX(&dDpiX);
		pRenderer->get_DpiY(&dDpiY);

		pFontManager->LoadFontFromFile(m_wsFilePath, 0, dFontSize, dDpiX, dDpiY);
	}
}
}
