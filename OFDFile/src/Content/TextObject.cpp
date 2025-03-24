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

void CTextCode::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer || m_wsText.empty())
		return;

	pRenderer->CommandDrawText(m_wsText, m_dX, m_dY, 0, 0);
}

CTextObject::CTextObject(CXmlReader& oLiteReader)
	: IPageBlock(oLiteReader), CGraphicUnit(oLiteReader),
	  m_bStroke(false), m_bFill(false), m_dHScale(1.),
	  m_unReadDirection(0), m_unCharDirection(0), m_unWeight(400),
	  m_bItalic(false),
	  m_pFillColor(nullptr), m_pStrokeColor(nullptr)
{
	CTextObject::Read(oLiteReader);
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

bool CTextObject::Read(CXmlReader& oLiteReader)
{
	if (L"ofd:TextObject" != oLiteReader.GetName() || oLiteReader.IsEmptyElement() || !oLiteReader.IsValid())
		return false;

	if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oLiteReader.GetName();

			if (L"Font" == wsAttributeName)
				m_unFont = oLiteReader.GetUInteger(true);
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
	}

	return true;
}

void CTextObject::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer || m_arTextCodes.empty())
		return;

	((CGraphicUnit*)this)->Apply(pRenderer);

	if (m_bFill && nullptr != m_pFillColor)
	{
		pRenderer->put_BrushType(c_BrushTypeSolid);
		pRenderer->put_BrushColor1(255);
		pRenderer->put_BrushAlpha1(255);
	}
	else
		pRenderer->put_BrushType(c_BrushTypeNotSet);

	pRenderer->put_FontSize(m_dSize);

	for (const CTextCode* pTextCode : m_arTextCodes)
		pTextCode->Draw(pRenderer);
}

}
