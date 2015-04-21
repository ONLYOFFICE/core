#ifndef _EMF_PLAYER_H
#define _EMF_PLAYER_H

#include <vector>
#include <map>
#include "EmfTypes.h"
#include "EmfObjects.h"
#include "EmfClip.h"

namespace MetaFile
{
	class CEmfFile;
	class CEmfDC;

	class CEmfPlayer
	{
	public:

		CEmfPlayer(CEmfFile* pFile);
		~CEmfPlayer();
		void Clear();
		CEmfDC* SaveDC();
		CEmfDC* RestoreDC();
		CEmfDC* GetDC();
		void RegisterObject(unsigned int ulIndex, CEmfObjectBase* pObject);
		void SelectObject(unsigned int ulIndex);
		void DeleteObject(unsigned int ulIndex);
		void SelectPalette(unsigned int ulIndex);

	private:

		void InitStockObjects();
		void InitStockBrush(bool bNull, unsigned char r, unsigned char g, unsigned char b, unsigned int ulIndex);
		void InitStockPen(bool bNull, unsigned char r, unsigned char g, unsigned char b, unsigned int ulIndex);

	private:

		typedef std::map < unsigned int, CEmfObjectBase* > CEmfObjectMap;

		CEmfDC*              m_pDC;
		std::vector<CEmfDC*> m_vDCStack;
		CEmfFile*            m_pEmfFile;		
		CEmfObjectMap        m_mObjects;
	};

	class CEmfDC
	{
	public:

		CEmfDC();
		~CEmfDC();
		CEmfDC* Copy();

		void            SetMapMode(unsigned int ulMapMode);
		unsigned int   GetMapMode();
		TEmfXForm*      GetTransform();
		TEmfXForm*      GetInverseTransform();
		void            MultiplyTransform(TEmfXForm& oForm, unsigned int ulMode);
		void            SetTextColor(TEmfColor& oColor);
		TEmfColor&      GetTextColor();
		void            SetBrush(CEmfLogBrushEx* pBrush);
		void            RemoveBrush(CEmfLogBrushEx* pBrush);
		CEmfLogBrushEx* GetBrush();
		void            SetFont(CEmfLogFont* pFont);
		void            RemoveFont(CEmfLogFont* pFont);
		CEmfLogFont*    GetFont();
		void            SetTextAlign(unsigned int ulAlign);
		unsigned int   GetTextAlign();
		void            SetBgMode(unsigned int ulBgMode);
		unsigned int   GetBgMode();
		void            SetBgColor(TEmfColor& oColor);
		TEmfColor&      GetBgColor();
		void            SetMiterLimit(unsigned int ulMiter);
		unsigned int   GetMiterLimit();
		void            SetFillMode(unsigned int ulFillMode);
		unsigned int   GetFillMode();
		void            SetPen(CEmfLogPen* pPen);
		void            RemovePen(CEmfLogPen* pPen);
		CEmfLogPen*     GetPen();
		void            SetStretchMode(unsigned int& oMode);
		unsigned int   GetStretchMode();
		double          GetPixelWidth();
		double          GetPixelHeight();
		void            SetWindowOrigin(TEmfPointL& oPoint);
		void            SetWindowExtents(TEmfSizeL& oPoint);
		TEmfWindow*     GetWindow();
		void            SetViewportOrigin(TEmfPointL& oPoint);
		void            SetViewportExtents(TEmfSizeL& oPoint);
		TEmfWindow*     GetViewport();
		void            SetRop2Mode(unsigned int& nMode);
		unsigned int   GetRop2Mode();
		void            SetPalette(CEmfLogPalette* pPalette);
		void            RemovePalette(CEmfLogPalette* pPalette);
		CEmfLogPalette* GetPalette();
		void            SetCurPos(TEmfPointL& oPoint);
		void            SetCurPos(int lX, int lY);
		TEmfPointL&     GetCurPos();
		CEmfClip*       GetClip();
		void            ClipToPath();

	private:

		void            SetPixelWidth(double dPixelW);
		void            SetPixelHeight(double dPixelH);
		bool            UpdatePixelMetrics();

	private:

		CEmfLogPen      m_oDefaultPen;
		CEmfLogBrushEx  m_oDefaultBrush;
		unsigned int   m_ulMapMode;
		CEmfLogBrushEx* m_pBrush;
		CEmfLogPen*     m_pPen;
		CEmfLogFont*    m_pFont;
		CEmfLogPalette* m_pPalette;
		TEmfXForm       m_oTransform;
		TEmfXForm       m_oInverseTransform;
		TEmfColor       m_oTextColor;
		TEmfColor       m_oBgColor;
		unsigned int   m_ulTextAlign;
		unsigned int   m_ulBgMode;
		unsigned int   m_ulMiterLimit;
		unsigned int   m_ulFillMode;
		unsigned int   m_ulStretchMode;
		unsigned int   m_ulRop2Mode;
		double          m_dPixelWidth;
		double          m_dPixelHeight;
		TEmfWindow      m_oWindow;
		TEmfWindow      m_oViewport;
		TEmfPointL      m_oCurPos;
		CEmfClip        m_oClip;
	};
}

#endif //_EMF_PLAYER_H