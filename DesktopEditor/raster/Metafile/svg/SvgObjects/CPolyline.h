#ifndef CPOLYLINE_H
#define CPOLYLINE_H

#include "CPath.h"

namespace SVG
{
	class CPolyline : public CPath
	{
		friend class CObject;
	protected:
		CPolyline(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;
	};

	class CPolygon : public CPolyline
	{
		friend class CObject;
		CPolygon(CSvgReader& oReader, CRenderedObject* pParent = NULL);
	public:
		void SetAttribute(const std::string& sName, CSvgReader& oReader) override;
	};
}

#endif // CPOLYLINE_H
