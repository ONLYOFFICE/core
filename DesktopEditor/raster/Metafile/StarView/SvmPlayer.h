
#ifndef CSvmPlayer_H
#define CSvmPlayer_H


#include "SvmEnums.h"
#include "SvmObjects.h"

#include <map>
#include <vector>

namespace MetaFile
{
	class CSvmFile;
	class CSvmDC;

class CSvmPlayer
{
public:
	CSvmPlayer(CSvmFile *file);
    virtual ~CSvmPlayer();
	void	Clear();
	CSvmDC* SaveDC();
	CSvmDC* RestoreDC();
	CSvmDC* GetDC();

	void RegisterObject(CSvmObjectBase* pObject);
	void SelectObject(unsigned short ushIndex);
	void SelectPalette(unsigned short ushIndex);
	void DeleteObject(unsigned short ushIndex);
private:
	void InitStockObjects();
	void InitStockBrush(bool bNull, unsigned char r, unsigned char g, unsigned char b);
	void InitStockPen(bool bNull, unsigned char r, unsigned char g, unsigned char b);

	typedef std::map < unsigned int, CSvmObjectBase* > CSvmObjectMap;

	CSvmDC*              m_pDC;
	std::vector<CSvmDC*> m_vDCStack;
	CSvmFile*            m_pSvmFile;
	CSvmObjectMap        m_mObjects;

	typedef std::map <unsigned int, CWmfObjectBase*> CWmfObjectMap;

	unsigned short              m_ushIndex;
	std::vector<unsigned short> m_vAvailableIndexes;
};

class CSvmDC
{
public:

	CSvmDC();
	~CSvmDC();
	CSvmDC* Copy();

	void            SetMapMode(MapMode &mapMode);
	TXForm*			GetTransform();
	TXForm*			GetInverseTransform();
	void            MultiplyTransform(TXForm& oForm, unsigned int ulMode);
	void            SetTextColor(TSvmColor& oColor);
	TSvmColor&      GetTextColor();
	void            SetBrush(CSvmBrush* pBrush);
	void            RemoveBrush(CSvmBrush *pBrush);
	CSvmBrush*		GetBrush();
	void            SetFont(CSvmFont* pFont);
	void            RemoveFont(CSvmFont* pFont);
	CSvmFont*		GetFont();
	void            SetTextAlign(unsigned int ulAlign);
	unsigned int    GetTextAlign();
	void            SetBgMode(unsigned int ulBgMode);
	unsigned int    GetBgMode();
	void            SetBgColor(TSvmColor& oColor);
	void			SetBgColor(TSvmColor* oColor);
	TSvmColor&      GetBgColor();
	void            SetMiterLimit(unsigned int ulMiter);
	unsigned int    GetMiterLimit();
	void            SetFillMode(unsigned int ulFillMode);
	unsigned int    GetFillMode();
	void            SetPen(CSvmPen* pPen);
	void            RemovePen(CSvmPen* pPen);
	CSvmPen*		GetPen();
	void            SetStretchMode(unsigned int& oMode);
	unsigned int    GetStretchMode();
	double          GetPixelWidth();
	double          GetPixelHeight();
	void            SetWindowOrigin(TSvmPoint& oPoint);
	void            SetWindowExtents(TSvmSize& oPoint);
	TSvmWindow*     GetWindow();
	void            SetViewportOrigin(TSvmPoint& oPoint);
	void            SetViewportExtents(TSvmSize& oPoint);
	TSvmWindow*     GetViewport();
	void            SetRop2Mode(unsigned int& nMode);
	unsigned int    GetRop2Mode();
	//void            SetPalette(CSvmLogPalette* pPalette);
	//void            RemovePalette(CSvmLogPalette* pPalette);
	//CSvmLogPalette* GetPalette();
	void            SetCurPos(TSvmPoint& oPoint);
	void            SetCurPos(int lX, int lY);
	TSvmPoint&      GetCurPos();
	//CSvmClip*       GetClip();
	//void            ClipToPath(CSvmPath* pPath, unsigned int unMode);
	void            SetArcDirection(unsigned int unDirection);
	unsigned int    GetArcDirection();

private:

	void            SetPixelWidth(double dPixelW);
	void            SetPixelHeight(double dPixelH);
	bool            UpdatePixelMetrics();

private:

	unsigned int    m_ulMapMode;
	CSvmBrush*		m_pBrush;
	CSvmPen*		m_pPen;
	CSvmFont*		m_pFont;
	//CSvmLogPalette* m_pPalette;
	TXForm			m_oTransform;
	TXForm			m_oInverseTransform;
	TSvmColor       m_oTextColor;
	TSvmColor       m_oBgColor;
	unsigned int    m_ulTextAlign;
	unsigned int    m_ulBgMode;
	unsigned int    m_ulMiterLimit;
	unsigned int    m_ulFillMode;
	unsigned int    m_ulStretchMode;
	unsigned int    m_ulRop2Mode;
	double          m_dPixelWidth;
	double          m_dPixelHeight;
	TSvmWindow      m_oWindow;
	TSvmWindow      m_oViewport;
	TSvmPoint      m_oCurPos;
//	CSvmClip        m_oClip;
	unsigned int    m_unArcDirection;
};

}

#endif

