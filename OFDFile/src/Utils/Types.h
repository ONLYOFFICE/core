#ifndef TYPES_H
#define TYPES_H

#include <string>
#include "../../../DesktopEditor/xml/include/xmlutils.h"


namespace OFD
{
#define OFD_EPSILON 0.0001

struct TBox
{
	double m_dX;
	double m_dY;
	double m_dWidth;
	double m_dHeight;

	TBox();

	bool Empty() const;
	bool Read(const std::wstring& wsValue);
};

struct TColor
{
	TColor();
	TColor(XmlUtils::CXmlLiteReader& oLiteReader);

	bool Read(XmlUtils::CXmlLiteReader& oLiteReader);
};
}

#endif // TYPES_H
