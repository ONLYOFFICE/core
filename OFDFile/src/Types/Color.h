#ifndef COLOR_H
#define COLOR_H

#include "../Utils/XmlReader.h"

namespace OFD
{
class CColor
{
	struct TColorChannels
	{
		unsigned char m_chRed;
		BYTE m_chGreen;
		BYTE m_chBlue;
	} m_oValue;

	int m_nIndex;
	unsigned int m_unColorSpace;
	BYTE m_chAlpha;
	// Pattern
	// AxialShd
	// RadialShd
	// GouraudShd
	// LaGouraudhd
public:
	CColor(CXmlReader& oXmlReader);

	bool Read(CXmlReader& oXmlReader);
};
}

#endif // COLOR_H
