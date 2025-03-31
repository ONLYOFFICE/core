#ifndef GRAPHICUNIT_H
#define GRAPHICUNIT_H

#include "../Utils/XmlReader.h"
#include "../Utils/Types.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"

#include <vector>

namespace OFD
{
class CGraphicUnit
{
	TBox m_oBoundary;
	std::wstring m_wsName;
	bool m_bVisible;
	TMatrix m_oCTM;
	unsigned int m_unDrawParam;
	double m_dLineWidth;

	enum class ECap
	{
		Butt,
		Round,
		Square
	} m_eCap;

	enum class EJoin
	{
		Miter,
		Round,
		Bevel
	} m_eJoin;

	double m_dMiterLimit;
	double m_dDashOffset;
	std::vector<double> m_arDashPattern;
	unsigned char m_uchAlpha;

	friend class CPathObject;
public:
	CGraphicUnit(CXmlReader& oLiteReader);

	void Apply(IRenderer* pRenderer) const;

	TBox GetBoundary() const;
};
}

#endif // GRAPHICUNIT_H
