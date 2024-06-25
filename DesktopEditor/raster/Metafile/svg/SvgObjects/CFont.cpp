#include "CFont.h"

namespace SVG 
{
	CGlyph::CGlyph(XmlUtils::CXmlNode &oNode)
		: CPath(oNode)
	{
		std::wstring wsUnicode(oNode.GetAttribute(L"unicode"));

		if (!wsUnicode.empty())
			m_wchUnicode = wsUnicode[0];
	}

	wchar_t CGlyph::GetUnicode() const
	{
		return m_wchUnicode;
	}

	CFontFace::CFontFace(XmlUtils::CXmlNode &oNode)
	{
		
	}

	CFont::CFont(XmlUtils::CXmlNode &oNode)
		: CAppliedObject(oNode), m_pMissingGlyph(NULL)
	{
		ParseGlyphs(oNode);

		m_oArguments.m_wsFontVariant = oNode.GetAttribute(L"font-variant");
		m_oArguments.m_wsFontStyle   = oNode.GetAttribute(L"font-style");
		m_oArguments.m_wsFontWidght  = oNode.GetAttribute(L"font-weight");
	}

	CFont::~CFont()
	{
		for (std::pair<wchar_t, CGlyph*> oElement : m_mGlyphs)
			RELEASEOBJECT(oElement.second);

		RELEASEOBJECT(m_pMissingGlyph);
	}

	void CFont::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
	}

	bool CFont::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		return true;
	}

	void CFont::ParseGlyphs(XmlUtils::CXmlNode &oNode)
	{
		std::vector<XmlUtils::CXmlNode> arChilds;

		if (!oNode.GetChilds(arChilds) || arChilds.empty())
			return;

		for (XmlUtils::CXmlNode& oChild : arChilds)
		{
			if (L"glyph" == oChild.GetName())
			{
				CGlyph *pGlyph = new CGlyph(oChild);

				if (NULL == pGlyph)
					continue;

				if (m_mGlyphs.end() == m_mGlyphs.find(pGlyph->GetUnicode()))
					m_mGlyphs.insert(std::make_pair(pGlyph->GetUnicode(), pGlyph));
				else
					delete pGlyph;
			}
			else if (L"missing-glyph" == oChild.GetName())
			{
				std::vector<XmlUtils::CXmlNode> arMissingGlyphChilds;
				if (!oChild.GetChilds(arMissingGlyphChilds) || arMissingGlyphChilds.empty())
					continue;

				for (XmlUtils::CXmlNode& oChildMissingGlyph : arMissingGlyphChilds)
				{
					if (L"path" == oChildMissingGlyph.GetName())
					{
						m_pMissingGlyph = new CPath(oChildMissingGlyph);
						if (NULL != m_pMissingGlyph)
							break;
					}
				}
			}
		}
	}
}
