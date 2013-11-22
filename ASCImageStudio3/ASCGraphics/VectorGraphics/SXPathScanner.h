#ifndef _SXPATH_SCANNER_H
#define _SXPATH_SCANNER_H

#include "STypes.h"

class SXPath;
class SBitmap;
struct SIntersect;
class CPixels;

//-------------------------------------------------------------------------------------------------------------------------------
// SXPathScanner
//-------------------------------------------------------------------------------------------------------------------------------

class SXPathScanner 
{
public:

	// <pXPath> должен быть отсортирован.
	SXPathScanner(SXPath *pXPath, BOOL bEO);

	~SXPathScanner();

	// Возвращаем BBox для Path.
	void GetBBox(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY)
    { 
		*pnMinX = m_nMinX; 
		*pnMinY = m_nMinY; 
		*pnMaxX = m_nMaxX; 
		*pnMaxY = m_nMaxY; 
	}

	// Возвращаем BBox для Path с учетом AntiAliasing.
	void GetBBoxAA(int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY);

	// Возвращаем минимальное и максимальное значения X для промежутка Y.
	void GetSpanBounds(int nY, int *pnSpanMinX, int *pnSpanMaxX);

	// True, если точка (<x>,<y>) лежит внутри Path.
	BOOL IsInsidePath(int nX, int nY);

	// True, если весь интервал ([<nX0>,<nX1>], <nY>) лежит внутри Path.
	BOOL IsInsidePath(int x0, int x1, int y);

	// Возвращаем следующий промежуто, лежащий внутри Path для <nY>. Если <nY> отличается, по сравению с предыдущим вызовом функции
	// GetNextSpan, тогда возвращаем первый промежуток для данного <nY>. Возвращаем FALSE, если нет больше промежутков для <nY>.
	BOOL GetNextSpan(int nY, int *pnX0, int *pnX1);

	// Помещаем одну AA-линию в <pAABuffer>. Функция возвращает минимальное и максимальное значение координат пикселей с ненулевыми
	// значениями.
	void RenderAALine(SBitmap *pAABuffer, int *pnX0, int *pnX1, int nY);
	void RenderAALine(CPixels *pAABuffer, int *pnX0, int *pnX1, int nY);

	// Обрезаем АА-линию, обнуляя значения пикселей. На входе, все ненулевые пиксели находятся внутри промежутка [<nX0>,<nX1>]. 
	// Функция обновляет значения переменных <nX0> и <nX1>.
	void ClipAALine(SBitmap *pAABuffer, int *pnX0, int *pnX1, int nY);
	void ClipAALine(CPixels *pAABuffer, int *pnX0, int *pnX1, int nY);

private:

	void ComputeIntersections(int nY);

private:

	SXPath     *m_pXPath; 
	BOOL        m_bEO;
	int         m_nMinX;       //
	int         m_nMinY;       // BBox для XPath
	int         m_nMaxX;       //
	int         m_nMaxY;       //

	int         m_nInterY;     // Текущее значение Y
	int         m_nInterIndex; // Текущий номер в <inter> - используемый в GetNextSpan 
	int         m_nInterCount; // Текущий EO/NZWN счетчик - используемый в GetNextSpan 
	int         m_nXPathIndex; // Текущий номер в <m_pXPath> - использвуемый в ComputeIntersections
	SIntersect *m_pInter;      // Массив пересечений для <m_nInterY>
	int         m_nInterLen;   // Количество элементов в массиве <m_pInter>
	int         m_nInterSize;  // Размер массива <inter>
};

#endif /* _SXPATH_SCANNER_H */
