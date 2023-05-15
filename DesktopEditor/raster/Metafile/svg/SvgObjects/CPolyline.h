#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "CPath.h"

namespace SVG
{
	class CPolyline : public CPath
	{
	public:
		CPolyline(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
	};

	class CPolygon : public CPolyline
	{
	public:
		CPolygon(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL);
	};
}

#endif // CPOLYLINE_H
