#ifndef DRAWPARAM_H
#define DRAWPARAM_H

#include "../IOFDElement.h"
#include "PenSettings.h"
#include "Color.h"

namespace OFD
{
class CDrawParam : public IOFDElement
{
	CPenSettings m_oPenSettings;

	CColor *m_pStrokeColor;
	CColor *m_pFillColor;
public:
	CDrawParam(CXmlReader& oXmlReader);
	~CDrawParam();

	bool ApplyStrokeColor(IRenderer* pRenderer, const CRes* pPublicRes) const;
	bool ApplyFillColor(IRenderer* pRenderer, const CRes* pPublicRes) const;
};
}
#endif // DRAWPARAM_H
