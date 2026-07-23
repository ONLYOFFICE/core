#include "Region.h"

#include "../Content/PathObject.h"

namespace OFD
{
CArea::CArea(CXmlReader& oReader)
{
	if (oReader.MoveToFirstAttribute())
	{
		do
		{
			if ("Start" == oReader.GetNameA())
			{
				m_oStart.Read(oReader.GetText2A());
				break;
			}
		}while (oReader.MoveToNextAttribute());

		oReader.MoveToElement();
	}

	const int nDepth{oReader.GetDepth()};
	std::string sName;

	while (oReader.ReadNextSiblingNode2(nDepth))
	{
		sName = oReader.GetNameA();

		if ("Move" == sName)
			AddElement(CMoveElement::ReadFromNode(oReader));
		else if ("Line" == sName)
			AddElement(CLineElement::ReadFromNode(oReader));
		else if ("QuadraticBezier" == sName)
			AddElement(CBezierCurve2Element::ReadFromNode(oReader));
		else if ("CubicBezier" == sName)
			AddElement(CBezierCurveElement::ReadFromNode(oReader));
		else if ("Arc" == sName)
			AddElement(CArcElement::ReadFromNode(oReader));
	}
}

CArea::~CArea()
{
	for (IPathElement* pElement : m_arElements)
		delete pElement;
}

void CArea::AddElement(IPathElement* pElement)
{
	if (nullptr == pElement)
		return;

	m_arElements.push_back(pElement);
}

CRegion::CRegion(CXmlReader& oReader)
{
	const int nDepth{oReader.GetDepth()};

	while (oReader.ReadNextSiblingNode2(nDepth))
	{
		if ("Area" != oReader.GetNameA())
			continue;

		CArea *pArea = new CArea(oReader);

		if (nullptr != pArea)
			m_arAreas.push_back(pArea);
	}
}

CRegion::~CRegion()
{
	for (CArea* pArea : m_arAreas)
		delete pArea;
}

}
