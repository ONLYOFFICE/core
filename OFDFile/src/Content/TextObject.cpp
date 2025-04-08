#include "TextObject.h"

namespace OFD
{
CTextCode::CTextCode(CXmlReader& oLiteReader)
{
	if (L"ofd:TextCode" != oLiteReader.GetName() || oLiteReader.IsEmptyElement() || !oLiteReader.IsValid())
		return;

	if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oLiteReader.GetName();

			if (L"X" == wsAttributeName)
				m_dX = oLiteReader.GetDouble(true);
			else if (L"Y" == wsAttributeName)
				m_dY = oLiteReader.GetDouble(true);
			else if (L"DeltaX" == wsAttributeName)
				m_arDeltaX = oLiteReader.GetArrayDoubles(true);
			else if (L"DeltaY" == wsAttributeName)
				m_arDeltaY = oLiteReader.GetArrayDoubles(true);
		} while (oLiteReader.MoveToNextAttribute());
	}

	oLiteReader.MoveToElement();

	m_wsText = oLiteReader.GetText2();
}

void CTextCode::Draw(IRenderer* pRenderer, unsigned int& unIndex, const std::vector<TCGTransform>& arCGTransforms) const
{
	if (nullptr == pRenderer || m_wsText.empty())
		return;

	double dX = m_dX, dDelta = 0;
	bool bDrawed = false;

	for (unsigned int unGlyphIndex = 0; unGlyphIndex < m_wsText.length(); ++unGlyphIndex)
	{
		if (!arCGTransforms.empty() && false)
		{
			for (const TCGTransform& oCGTransform : arCGTransforms)
			{
				if (oCGTransform.Draw(pRenderer, unIndex, dX, m_dY))
				{
					bDrawed = true;
					break;
				}
			}
		}

		if (!bDrawed)
		{
			pRenderer->CommandDrawTextCHAR(m_wsText[unGlyphIndex], dX, m_dY, 0, 0);
			++unIndex;
		}

		if (unGlyphIndex < m_arDeltaX.size())
			dDelta = m_arDeltaX[unGlyphIndex];

		dX += dDelta;
	}
}

CTextObject::CTextObject(CXmlReader& oLiteReader, const CRes* pPublicRes, NSFonts::IFontManager* pFontManager)
	: IPageBlock(oLiteReader), CGraphicUnit(oLiteReader),
	  m_bStroke(false), m_bFill(false), m_dHScale(1.),
	  m_unReadDirection(0), m_unCharDirection(0), m_unWeight(400),
	  m_bItalic(false),
	  m_pFillColor(nullptr), m_pStrokeColor(nullptr), m_pFont(nullptr),
	  m_pFontManager(pFontManager)
{
	if (L"ofd:TextObject" != oLiteReader.GetName() || oLiteReader.IsEmptyElement() || !oLiteReader.IsValid())
		return;

	if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oLiteReader.GetName();

			if (L"Font" == wsAttributeName)
			{
				if (nullptr != pPublicRes)
					m_pFont = pPublicRes->GetFont(oLiteReader.GetUInteger(true));
			}
			else if (L"Size" == wsAttributeName)
				m_dSize = oLiteReader.GetDouble(true);
			else if (L"Stroke" == wsAttributeName)
				m_bStroke = oLiteReader.GetBoolean(true);
			else if (L"Fill" == wsAttributeName)
				m_bFill = oLiteReader.GetBoolean(true);
			else if (L"HScale" == wsAttributeName)
				m_dHScale = oLiteReader.GetDouble(true);
			else if (L"ReadDirection" == wsAttributeName)
				m_unReadDirection = oLiteReader.GetUInteger(true);
			else if (L"CharDirection" == wsAttributeName)
				m_unCharDirection =oLiteReader.GetUInteger(true);
			else if (L"Weight" == wsAttributeName)
				m_unWeight = oLiteReader.GetUInteger(true);
			else if (L"Italic" == wsAttributeName)
				m_bItalic = oLiteReader.GetBoolean(true);
		} while (oLiteReader.MoveToNextAttribute());
	}

	oLiteReader.MoveToElement();

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:FillColor" == wsNodeName)
		{
			if (nullptr != m_pFillColor)
				delete m_pFillColor;

			m_pFillColor = new CColor(oLiteReader, pPublicRes);
		}
		else if (L"ofd:StrokeColor" == wsNodeName)
		{
			if (nullptr != m_pStrokeColor)
				delete m_pStrokeColor;

			m_pStrokeColor = new CColor(oLiteReader, pPublicRes);
		}
		else if (L"ofd:TextCode" == wsNodeName)
			m_arTextCodes.push_back(new CTextCode(oLiteReader));
		else if (L"ofd:CGTransform" == wsNodeName)
			m_arCGTransforms.push_back(TCGTransform::Read(oLiteReader));
	}
}

CTextObject::~CTextObject()
{
	if (nullptr != m_pFillColor)
		delete m_pFillColor;

	if (nullptr != m_pStrokeColor)
		delete m_pStrokeColor;

	for (const CTextCode* pTextCode : m_arTextCodes)
		delete pTextCode;
}

void CTextObject::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer || m_arTextCodes.empty())
		return;

	CGraphicUnit::Apply(pRenderer);

	if (nullptr != m_pFont)
		m_pFont->Apply(pRenderer, m_pFontManager, m_dSize * 72. / 25.4);

	if (m_bFill && nullptr != m_pFillColor)
	{
		pRenderer->put_BrushType(c_BrushTypeSolid);
		pRenderer->put_BrushColor1(m_pFillColor->ToInt());
		pRenderer->put_BrushAlpha1(m_pFillColor->GetAlpha());
	}
	else
		pRenderer->put_BrushType(c_BrushTypeNotSet);

	pRenderer->put_FontSize(m_dSize * 72. / 25.4);

	unsigned int unGlyphsIndex;

	for (const CTextCode* pTextCode : m_arTextCodes)
		pTextCode->Draw(pRenderer, unGlyphsIndex, m_arCGTransforms);
}

TCGTransform TCGTransform::Read(CXmlReader& oLiteReader)
{
	TCGTransform oCGTransform;

	if (L"ofd:CGTransform" != oLiteReader.GetName() || oLiteReader.IsEmptyElement())
		return oCGTransform;

	if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oLiteReader.GetName();

			if (L"CodePosition" == wsAttributeName)
				oCGTransform.m_unCodePosition = oLiteReader.GetUInteger(true);
			else if (L"CodeCount" == wsAttributeName)
				oCGTransform.m_unCodeCount = oLiteReader.GetUInteger(true);
			else if (L"GlyphCount" == wsAttributeName)
				oCGTransform.m_unGlyphCount = oLiteReader.GetUInteger(true);
		} while (oLiteReader.MoveToNextAttribute());
	}

	oLiteReader.MoveToElement();

	const int nDepth = oLiteReader.GetDepth();
	unsigned int unCount = 0;

	while (oLiteReader.ReadNextSiblingNode(nDepth) && unCount < oCGTransform.m_unGlyphCount)
	{
		if ("ofd:Glyphs" == oLiteReader.GetNameA())
		{
			oCGTransform.m_arGlyphs.push_back(oLiteReader.GetUInteger());
			++unCount;
		}
	}

	return oCGTransform;
}

bool TCGTransform::Draw(IRenderer* pRenderer, unsigned int& unIndex, double dX, double dY) const
{
	if (m_unCodePosition != unIndex || 0 == m_unCodeCount || 0 == m_unGlyphCount)
		return false;

	pRenderer->put_FontStringGID(1);

	for (unsigned int unGlyphCount = 0; unGlyphCount < m_unGlyphCount; ++unGlyphCount)
		pRenderer->CommandDrawTextCHAR(m_arGlyphs[unGlyphCount], dX, dY, 0, 0);

	unIndex += m_unCodeCount;

	pRenderer->put_FontStringGID(0);

	return true;
}
}
