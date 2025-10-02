#ifndef GRAPHICUNIT_H
#define GRAPHICUNIT_H

#include "../Types/PenSettings.h"
#include "../Utils/Types.h"

namespace OFD
{
class CGraphicUnit
{
	TBox m_oBoundary;
	std::wstring m_wsName;
	bool m_bVisible;
	TMatrix m_oCTM;
	unsigned int m_unDrawParam;
	CPenSettings m_oPenSettings;
public:
	CGraphicUnit(CXmlReader& oLiteReader);

	void Apply(IRenderer* pRenderer, TMatrix& oOldTransform) const;

	TBox GetBoundary() const;
};
}

#endif // GRAPHICUNIT_H
