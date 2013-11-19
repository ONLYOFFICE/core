#ifndef _MATH_EXT_H
#define _MATH_EXT_H

#include <math.h>

static inline int round(double dValue) 
{
	return (int)floor( dValue + 0.5 );
}

static inline double distance(double dFirstX, double dFirstY, double dSecondX, double dSecondY) 
{
	double dDx = dSecondX - dFirstX;
	double dDy = dSecondY - dFirstY;
	return sqrt( dDx * dDx + dDy * dDy );
}

#endif /* _MATH_EXT_H */
