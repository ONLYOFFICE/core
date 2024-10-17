#include "CFont.h"

namespace SVG 
{
	CGlyph::CGlyph(XmlUtils::CXmlNode &oNode)
		: CPath(oNode)
	{
		std::wstring wsUnicode(oNode.GetAttribute(L"unicode"));

		if (!wsUnicode.empty())
			m_wchUnicode = wsUnicode[0];

		m_oHorizAdvX.SetValue(oNode.GetAttributeOrValue(L"horiz-adv-x"));
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

		m_oHorizAdvX.SetValue(oNode.GetAttributeOrValue(L"horiz-adv-x"));
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
		return false;
	}

	bool CFont::Draw(const std::wstring &wsText, const double& dX, const double& dY, const double& dFontHeight, IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == pRenderer || wsText.empty())
			return false;
		
		double dM11, dM12, dM21, dM22, dRx, dRy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

		Aggplus::CMatrix oMatrix(dM11, dM12, dM21, dM22, dRx, dRy);
		oMatrix.Translate(dX, dY);

		MGlyphsMap::const_iterator itFound;
		double dGlyphScale = 1.;

		#define DrawGlyph(glyphPtr, function) \
		const TBounds oGlyphBound{glyphPtr->GetBounds()};\
		if (Equals(0., dFontHeight) || Equals(oGlyphBound.m_dBottom, oGlyphBound.m_dTop) || Equals(oGlyphBound.m_dRight, oGlyphBound.m_dLeft)) \
			continue; \
		dGlyphScale = dFontHeight / 1000.; \
		if (!Equals(1., dGlyphScale)) \
		{ \
			oMatrix.Scale(dGlyphScale, -dGlyphScale); \
			pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty()); \
		} \
		function; \
		if (!Equals(1., dGlyphScale)) \
		{ \
			oMatrix.Scale(1. / dGlyphScale, -1. / dGlyphScale); \
			pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty()); \
		} \

		for (wchar_t wchGlyph : wsText)
		{
			itFound = m_mGlyphs.find(wchGlyph);

			if (m_mGlyphs.cend() == itFound)
			{
				if (NULL == m_pMissingGlyph)
					continue;

				DrawGlyph(m_pMissingGlyph, m_pMissingGlyph->Draw(pRenderer, pFile, oMode, pStyles, pContexObject))

				oMatrix.Translate(m_oHorizAdvX.ToDouble(NSCSS::Pixel), 0);
			}
			else
			{
				DrawGlyph(itFound->second, itFound->second->Draw(pRenderer, pFile, oMode, pStyles, pContexObject))

				if (!itFound->second->m_oHorizAdvX.Empty())
					oMatrix.Translate(itFound->second->m_oHorizAdvX.ToDouble(NSCSS::Pixel), 0);
				else
					oMatrix.Translate(m_oHorizAdvX.ToDouble(NSCSS::Pixel), 0);
			}

			pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), -oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
		}

		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dRx, dRy);

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
