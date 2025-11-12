#include "CFont.h"

namespace SVG 
{
	CGlyph::CGlyph(CSvgReader& oReader, CSvgFile* pFile)
		: CPath(oReader)
	{}

	void CGlyph::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("unicode" == sName)
		{
			const std::wstring wsUnicode{oReader.GetText()};

			if (!wsUnicode.empty())
				m_wchUnicode = wsUnicode[0];
		}
		else if ("horiz-adv-x" == sName)
			m_oHorizAdvX.SetValue(oReader.GetText());
		else
			CPath::SetAttribute(sName, oReader);
	}

	wchar_t CGlyph::GetUnicode() const
	{
		return m_wchUnicode;
	}

	CFontFace::CFontFace(CSvgReader& oReader)
	{}

	CFont::CFont(CSvgReader& oReader, CSvgFile* pFile)
		: CAppliedObject(oReader), m_pMissingGlyph(NULL)
	{
		ParseGlyphs(oReader, pFile);
	}

	CFont::~CFont()
	{
		for (std::pair<wchar_t, CGlyph*> oElement : m_mGlyphs)
			RELEASEOBJECT(oElement.second);

		RELEASEOBJECT(m_pMissingGlyph);
	}

	void CFont::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("font-variant" == sName)
			m_oArguments.m_wsFontVariant = oReader.GetText();
		else if ("font-style" == sName)
			m_oArguments.m_wsFontStyle = oReader.GetText();
		else if ("font-weight" == sName)
			m_oArguments.m_wsFontWidght = oReader.GetText();
		else if ("horiz-adv-x" == sName)
			m_oHorizAdvX.SetValue(oReader.GetText());
		else
			CAppliedObject::SetAttribute(sName, oReader);
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
		}

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

	void CFont::ParseGlyphs(CSvgReader& oReader, CSvgFile* pFile)
	{
		WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
		{
			if ("glyph" == sNodeName)
			{
				CGlyph *pGlyph = CObject::Create<CGlyph>(oReader, pFile, pFile);

				if (NULL == pGlyph)
					continue;

				if (m_mGlyphs.end() == m_mGlyphs.find(pGlyph->GetUnicode()))
					m_mGlyphs.insert(std::make_pair(pGlyph->GetUnicode(), pGlyph));
				else
					delete pGlyph;
			}
			else if ("missing-glyph" == sNodeName)
			{
				WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, "path")
				{
					m_pMissingGlyph = new CPath(oReader);
					if (NULL != m_pMissingGlyph)
						break;
				}
				END_WHILE
			}
		}
		END_WHILE
	}
}
