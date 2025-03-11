#include "TextObject.h"

#include "../Utils/Utils.h"

namespace OFD
{
CTextCode::CTextCode(XmlUtils::CXmlLiteReader& oLiteReader)
{
	if (NO_VALID_NODE(oLiteReader, L"ofd:TextCode"))
		return;

	if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oLiteReader.GetName();

			if (L"X" == wsAttributeName)
				m_dX = XmlUtils::GetDouble(oLiteReader.GetText());
			else if (L"Y" == wsAttributeName)
				m_dY = XmlUtils::GetDouble(oLiteReader.GetText());
			else if (L"DeltaX" == wsAttributeName)
				m_arDeltaX = GetDoubleValues(oLiteReader.GetText());
			else if (L"DeltaY" == wsAttributeName)
				m_arDeltaY = GetDoubleValues(oLiteReader.GetText());
		} while (oLiteReader.MoveToNextAttribute());
	}

	oLiteReader.MoveToElement();

	m_wsText = oLiteReader.GetText2();
}

CTextObject::CTextObject(XmlUtils::CXmlLiteReader& oLiteReader)
	: CGraphicUnit(oLiteReader),
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

	for (CTextCode* pTextCode : m_arTextCodes)
		delete pTextCode;
}

bool CTextObject::Read(XmlUtils::CXmlLiteReader& oLiteReader)
{
	if (NO_VALID_NODE(oLiteReader, L"ofd:TextObject"))
		return false;

	if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oLiteReader.GetName();

			if (L"Font" == wsAttributeName)
			{
					m_unFont = XmlUtils::GetUInteger(oLiteReader.GetText());
			}
			else if (L"Size" == wsAttributeName)
				m_dSize = XmlUtils::GetDouble(oLiteReader.GetText());
			else if (L"Stroke" == wsAttributeName)
				m_bStroke = XmlUtils::GetBoolean(oLiteReader.GetText());
			else if (L"Fill" == wsAttributeName)
				m_bFill = XmlUtils::GetBoolean(oLiteReader.GetText());
			else if (L"HScale" == wsAttributeName)
				m_dHScale = XmlUtils::GetDouble(oLiteReader.GetText());
			else if (L"ReadDirection" == wsAttributeName)
				m_unReadDirection = XmlUtils::GetUInteger(oLiteReader.GetText());
			else if (L"CharDirection" == wsAttributeName)
				m_unCharDirection = XmlUtils::GetUInteger(oLiteReader.GetText());
			else if (L"Weight" == wsAttributeName)
				m_unWeight = XmlUtils::GetUInteger(oLiteReader.GetText());
			else if (L"Italic" == wsAttributeName)
				m_bItalic =  XmlUtils::GetBoolean(oLiteReader.GetText());
		} while (oLiteReader.MoveToNextAttribute());
	}

	oLiteReader.MoveToElement();

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:FillColor" == wsNodeName && m_bFill)
		{
			if (nullptr != m_pFillColor)
				delete m_pFillColor;

			m_pFillColor = new TColor(oLiteReader);
		}
		else if (L"ofd:StrokeColor" == wsNodeName && m_bStroke)
		{
			if (nullptr != m_pStrokeColor)
				delete m_pStrokeColor;

			m_pStrokeColor = new TColor(oLiteReader);
		}
		else if (L"ofd:TextCode" == wsNodeName)
			m_arTextCodes.push_back(new CTextCode(oLiteReader));
	}

	return true;
}

}
