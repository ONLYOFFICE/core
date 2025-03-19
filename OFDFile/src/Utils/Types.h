#ifndef TYPES_H
#define TYPES_H

#include <string>

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
	bool Read(const std::string& wsValue);
};
}

#endif // TYPES_H
