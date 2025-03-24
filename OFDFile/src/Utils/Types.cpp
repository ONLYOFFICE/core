#include "Types.h"

#include "Utils.h"

namespace OFD
{
TBox::TBox()
	: m_dX(0.), m_dY(0.), m_dWidth(0.), m_dHeight(0.)
{}

bool TBox::Empty() const
{
	return m_dWidth < OFD_EPSILON || m_dHeight < OFD_EPSILON;
}

bool TBox::Read(const std::string& sValue)
{
	const std::vector<std::string> arValues{Split(sValue, ' ')};

	if (4 > arValues.size())
		return false;

	if (!StringToDouble(arValues[0], m_dX)     ||
	    !StringToDouble(arValues[1], m_dY)     ||
	    !StringToDouble(arValues[2], m_dWidth) ||
	    !StringToDouble(arValues[3], m_dHeight))
		return false;

	return true;
}

TMatrix::TMatrix()
	: m_dM11(1.), m_dM12(0.), m_dM21(0.), m_dM22(1.), m_dDx(0.), m_dDy(0.)
{}

bool TMatrix::Read(const std::string& sValue)
{
	const std::vector<std::string> arValues{Split(sValue, ' ')};

	if (6 > arValues.size())
		return false;

	if (!StringToDouble(arValues[0], m_dM11) ||
	    !StringToDouble(arValues[1], m_dM12) ||
	    !StringToDouble(arValues[2], m_dM21) ||
	    !StringToDouble(arValues[3], m_dM22) ||
	    !StringToDouble(arValues[4], m_dDx)  ||
	    !StringToDouble(arValues[5], m_dDy))
		return false;

	return true;
}

}
