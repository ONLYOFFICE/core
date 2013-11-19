#ifndef _SCLIP_H
#define _SCLIP_H

#include "STypes.h"
#include "SMathExt.h"

class SPath;
class SXPath;
class SXPathScanner;
class SBitmap;

//-------------------------------------------------------------------------------------------------------------------------------

enum SClipResult 
{
	clipAllInside,
	clipAllOutside,
	clipPartial
};

//-------------------------------------------------------------------------------------------------------------------------------
// SClip
//-------------------------------------------------------------------------------------------------------------------------------

class SClip
{
public:

	SClip(double dMinX, double dMinY, double dMaxX, double dMaxY, BOOL bAntialias);

	SClip *Copy() 
	{ 
		return new SClip(this); 
	}

	~SClip();

	void ResetToRect(double dX0, double dY0, double dX1, double dY1);

	// Пересекаем Clip с заданным прямоугольником.
	int ClipToRect(double dX0, double dY0, double dX1, double dY1);

	// Пересекаем Clip с <pPath>.
	int ClipToPath(SPath *pPath, double *pMatrix, double dFlatness, BOOL bEO);

	// Возвращаем TRUE, если (<dX>,<dY>) лежит внутри Clip.
	BOOL IsInsideClip(int dX, int dY);

	// Проверяем расположение заданного прямоугольника относительно Clip. Возвращаемые значения:
	//   - clipAllInside
	//     Если весь прямоугольник содержится внутри Clipping Region, т.е. все точки прямоугольника видимые.
	//   - clipAllOutside
	//     Если прямугольник не имеет пересечений с Clipping Region, т.е. все точки невидимые
	//   - clipPartial
	//     Если прямоугольник частично лежит внутри и частично снаружи относительно Clipping Region
	SClipResult CheckRectangle(int dRectMinX, int dRectMinY, int dRectMaxX, int dRectMaxY);

	// Аналогичная функция, только относящаяся к горизонтальному промежутку.
	SClipResult CheckSpan(int dSpanMinX, int dSpanMaxX, int nSpanY);

	// Обрезаем АА-линию, обнуляя значения пикселей. На входе, все ненулевые пиксели находятся внутри промежутка [<nX0>,<nX1>]. 
	// Функция обновляет значения переменных <nX0> и <nX1>.
	void ClipAALine(SBitmap *pAABuffer, int *pnX0, int *pnX1, int nY);

	int GetMinX() 
	{ 
		return m_nMinX; 
	}
	int GetMaxX() 
	{ 
		return m_nMaxX; 
	}
	int GetMinY() 
	{ 
		return m_nMinY; 
	}
	int GetMaxY() 
	{ 
		return m_nMaxY; 
	}

	int GetPathsCount() 
	{ 
		return m_nPathsCount; 
	}

private:

	SClip(SClip *pClip);
	void Resize(int nPathsCount);

private:

	BOOL            m_bAntiAlias;
  
	double          m_dMinX;
	double          m_dMinY;
	double          m_dMaxX;
	double          m_dMaxY;

	int             m_nMinX;
	int             m_nMinY;
	int             m_nMaxX;
	int             m_nMaxY;

	SXPath        **m_ppPaths;
	unsigned char  *m_pFlags;
	SXPathScanner **m_pScanners;
	int             m_nPathsCount;
	int             m_nSize;
};

#endif /* _SCLIP_H */
