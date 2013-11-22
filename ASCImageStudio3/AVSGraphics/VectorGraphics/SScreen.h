#ifndef _SSCREEN_H
#define _SSCREEN_H

#include "STypes.h"

struct SScreenParams;

//-------------------------------------------------------------------------------------------------------------------------------
// SScreen
//-------------------------------------------------------------------------------------------------------------------------------

class SScreen 
{
public:

	SScreen(SScreenParams *pParams = NULL);
	SScreen(SScreen *pScreen);
	~SScreen();

	SScreen *Copy() 
	{ 
		return new SScreen(this); 
	}

	// Смотрим значение пикселя в точке (nX, nY) (в сером цвете).
	int GetGrayPixel(int nX, int nY, unsigned char unValue);

	// Функция возвращает TRUE, если значение unValue либо больше m_unMaxValue, либо меньше m_unMinValue.
	BOOL IsStatic(unsigned char unValue);

private:

	void BuildDispersedMatrix(int nX, int nY, int nValue, int nDelta, int nOffset);
	void BuildClusteredMatrix();
	void BuildSCDMatrix(int nRadius);
	int  Distance(int nX0, int nY0, int nX1, int nY1);

private:

	unsigned char *m_pMatrix;    // Threshold matrix
	int            m_nSize;      // Размер Threshold matrix
	unsigned char  m_unMinValue; // Любой пиксель со значением меньше, чем m_unMinValue считается черным
	unsigned char  m_unMaxValue; // Любой пиксель со значением больше, чем m_unMaxValue считается белым
};

#endif /* _SSCREEN_H */
