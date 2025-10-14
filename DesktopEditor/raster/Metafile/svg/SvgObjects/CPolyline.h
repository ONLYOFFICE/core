#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "CPath.h"

namespace SVG
{
	class CPolyline : public CPath
	{
		friend class CRenderedObject;
	protected:
		CPolyline(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;
	};

	class CPolygon : public CPolyline
	{
		friend class CRenderedObject;
		CPolygon(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;
	};
}

#endif // CPOLYLINE_H
