#include "TextObject.h"

#include "../Utils/Utils.h"

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
			else if (L"DeltaX" == wsAttributeName ||
			         L"DeltaY" == wsAttributeName)
			{
				const std::vector<std::string> arValues{oLiteReader.GetArrayStrings(true)};

				std::vector<double>& arDelta{L"DeltaX" == wsAttributeName ? m_arDeltaX : m_arDeltaY};

				arDelta.reserve(arValues.size());
				double dValue = 0.;

				for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
				{
					if ("g" == arValues[unIndex] && unIndex + 2 < arValues.size())
					{
						unsigned int unCount = 0;

						if (!StringToUInteger(arValues[unIndex + 1], unCount) || !StringToDouble(arValues[unIndex + 2], dValue))
							continue;

						unIndex += 2;

						arDelta.insert(arDelta.end(), unCount, dValue);
					}
					else if (StringToDouble(arValues[unIndex], dValue))
						arDelta.push_back(dValue);
					else
						arDelta.push_back(0.);
				}
			}
		} while (oLiteReader.MoveToNextAttribute());
	}

	oLiteReader.MoveToElement();

	m_wsText = oLiteReader.GetText2();
}

void CTextCode::Draw(IRenderer* pRenderer, unsigned int& unIndex, const std::vector<TCGTransform>& arCGTransforms) const
{
	if (nullptr == pRenderer || m_wsText.empty())
		return;

	double dX = m_dX, dY = m_dY, dDeltaX = 0, dDeltaY = 0;
	bool bDrawed = false;

	for (unsigned int unGlyphIndex = 0; unGlyphIndex < m_wsText.length(); ++unGlyphIndex)
	{
		if (!arCGTransforms.empty())
		{
			for (const TCGTransform& oCGTransform : arCGTransforms)
			{
				if (oCGTransform.Draw(pRenderer, m_wsText[unGlyphIndex], unIndex, dX, dY))
				{
					bDrawed = true;
					break;
				}
			}
		}

		if (!bDrawed)
		{
			pRenderer->CommandDrawTextCHAR(m_wsText[unGlyphIndex], dX, dY, 0, 0);
			++unIndex;
		}

		if (unGlyphIndex < m_arDeltaX.size())
			dDeltaX = m_arDeltaX[unGlyphIndex];

		if (unGlyphIndex < m_arDeltaY.size())
			dDeltaY = m_arDeltaY[unGlyphIndex];

		dX += dDeltaX;
		dY += dDeltaY;
	}
}

CTextObject::CTextObject(CXmlReader& oLiteReader)
	: IPageBlock(oLiteReader), CGraphicUnit(oLiteReader),
	  m_bStroke(false), m_bFill(true), m_dHScale(1.),
	  m_unReadDirection(0), m_unCharDirection(0), m_unWeight(400),
	  m_bItalic(false),
	  m_pFillColor(nullptr), m_pStrokeColor(nullptr), m_unFontID(0)
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
				m_unFontID = oLiteReader.GetUInteger(true);
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

			m_pFillColor = new CColor(oLiteReader);
		}
		else if (L"ofd:StrokeColor" == wsNodeName)
		{
			if (nullptr != m_pStrokeColor)
				delete m_pStrokeColor;

			m_pStrokeColor = new CColor(oLiteReader);
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

void CTextObject::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer || m_arTextCodes.empty())
		return;

	const CRes* pPublicRes{oCommonData.GetPublicRes()};

	const CFont* pFont = pPublicRes->GetFont(m_unFontID);

	if (nullptr == pFont)
		return;

	pFont->Apply(pRenderer);

	TMatrix oOldTransform;
	CGraphicUnit::Apply(pRenderer, oOldTransform);

	std::vector<const CDrawParam*> arDrawParams{pPublicRes->GetDrawParams()};

	if (m_bFill)
	{
		pRenderer->put_BrushType(c_BrushTypeSolid);

		if (nullptr != m_pFillColor)
		{
			pRenderer->put_BrushColor1(m_pFillColor->ToInt(pPublicRes));
			pRenderer->put_BrushAlpha1(m_pFillColor->GetAlpha());
		}
		else
		{
			pRenderer->put_BrushColor1(0);

			if (EPageType::TemplatePage == ePageType)
				for (const CDrawParam* pDrawParam : arDrawParams)
					if (pDrawParam->ApplyFillColor(pRenderer, pPublicRes))
						break;
		}
	}
	else
		pRenderer->put_BrushType(c_BrushTypeNotSet);

	pRenderer->put_FontSize(m_dSize * 72. / 25.4);

	unsigned int unGlyphsIndex = 0;

	for (const CTextCode* pTextCode : m_arTextCodes)
		pTextCode->Draw(pRenderer, unGlyphsIndex, m_arCGTransforms);

	pRenderer->SetTransform(oOldTransform.m_dM11, oOldTransform.m_dM12, oOldTransform.m_dM21, oOldTransform.m_dM22, oOldTransform.m_dDx, oOldTransform.m_dDy);
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

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:Glyphs" != oLiteReader.GetNameA())
			continue;

		const std::vector<unsigned int> arValues{oLiteReader.GetArrayUInteger()};
		oCGTransform.m_arGlyphs.insert(oCGTransform.m_arGlyphs.end(), arValues.begin(), arValues.end());
	}

	return oCGTransform;
}

bool TCGTransform::Draw(IRenderer* pRenderer, const LONG& lUnicode, unsigned int& unIndex, double dX, double dY) const
{
	if (m_unCodePosition + m_arGlyphs.size() > unIndex || 0 == m_unCodeCount || m_arGlyphs.empty())
		return false;

	for (unsigned int unGlyphCount = 0; unGlyphCount < m_arGlyphs.size(); ++unGlyphCount)
		pRenderer->CommandDrawTextExCHAR(lUnicode, m_arGlyphs[unGlyphCount], dX, dY, 0, 0);

	unIndex += m_unCodeCount;

	return true;
}
}
