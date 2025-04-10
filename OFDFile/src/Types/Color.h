#ifndef COLOR_H
#define COLOR_H

#include "../Utils/XmlReader.h"
#include "../Res.h"

namespace OFD
{
class CColor
{
	BYTE m_oValues[4];

	int m_nIndex;
	BYTE m_chAlpha;

	unsigned int m_unColodSpaceID;
	// Pattern
	// AxialShd
	// RadialShd
	// GouraudShd
	// LaGouraudhd
public:
	CColor(CXmlReader& oXmlReader);

	int ToInt(const CRes* pPublicRes) const;
	BYTE GetAlpha() const;
};
}

#endif // COLOR_H
