#ifndef _WMF_PLAYER_H
#define _WMF_PLAYER_H

#include <map>
#include <vector>
#include "WmfObjects.h"

namespace MetaFile
{
	class CWmfDC;
	class CWmfFile;

	class CWmfPlayer
	{
	public:

		CWmfPlayer(CWmfFile* pFile);
		~CWmfPlayer();
		void Clear();
		CWmfDC* SaveDC();
		CWmfDC* RestoreDC();
		CWmfDC* GetDC();
		void RegisterObject(CWmfObjectBase* pObject);
		void SelectObject(unsigned short ushIndex);
		void SelectPalette(unsigned short ushIndex);
		void DeleteObject(unsigned short ushIndex);

	private:

		typedef std::map <unsigned int, CWmfObjectBase*> CWmfObjectMap;

		CWmfDC*                     m_pDC;
		std::vector<CWmfDC*>        m_vDCStack;
		CWmfFile*                   m_pFile;
		CWmfObjectMap               m_mObjects;
		unsigned short              m_ushIndex;
		std::vector<unsigned short> m_vAvailableIndexes;
	};

	class CWmfDC
	{
	public:
		CWmfDC();
		~CWmfDC();
		CWmfDC*        Copy();
		void           SetBrush(CWmfBrush* pBrush);
		void           RemoveBrush(CWmfBrush* pBrush);
		CWmfBrush*     GetBrush();				     
		void           SetPen(CWmfPen* pPen);
		void           RemovePen(CWmfPen* pPen);
		CWmfPen*       GetPen();
		void           SetPalette(CWmfPalette* pPalette);
		void           RemovePalette(CWmfPalette* pPalette);
		CWmfPalette*   GetPalette();
		void           SetFont(CWmfFont* pFont);
		void           RemoveFont(CWmfFont* pFont);
		CWmfFont*      GetFont();
		void           SetRegion(CWmfRegion* pRegion);
		void           RemoveRegion(CWmfRegion* pRegion);
		CWmfRegion*    GetRegion();
		void           SetMapMode(unsigned short ushMapMode);
		unsigned int   GetMapMode();
		double         GetPixelWidth();
		double         GetPixelHeight();
		TWmfWindow*    GetWindow();
		void           SetWindowOrg(short shX, short shY);
		void           SetWindowExt(short shW, short shH);
		void           SetWindowOff(short shX, short shY);
		void           SetWindowScale(double dX, double dY);
		TWmfWindow*    GetViewport();
		void           SetViewportOrg(short shX, short shY);
		void           SetViewportExt(short shW, short shH);
		void           SetViewportOff(short shX, short shY);
		void           SetViewportScale(double dX, double dY);
		void           SetTextColor(TWmfColor& oColor);
		TWmfColor&     GetTextColor();
		void           SetTextBgColor(TWmfColor& oColor);
		TWmfColor&     GetTextBgColor();
		TWmfPointS&    GetCurPos();
		void           SetCurPos(TWmfPointS& oPoint);
		void           SetCurPos(short shX, short shY);
		void           SetTextBgMode(unsigned short ushMode);
		unsigned short GetTextBgMode();
		void           SetLayout(unsigned short ushLayout);
		unsigned short GetLayout();
		void           SetPolyFillMode(unsigned short ushMode);
		unsigned short GetPolyFillMode();
		void           SetRop2Mode(unsigned short ushMode);
		unsigned short GetRop2Mode();
		void           SetStretchBltMode(unsigned short ushMode);
		unsigned short GetStretchBltMode();
		void           SetTextAlign(unsigned short ushTextAlign);
		unsigned short GetTextAlign();
		void           SetCharSpacing(unsigned short ushCharSpacing);
		unsigned short GetCharSpacing();

	private:

		void           SetPixelWidth(double dW);
		void           SetPixelHeight(double dH);
		bool           UpdatePixelMetrics();

	private:

		CWmfBrush      m_oDefaultBrush;
		CWmfPen        m_oDefaultPen;
		CWmfBrush*     m_pBrush;
		CWmfPen*       m_pPen;
		CWmfPalette*   m_pPalette;
		CWmfFont*      m_pFont;
		CWmfRegion*    m_pRegion;
		unsigned short m_ushMapMode;
		double         m_dPixelWidth;
		double         m_dPixelHeight;
		TWmfWindow     m_oWindow;
		TWmfWindow     m_oViewport;
		TWmfColor      m_oTextColor;
		TWmfColor      m_oTextBgColor;
		TWmfPointS     m_oCurPos;
		unsigned short m_ushTextBgMode;
		unsigned short m_ushLayout;
		unsigned short m_ushPolyFillMode;
		unsigned short m_ushRop2Mode;
		unsigned short m_ushStretchBltMode;
		unsigned short m_ushTextAlign;
		unsigned short m_ushCharSpacing;
	};

	//class CEmfDC
	//{
	//public:

	//	CEmfDC();
	//	~CEmfDC();
	//	CEmfDC* Copy();

	//	TEmfXForm*      GetTransform();
	//	TEmfXForm*      GetInverseTransform();
	//	void            MultiplyTransform(TEmfXForm& oForm, unsigned int ulMode);
	//	void            SetMiterLimit(unsigned int ulMiter);
	//	unsigned int    GetMiterLimit();
	//	CEmfClip*       GetClip();
	//	void            ClipToPath(CEmfPath* pPath, unsigned int unMode);

	//private:

	//	TEmfXForm       m_oTransform;
	//	TEmfXForm       m_oInverseTransform;
	//	unsigned int    m_ulMiterLimit;
	//	CEmfClip        m_oClip;
	//};
}

#endif //_WMF_PLAYER_H