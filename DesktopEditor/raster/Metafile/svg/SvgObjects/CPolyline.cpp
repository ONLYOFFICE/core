#include "CPolyline.h"

namespace SVG
{
	CPolyline::CPolyline(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent)
		: CPath(oNode, pParent, false)
	{
		std::vector<double> arValues = NSCSS::NS_STATIC_FUNCTIONS::ReadDoubleValues(oNode.GetAttribute(L"points"));

		if (arValues.size() < 4)
			return;

		AddElement(new CMoveElement(Point{arValues[0], arValues[1]}));

		for (unsigned int unIndex = 2; unIndex < arValues.size(); unIndex += 2)
			AddElement(new CLineElement(Point{arValues[unIndex + 0], arValues[unIndex + 1]}));
	}

	CPolygon::CPolygon(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent)
		: CPolyline(oNode, pParent)
	{
		AddElement(new CCloseElement());
	}
}
