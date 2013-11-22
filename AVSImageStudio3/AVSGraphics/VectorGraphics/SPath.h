#ifndef _SPATH_H
#define _SPATH_H

//-------------------------------------------------------------------------------------------------------------------------------
// SPathPoint
//-------------------------------------------------------------------------------------------------------------------------------

struct SPathPoint 
{
	double dX;
	double dY;
};

//-------------------------------------------------------------------------------------------------------------------------------
// SPath.nFlags
//-------------------------------------------------------------------------------------------------------------------------------

// Флаг для первой точки любого subpath
#define SPathFirst         0x01

// Флаг для последней точки любого subpath
#define SPathLast          0x02

// Если subpath замкнутый, тогда его последняя и первая точка должны совпадать, их флаг в данном случае следующий 
#define SPathClosed        0x04

// Флаг, означающий, что данная точка является контрольной для кривой Безье
#define SPathCurve         0x08

//-------------------------------------------------------------------------------------------------------------------------------
// SPathHint
//-------------------------------------------------------------------------------------------------------------------------------

struct SPathHint 
{
	int nFirstControl;
	int nSecondControl;
	int nFirstPoint;
	int nLastPoint;
};

//-------------------------------------------------------------------------------------------------------------------------------
// SPath
//-------------------------------------------------------------------------------------------------------------------------------

class SPath 
{
public:

	SPath();

	SPath *Сopy() 
	{ 
		return new SPath(this); 
	}

	SPath& operator=(const SPath& oPath);

	~SPath();

	void Append(SPath *pPath);

	int MoveTo(double dX, double dY);

	int LineTo(double dX, double dY);

	int CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);

	int Close();

	void Offset(double dDx, double dDy);

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


protected:

	SPath(SPath *pPath);
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

protected:

	SPathPoint    *m_pPoints;      // Массив точек
	unsigned char *m_pFlags;       // Массив флагов, указыающих значение точки в SubPath
	int            m_nPointsCount; // Количество точек 
	int            m_nSize;        // Непосредственный размер массива
	
	int            m_nCurSubpath;  // Номер первой точки последнего SubPath

	SPathHint     *m_pHints;       //
	int            m_nHintsCount;
	int            m_nHintsSize;

	friend class   SXPath;
	friend class   SImage;
	friend class   CAVSGraphics;
};

#endif /* _SPATH_H */
