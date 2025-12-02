#include "CPolyline.h"

#include "../../../../../Common/3dParty/html/css/src/StaticFunctions.h"

namespace SVG
{
	CPolyline::CPolyline(CSvgReader& oReader, CRenderedObject* pParent)
		: CPath(oReader, pParent)
	{}

	void CPolyline::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("points" == sName)
		{
			const std::vector<double> arValues = NSCSS::NS_STATIC_FUNCTIONS::ReadDoubleValues(oReader.GetText());

			if (arValues.size() < 4)
				return;

			AddElement(new CMoveElement(Point{arValues[0], arValues[1]}));

			for (unsigned int unIndex = 2; unIndex < arValues.size(); unIndex += 2)
				AddElement(new CLineElement(Point{arValues[unIndex + 0], arValues[unIndex + 1]}));
		}
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	CPolygon::CPolygon(CSvgReader& oReader, CRenderedObject* pParent)
	    : CPolyline(oReader, pParent)
	{}

	void CPolygon::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		CPolyline::SetAttribute(sName, oReader);
		AddElement(new CCloseElement());
	}
}
