#ifndef _SXPATH_H
#define _SXPATH_H

#include "STypes.h"

class SPath;
struct SXPathAdjust;

//-------------------------------------------------------------------------------------------------------------------------------

#define MaxCurveSplits (1 << 10)

//-------------------------------------------------------------------------------------------------------------------------------
// SXPathSegment
//-------------------------------------------------------------------------------------------------------------------------------

struct SXPathSegment 
{
	double       dFirstX;  // Координаты первой точки
	double       dFirstY;  //
	double       dSecondX; // Координаты второй точки
	double       dSecondY; //
	double       dDxDy;    // тангес угла наклона: Dx / Dy 
	double       dDyDx;    // тангес угла наклона: Dy / Dx
	unsigned int unFlags;
};

#define XPathFirst   0x01	// Первый сегмент SubPath
#define XPathLast    0x02	// Последний сегмент SubPath
#define XPathEnd0    0x04	// Первая точка(окончания) открытого SubPath
#define XPathEnd1    0x08 // Вторая точка(окончания) открытого SubPath
#define XPathHoriz   0x10 // Горизонтальный сегмент (dSecondY == dSecondX) (dDxDy неопределено)
#define XPathVert    0x20 // Вертикальный сегмент (dFirstY == dFirstX) (dDyDx неопределено)
#define XPathFlip	   0x40	// Перевернутый сегмент dFirstY > dSecondY

//-------------------------------------------------------------------------------------------------------------------------------
// SXPath
//-------------------------------------------------------------------------------------------------------------------------------

class SXPath 
{
public:

	// Расширяем и сглаживаем <pPath>. Переводим все из пользовательских координат в координаты устройста с помощью матрицы <pMatrix>.
	// Если <bCloseSubpaths> == true, закрываем все открытые SubPaths.
	SXPath(SPath *pPath, double *pMatrix, double dFlatness, BOOL bCloseSubpaths);

	SXPath *Copy() 
	{ 
		return new SXPath(this); 
	}

	~SXPath();

	// Умножаем все координаты на AntiAliasingSize.
	void AntiAliasingScale();

	void Sort();

private:

	SXPath();
	SXPath(SXPath *pXPath);
	void Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY);
	void StrokeAdjust(SXPathAdjust *pAdjust, double *pdX, double *pdY);
	void Resize(int nSegmentsCount);
	void AddCurve(double dX0, double dY0, double dX1, double dY1, double dX2, double dY2, double dX3, double dY3, double dFlatness, BOOL bFirst, BOOL bLast, BOOL bEnd0, BOOL bEnd1);
	void AddSegment(double dX0, double dY0, double dX1, double dY1, BOOL bFirst, BOOL bLast, BOOL bEnd0, BOOL bEnd1);

private:

	SXPathSegment *m_pSegments;      // Список сегментов
	int            m_nSegmentsCount; // Количество сегментов
	int            m_nSize;          // Размер массива m_pSegments

	friend class SXPathScanner;
	friend class SClip;
	friend class SImage;
	friend class CAVSGraphics;
};

#endif /* _SXPATH_H */
