#include "PathObject.h"
#include "src/Utils/Utils.h"

namespace OFD
{
CPathObject::CPathObject(CXmlReader& oLiteReader)
	: IPageBlock(oLiteReader), CGraphicUnit(oLiteReader),
	  m_bStroke(true), m_bFill(false), m_eRule(ERule::NonZero),
	  m_pFillColor(nullptr), m_pStrokeColor(nullptr)
{
	if (L"ofd:PathObject" != oLiteReader.GetName() || oLiteReader.IsEmptyElement() || !oLiteReader.IsValid())
		return;

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
						AddElement(CBezierCurveElement::ReadFromArray(arValues));
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

void CPathObject::Draw(IRenderer* pRenderer, const CCommonData& oCommonData) const
{
	if (nullptr == pRenderer || m_arElements.empty())
		return;

	CGraphicUnit::Apply(pRenderer);

	pRenderer->BeginCommand(c_nPathType);
	pRenderer->PathCommandStart();

	for (const IPathElement* pElement : m_arElements)
		pElement->Draw(pRenderer);

	int nEndType = -1;

	if (m_bStroke)
		nEndType = c_nStroke;

	if (m_bFill)
	{
		switch (m_eRule)
		{
			case ERule::NonZero:
			{
				nEndType = (-1 == nEndType ? c_nWindingFillMode : nEndType | c_nWindingFillMode);
				break;
			}
			case ERule::Even_Odd:
			{
				nEndType = (-1 == nEndType ? c_nEvenOddFillMode : nEndType | c_nEvenOddFillMode);
				break;
			}
		}
	}

	if (m_bFill)
	{
		pRenderer->put_BrushType(c_BrushTypeSolid);

		if (nullptr != m_pFillColor)
		{
			pRenderer->put_BrushColor1(m_pFillColor->ToInt(oCommonData.GetPublicRes()));
			pRenderer->put_BrushAlpha1(m_pFillColor->GetAlpha());
		}
		else
		{
			pRenderer->put_BrushColor1(0);
			pRenderer->put_BrushAlpha1(0xff);
		}
	}
	else
		pRenderer->put_BrushType(c_BrushTypeNotSet);


	if(m_bStroke)
	{
		pRenderer->put_PenSize(m_dLineWidth);

		if (nullptr != m_pStrokeColor)
		{
			pRenderer->put_PenColor(m_pStrokeColor->ToInt(oCommonData.GetPublicRes()));
			pRenderer->put_PenAlpha(m_pStrokeColor->GetAlpha());
		}
		else
		{
			pRenderer->put_PenColor(0);
			pRenderer->put_PenAlpha(0xff);
		}
	}
	else
		pRenderer->put_PenSize(0.);

	if (-1 != nEndType)
		pRenderer->DrawPath(nEndType);

	pRenderer->PathCommandEnd();
	pRenderer->EndCommand(c_nPathType);
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

void CStartElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandMoveTo(m_dX, m_dY);
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

void CMoveElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandMoveTo(m_dX, m_dY);
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

void CLineElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandLineTo(m_dX, m_dY);
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

void CBezierCurve2Element::Draw(IRenderer* pRenderer) const
{
	// if (nullptr != pRenderer)
	// 	pRenderer->PathCommandCurveTo()
}

CBezierCurveElement::CBezierCurveElement()
{}

IPathElement* CBezierCurveElement::ReadFromArray(std::vector<std::string>& arValues)
{
	if (arValues.size() < 4)
		return nullptr;

	CBezierCurveElement *pElement = new CBezierCurveElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(arValues[0], pElement->m_dX1) && StringToDouble(arValues[1], pElement->m_dY1) &&
	    StringToDouble(arValues[2], pElement->m_dX2) && StringToDouble(arValues[3], pElement->m_dY2))
		return pElement;

	delete pElement;
	return nullptr;
}

void CBezierCurveElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandCurveTo(m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3);
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

void CArcElement::Draw(IRenderer* pRenderer) const
{
	// if (nullptr != pRenderer)
		// pRenderer->PathCommandArcTo(m_dX, m_dY, m_dRadiusX * 2., m_dRadiusY * 2., )
}

CCloseElement::CCloseElement()
{}

IPathElement* CCloseElement::ReadFromArray(std::vector<std::string>& arValues)
{
	return new CCloseElement();
}

void CCloseElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandClose();
}
}
