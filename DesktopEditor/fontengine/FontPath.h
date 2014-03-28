#ifndef _PATH_H
#define _PATH_H

#include "../common/Types.h"
#include <memory>
#include <string.h>
#include <stdlib.h>
#include "ftimage.h"

class ISimpleGraphicsPath
{
public:
	virtual bool _MoveTo(double x, double y)													= 0;
	virtual bool _LineTo(double x, double y)													= 0;
	virtual bool _CurveTo(double x1, double y1, double x2, double y2, double x3, double y3)		= 0;
	virtual bool _Close()																		= 0;
};

//-------------------------------------------------------------------------------------------------------------------------------
// TPathPoint
//-------------------------------------------------------------------------------------------------------------------------------

struct TPathPoint 
{
	double dX;
	double dY;
};

//-------------------------------------------------------------------------------------------------------------------------------
// CPath.nFlags
//-------------------------------------------------------------------------------------------------------------------------------

// Флаг для первой точки любого subpath
#define PathFirst         0x01

// Флаг для последней точки любого subpath
#define PathLast          0x02

// Если subpath замкнутый, тогда его последняя и первая точка должны совпадать, их флаг в данном случае следующий 
#define PathClosed        0x04

// Флаг, означающий, что данная точка является контрольной для кривой Безье
#define PathCurve         0x08

//-------------------------------------------------------------------------------------------------------------------------------
// TPathHint
//-------------------------------------------------------------------------------------------------------------------------------

struct TPathHint 
{
	int nFirstControl;
	int nSecondControl;
	int nFirstPoint;
	int nLastPoint;
};

//-------------------------------------------------------------------------------------------------------------------------------
// CFontPath
//-------------------------------------------------------------------------------------------------------------------------------

class CFontPath 
{
public:

	CFontPath();
	~CFontPath();

	void Append(CFontPath *pPath);

	int MoveTo(double dX, double dY);

	int LineTo(double dX, double dY);

	int CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);

	int Close();

	void Offset(double dDx, double dDy);

	void Reverse();

	void ToMM(double dHorDpi, double dVerDpi);

	int GetCount() 
	{ 
		return m_nPointsCount; 
	}
	void GetPoint(int nIndex, double *pdX, double *pdY, unsigned char *punFlag)
    { 
		*pdX = m_pPoints[nIndex].dX; 
		*pdY = m_pPoints[nIndex].dY; 
		*punFlag = m_pFlags[nIndex]; 
	}

	BOOL GetCurPoint(double *pdX, double *pdY);

	// Добавляем флаг StrokeAdjust.
	void AddStrokeAdjustHint(int nFirstControl, int nSecondControl, int nFirstPoint, int nLastPoint);
	
	BOOL ToInterface(ISimpleGraphicsPath* pPath);

private:

	CFontPath(CFontPath *pPath);
	void Resize(int nPointsCount);
	BOOL NoCurrentPoint() 
	{ 
		return m_nCurSubpath == m_nPointsCount; 
	}
	BOOL OnePointSubpath() 
	{ 
		return m_nCurSubpath == m_nPointsCount - 1; 
	}
	BOOL OpenSubpath() 
	{ 
		return m_nCurSubpath < m_nPointsCount - 1; 
	}

private:

	TPathPoint    *m_pPoints;      // Массив точек
	unsigned char *m_pFlags;       // Массив флагов, указыающих значение точки в SubPath
	int            m_nPointsCount; // Количество точек 
	int            m_nSize;        // Непосредственный размер массива
	
	int            m_nCurSubpath;  // Номер первой точки последнего SubPath

	TPathHint     *m_pHints;       //
	int            m_nHintsCount;
	int            m_nHintsSize;   
};

#endif /* _PATH_H */
