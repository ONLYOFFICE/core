#pragma once

#include "SvmEnums.h"
#include "SvmObjects.h"
#include "SvmClip.h"

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

	void RegisterObject	(CSvmObjectBase* pObject);
	void SelectObject	(unsigned short ushIndex);
	void DeleteObject	(unsigned short ushIndex);
	
	CSvmObjectBase *GetLastObject (ESvmObjectType type);

	void Pop();
	void Push(int Flags);

	void SetRasterOp(int op);

private:
	void InitStockObjects();
	void InitStockBrush	(bool bNull, unsigned char r, unsigned char g, unsigned char b);
	void InitStockPen	(bool bNull, unsigned char r, unsigned char g, unsigned char b);

	typedef std::map < unsigned int, CSvmObjectBase* > CSvmObjectMap;

	CSvmDC*					m_pDC;
	std::vector<CSvmDC*>	m_vDCStack;
	CSvmFile*				m_pFile;
	CSvmObjectMap			m_mObjects;
	int						m_nFlags;
	ESvnRasterOp			m_eRasterOp;

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

	ESvmMapUnit		GetMapMode();
	void            SetMapMode(MapMode &mapMode, bool prefered = false);
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
	CSvmClip*       GetClip();
	//void            ClipToPath(CSvmPath* pPath, unsigned int unMode);
	void            SetArcDirection(unsigned int unDirection);
	unsigned int    GetArcDirection();

	void SetWindowOff(short shX, short shY)
	{
		m_oWindow.lX += shX;
		m_oWindow.lY += shY;
		UpdatePixelMetrics();
	}
	void SetWindowScale(double dX, double dY)
	{
		m_oWindow.ulW = (int)(m_oWindow.ulW * dX);
		m_oWindow.ulH = (int)(m_oWindow.ulH * dY);
		UpdatePixelMetrics();
	}
	void SetWindowExt(short shW, short shH)
	{
		m_oWindow.ulW = shW;
		m_oWindow.ulH = shH;
		UpdatePixelMetrics();
	}
	
	double          m_dPixelWidth;
	double          m_dPixelHeight;
	
	double          m_dPixelWidthPrefered;
	double          m_dPixelHeightPrefered;

private:

	void	SetPixelWidth(double dPixelW);
	void	SetPixelHeight(double dPixelH);
	bool	UpdatePixelMetrics();

	ESvmMapUnit     m_ulMapMode;
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
	
	TSvmWindow      m_oWindow;
	TSvmWindow      m_oViewport;
	
	TSvmPoint		m_oCurPos;
	//CSvmClip        m_oClip;
	unsigned int    m_unArcDirection;
};

}