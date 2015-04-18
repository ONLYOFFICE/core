#ifndef _EMF_PLAYER_H
#define _EMF_PLAYER_H

#include <vector>
#include <map>
#include "EmfTypes.h"
#include "EmfObjects.h"

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
		void RegisterObject(unsigned long ulIndex, CEmfObjectBase* pObject);
		void SelectObject(unsigned long ulIndex);
		void DeleteObject(unsigned long ulIndex);
		void SelectPalette(unsigned long ulIndex);

	private:

		void InitStockObjects();
		void InitStockBrush(bool bNull, unsigned char r, unsigned char g, unsigned char b, unsigned long ulIndex);
		void InitStockPen(bool bNull, unsigned char r, unsigned char g, unsigned char b, unsigned long ulIndex);

	private:

		typedef std::map < unsigned long, CEmfObjectBase* > CEmfObjectMap;

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

		void            SetMapMode(unsigned long ulMapMode);
		unsigned long   GetMapMode();
		TEmfXForm*      GetTransform();
		TEmfXForm*      GetInverseTransform();
		void            MultiplyTransform(TEmfXForm& oForm, unsigned long ulMode);
		void            SetTextColor(TEmfColor& oColor);
		TEmfColor&      GetTextColor();
		void            SetBrush(CEmfLogBrushEx* pBrush);
		void            RemoveBrush(CEmfLogBrushEx* pBrush);
		CEmfLogBrushEx* GetBrush();
		void            SetFont(CEmfLogFont* pFont);
		void            RemoveFont(CEmfLogFont* pFont);
		CEmfLogFont*    GetFont();
		void            SetTextAlign(unsigned long ulAlign);
		unsigned long   GetTextAlign();
		void            SetBgMode(unsigned long ulBgMode);
		unsigned long   GetBgMode();
		void            SetBgColor(TEmfColor& oColor);
		TEmfColor&      GetBgColor();
		void            SetMiterLimit(unsigned long ulMiter);
		unsigned long   GetMiterLimit();
		void            SetFillMode(unsigned long ulFillMode);
		unsigned long   GetFillMode();
		void            SetPen(CEmfLogPen* pPen);
		void            RemovePen(CEmfLogPen* pPen);
		CEmfLogPen*     GetPen();
		void            SetStretchMode(unsigned long& oMode);
		unsigned long   GetStretchMode();
		double          GetPixelWidth();
		double          GetPixelHeight();
		void            SetWindowOrigin(TEmfPointL& oPoint);
		void            SetWindowExtents(TEmfSizeL& oPoint);
		TEmfWindow*     GetWindow();
		void            SetViewportOrigin(TEmfPointL& oPoint);
		void            SetViewportExtents(TEmfSizeL& oPoint);
		TEmfWindow*     GetViewport();
		void            SetRop2Mode(unsigned long& nMode);
		unsigned long   GetRop2Mode();
		void            SetPalette(CEmfLogPalette* pPalette);
		void            RemovePalette(CEmfLogPalette* pPalette);
		CEmfLogPalette* GetPalette();
		void            SetCurPos(TEmfPointL& oPoint);
		void            SetCurPos(long lX, long lY);
		TEmfPointL&     GetCurPos();

	private:

		void            SetPixelWidth(double dPixelW);
		void            SetPixelHeight(double dPixelH);
		bool            UpdatePixelMetrics();

	private:

		CEmfLogPen      m_oDefaultPen;
		CEmfLogBrushEx  m_oDefaultBrush;
		unsigned long   m_ulMapMode;
		CEmfLogBrushEx* m_pBrush;
		CEmfLogPen*     m_pPen;
		CEmfLogFont*    m_pFont;
		CEmfLogPalette* m_pPalette;
		TEmfXForm       m_oTransform;
		TEmfXForm       m_oInverseTransform;
		TEmfColor       m_oTextColor;
		TEmfColor       m_oBgColor;
		unsigned long   m_ulTextAlign;
		unsigned long   m_ulBgMode;
		unsigned long   m_ulMiterLimit;
		unsigned long   m_ulFillMode;
		unsigned long   m_ulStretchMode;
		unsigned long   m_ulRop2Mode;
		double          m_dPixelWidth;
		double          m_dPixelHeight;
		TEmfWindow      m_oWindow;
		TEmfWindow      m_oViewport;
		TEmfPointL      m_oCurPos;
	};
}

#endif //_EMF_PLAYER_H