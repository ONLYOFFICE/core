#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "CPath.h"

namespace SVG
{
	class CPolyline : public CPath
	{
	public:
		CPolyline(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL);
	};

	class CPolygon : public CPolyline
	{
	public:
		CPolygon(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent = NULL);
	};
}

#endif // CPOLYLINE_H
