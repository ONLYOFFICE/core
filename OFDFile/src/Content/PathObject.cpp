#include "PathObject.h"
#include "src/Utils/Utils.h"

namespace OFD
{
CPathObject::CPathObject(CXmlReader& oLiteReader)
	: IPageBlock(oLiteReader), CGraphicUnit(oLiteReader),
	  m_bStroke(true), m_bFill(true), m_eRule(ERule::NonZero),
	  m_pFillColor(nullptr), m_pStrokeColor(nullptr)
{
	CPathObject::Read(oLiteReader);
}

CPathObject::~CPathObject()
{
	for (const IPathElement* pElement : m_arElements)
		delete pElement;
}

void CPathObject::AddElement(const IPathElement* pElement)
{
	if (nullptr != pElement)
		m_arElements.push_back(pElement);
}

bool CPathObject::Read(CXmlReader& oLiteReader)
{
	if (L"ofd:PathObject" != oLiteReader.GetName() || oLiteReader.IsEmptyElement() || !oLiteReader.IsValid())
		return false;

	if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
	{
		std::wstring wsAttributeName;

		do
		{
			wsAttributeName = oLiteReader.GetName();

			if (L"Stroke" == wsAttributeName)
				m_bStroke = oLiteReader.GetBoolean(true);
			else if (L"Fill" == wsAttributeName)
				m_bFill = oLiteReader.GetBoolean(true);
			else if (L"Rule" == wsAttributeName)
			{
				if (L"Even-odd" == oLiteReader.GetText())
					m_eRule = ERule::Even_Odd;
				else
					m_eRule = ERule::NonZero;
			}
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
		else if (L"ofd:AbbreviatedData" == wsNodeName)
		{
			std::vector<std::string> arValues{Split(oLiteReader.GetText2A(), ' ')};

			char chElementName;

			while (!arValues.empty())
			{
				if (arValues.front().length() != 1)
				{
					arValues.erase(arValues.begin());
					continue;
				}

				chElementName = arValues[0][0];
				arValues.erase(arValues.begin());

				switch (chElementName)
				{
					case 'S':
					{
						AddElement(CStartElement::ReadFromArray(arValues));
						break;
					}
					case 'M':
					{
						AddElement(CMoveElement::ReadFromArray(arValues));
						break;
					}
					case 'L':
					{
						AddElement(CLineElement::ReadFromArray(arValues));
						break;
					}
					case 'Q':
					{
						AddElement(CBezierCurve2Element::ReadFromArray(arValues));
						break;
					}
					case 'B':
					{
						AddElement(CBezierCurve3Element::ReadFromArray(arValues));
						break;
					}
					case 'A':
					{
						AddElement(CArcElement::ReadFromArray(arValues));
						break;
					}
					case 'C':
					{
						AddElement(CCloseElement::ReadFromArray(arValues));
						break;
					}
					default:
						continue;
				}
			}
		}
	}

	return true;
}

void CPathObject::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer || m_arElements.empty())
		return;

	((CGraphicUnit*)this)->Apply(pRenderer);
}

CStartElement::CStartElement()
{}

IPathElement* CStartElement::ReadFromArray(std::vector<std::string>& arValues)
{
	if (arValues.size() < 2)
		return nullptr;

	CStartElement *pElement = new CStartElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(arValues[0], pElement->m_dX) && StringToDouble(arValues[1], pElement->m_dY))
		return pElement;

	if (nullptr != pElement)
		delete pElement;

	return nullptr;
}

CMoveElement::CMoveElement()
{}

IPathElement* CMoveElement::ReadFromArray(std::vector<std::string>& arValues)
{
	if (arValues.size() < 2)
		return nullptr;

	CMoveElement *pElement = new CMoveElement();

	if (nullptr == pElement)
		return nullptr;


	if (StringToDouble(arValues[0], pElement->m_dX) && StringToDouble(arValues[1], pElement->m_dY))
		return pElement;

	delete pElement;
	return nullptr;
}

CLineElement::CLineElement()
{}

IPathElement* CLineElement::ReadFromArray(std::vector<std::string>& arValues)
{
	if (arValues.size() < 2)
		return nullptr;

	CLineElement *pElement = new CLineElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(arValues[0], pElement->m_dX) && StringToDouble(arValues[1], pElement->m_dY))
		return pElement;

	delete pElement;
	return nullptr;
}

CBezierCurve2Element::CBezierCurve2Element()
{}

IPathElement* CBezierCurve2Element::ReadFromArray(std::vector<std::string>& arValues)
{
	if (arValues.size() < 4)
		return nullptr;

	CBezierCurve2Element *pElement = new CBezierCurve2Element();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(arValues[0], pElement->m_dX1) && StringToDouble(arValues[1], pElement->m_dY1) &&
	    StringToDouble(arValues[2], pElement->m_dX2) && StringToDouble(arValues[3], pElement->m_dY2))
		return pElement;

	delete pElement;
	return nullptr;
}

CBezierCurve3Element::CBezierCurve3Element()
{}

IPathElement* CBezierCurve3Element::ReadFromArray(std::vector<std::string>& arValues)
{
	if (arValues.size() < 4)
		return nullptr;

	CBezierCurve3Element *pElement = new CBezierCurve3Element();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(arValues[0], pElement->m_dX1) && StringToDouble(arValues[1], pElement->m_dY1) &&
	    StringToDouble(arValues[2], pElement->m_dX2) && StringToDouble(arValues[3], pElement->m_dY2))
		return pElement;

	delete pElement;
	return nullptr;
}

CArcElement::CArcElement()
{}

IPathElement* CArcElement::ReadFromArray(std::vector<std::string>& arValues)
{
	if (arValues.size() < 7)
		return nullptr;

	CArcElement *pElement = new CArcElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(arValues[0], pElement->m_dRadiusX) && StringToDouble(arValues[1], pElement->m_dRadiusY) &&
	    StringToDouble(arValues[2], pElement->m_dAngle) && StringToBoolean(arValues[3], pElement->m_bLarge) &&
	    StringToBoolean(arValues[4], pElement->m_bSweep) && StringToDouble(arValues[5], pElement->m_dX) &&
	    StringToDouble(arValues[6], pElement->m_dY))
		return pElement;

	delete pElement;
	return nullptr;
}

CCloseElement::CCloseElement()
{}

IPathElement* CCloseElement::ReadFromArray(std::vector<std::string>& arValues)
{
	return new CCloseElement();
}

}
