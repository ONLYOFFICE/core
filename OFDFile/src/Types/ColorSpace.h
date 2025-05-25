#ifndef COLORSPACE_H
#define COLORSPACE_H

#include "../IOFDElement.h"

namespace OFD
{
class CColorSpace : public IOFDElement
{
public:
	CColorSpace(CXmlReader& oXmlReader);

	enum class EType
	{
		GRAY,
		RGB,
		CMYK
	};

	EType GetType() const;
private:
	EType m_eType;
};
}
#endif // COLORSPACE_H
