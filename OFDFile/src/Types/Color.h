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

	const CColorSpace* m_pColorSpace;
	// Pattern
	// AxialShd
	// RadialShd
	// GouraudShd
	// LaGouraudhd
public:
	CColor(CXmlReader& oXmlReader, const CRes* pPublicRes);

	int ToInt() const;
	BYTE GetAlpha() const;
};
}

#endif // COLOR_H
