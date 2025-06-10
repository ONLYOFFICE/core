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

			std::vector<std::string>::const_iterator itElement = arValues.cbegin();

			char chElementName;

			while (arValues.cend() != itElement)
			{
				if (arValues.front().length() != 1)
				{
					++itElement;
					continue;
				}

				chElementName = (*itElement)[0];
				++itElement;

				switch (chElementName)
				{
					case 'S':
					{
						AddElement(CStartElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'M':
					{
						AddElement(CMoveElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'L':
					{
						AddElement(CLineElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'Q':
					{
						AddElement(CBezierCurve2Element::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'B':
					{
						AddElement(CBezierCurveElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'A':
					{
						AddElement(CArcElement::ReadFromArray(itElement, arValues.cend()));
						break;
					}
					case 'C':
					{
						AddElement(new CCloseElement());
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

	TMatrix oOldTransform;
	CGraphicUnit::Apply(pRenderer, oOldTransform);

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

	pRenderer->SetTransform(oOldTransform.m_dM11, oOldTransform.m_dM12, oOldTransform.m_dM21, oOldTransform.m_dM22, oOldTransform.m_dDx, oOldTransform.m_dDy);
}

CStartElement::CStartElement()
{}

IPathElement* CStartElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 2)
		return nullptr;

	CStartElement *pElement = new CStartElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX) && StringToDouble(*itBegin++, pElement->m_dY))
		return pElement;

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

IPathElement* CMoveElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 2)
		return nullptr;

	CMoveElement *pElement = new CMoveElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX) && StringToDouble(*itBegin++, pElement->m_dY))
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

IPathElement* CLineElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 2)
		return nullptr;

	CLineElement *pElement = new CLineElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX) && StringToDouble(*itBegin++, pElement->m_dY))
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

IPathElement* CBezierCurve2Element::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 4)
		return nullptr;

	CBezierCurve2Element *pElement = new CBezierCurve2Element();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX1) && StringToDouble(*itBegin++, pElement->m_dY1) &&
	    StringToDouble(*itBegin++, pElement->m_dX2) && StringToDouble(*itBegin++, pElement->m_dY2))
		return pElement;

	delete pElement;
	return nullptr;
}

void CBezierCurve2Element::Draw(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer)
		return;

	double dX = 0, dY = 0;
	pRenderer->PathCommandGetCurrentPoint(&dX, &dY);
	pRenderer->PathCommandCurveTo(dX, dY, m_dX1, m_dY1, m_dX2, m_dY2);
}

CBezierCurveElement::CBezierCurveElement()
{}

IPathElement* CBezierCurveElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{
	if (itEnd - itBegin < 6)
		return nullptr;

	CBezierCurveElement *pElement = new CBezierCurveElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble(*itBegin++, pElement->m_dX1) && StringToDouble(*itBegin++, pElement->m_dY1) &&
	    StringToDouble(*itBegin++, pElement->m_dX2) && StringToDouble(*itBegin++, pElement->m_dY2) &&
	    StringToDouble(*itBegin++, pElement->m_dX3) && StringToDouble(*itBegin++, pElement->m_dY3))
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

IPathElement* CArcElement::ReadFromArray(std::vector<std::string>::const_iterator& itBegin, const std::vector<std::string>::const_iterator& itEnd)
{

	if (itEnd - itBegin < 7)
		return nullptr;

	CArcElement *pElement = new CArcElement();

	if (nullptr == pElement)
		return nullptr;

	if (StringToDouble (*itBegin++, pElement->m_dRadiusX) && StringToDouble (*itBegin++, pElement->m_dRadiusY) &&
	    StringToDouble (*itBegin++, pElement->m_dAngle)   && StringToBoolean(*itBegin++, pElement->m_bLarge)   &&
	    StringToBoolean(*itBegin++, pElement->m_bSweep)   && StringToDouble (*itBegin++, pElement->m_dX)       &&
	    StringToDouble (*itBegin++, pElement->m_dY))
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

void CCloseElement::Draw(IRenderer* pRenderer) const
{
	if (nullptr != pRenderer)
		pRenderer->PathCommandClose();
}
}
