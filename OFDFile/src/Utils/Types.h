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

struct TMatrix
{
	double m_dM11;
	double m_dM12;
	double m_dM21;
	double m_dM22;
	double m_dDx;
	double m_dDy;

	TMatrix();

	bool Read(const std::string& sValue);
};
}

#endif // TYPES_H
