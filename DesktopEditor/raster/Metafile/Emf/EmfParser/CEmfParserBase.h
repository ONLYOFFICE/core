#ifndef CEMFPARSERBASE_H
#define CEMFPARSERBASE_H

//#include "../EmfTypes.h"
//#include "../EmfObjects.h"
//#include "../../Common/MetaFileUtils.h"

#include "../EmfPlayer.h"
#include "../EmfPath.h"

#include "../../Common/MetaFile.h"

#include "../EmfInterpretator/CEmfInterpretatorBase.h"

#ifdef _DEBUG
#include <iostream>
#include  <algorithm>
#include <cstdlib>
#endif

#if defined(DrawText)
#undef DrawText
#endif

namespace MetaFile
{
	enum EmfParserType
	{
		EmfParser,
		EmfPlusParser,
		EmfxParser
	};

	typedef enum
	{
		EMR_COMMENT_WINDOWS_METAFILE = 0x80000001,
		EMR_COMMENT_BEGINGROUP       = 0x00000002,
		EMR_COMMENT_ENDGROUP         = 0x00000003,
		EMR_COMMENT_MULTIFORMATS     = 0x40000004,
		EMR_COMMENT_UNICODE_STRING   = 0x00000040,
		EMR_COMMENT_UNICODE_END      = 0x00000080
	} EmrComment;

	class CEmfPlayer;

	class  CEmfParserBase : public virtual IMetaFileBase
	{
	public:
		CEmfParserBase();
		~CEmfParserBase();

		virtual bool            ReadFromBuffer(BYTE* pBuffer, unsigned int unSize, const bool& bIsExternal = true) = 0;
		virtual bool            OpenFromFile(const wchar_t*)    = 0;

		virtual void            PlayFile()                      = 0;
		virtual void            Scan()                          = 0;

		virtual EmfParserType   GetType()                       = 0;

		void            PlayMetaFile()                   override;
		void            ClearFile()                      override;
		TRect*          GetDCBounds()                    override;
		double          GetPixelHeight()                 override;
		double          GetPixelWidth()                  override;
		int             GetTextColor()                   override;
		IFont*          GetFont()                        override;
		IBrush*         GetBrush()                       override;
		IPen*           GetPen()                         override;
		unsigned int    GetTextAlign()                   override;
		unsigned int    GetTextBgMode()                  override;
		int             GetTextBgColor()                 override;
		unsigned int    GetFillMode()                    override;
		TPointD         GetCurPos()                      override;
		TXForm*         GetInverseTransform()            override;
		TXForm*         GetTransform(int = GM_ADVANCED)  override;
		unsigned int    GetMiterLimit()                  override;
		unsigned int    GetRop2Mode()                    override;
		IClip*          GetClip()                        override;
		int             GetCharSpace()                   override;
		bool            IsWindowFlippedY()               override;
		bool            IsWindowFlippedX()               override;
		unsigned int    GetMapMode()                     override;
		double          GetDpi()                         override;
		IRegion*        GetRegion()                      override;
		unsigned int    GetArcDirection()                override;
		bool            IsViewportFlippedY();
		bool            IsViewportFlippedX();

		virtual void    SetInterpretator(IOutputDevice* pOutput);
		void            SetInterpretator(const wchar_t *wsFilePath, InterpretatorType oInterpretatorType, unsigned int unWidth = 0, unsigned int unHeight = 0);
		void            SetInterpretator(IOutputDevice* pOutput, const wchar_t *wsFilePath);
		void            SetInterpretator(InterpretatorType oInterpretatorType, double dWidth = 0, double dHeight = 0);

		CEmfInterpretatorBase* GetInterpretator();

		CEmfDC*     GetDC();
		TEmfRectL*  GetBounds();
		CEmfPath*	GetPath() const;
	private:
		//Работа с изображениями
		void ImageProcessing(const TEmfAlphaBlend       &oTEmfAlphaBlend);
		void ImageProcessing(const TEmfStretchDIBITS    &oTEmfStretchDIBITS);
		void ImageProcessing(const TEmfBitBlt           &oTEmfBitBlt);
		void ImageProcessing(const TEmfSetDiBitsToDevice&oTEmfSetDiBitsToDevice);
		void ImageProcessing(const TEmfStretchBLT       &oTEmfStretchBLT);
		void ImageProcessing(const TEmfDibPatternBrush  &oTEmfDibPatternBrush, unsigned int ulBrushIndex);
		void DrawImage(int nX, int nY, int nW, int nH, BYTE *pImageBuffer, unsigned int unImageW, unsigned int unImageH);
		//----------------------

		void TranslatePoint(TEmfPointL &oPoint, double &dX, double &dY);
		void TranslatePoint(int nX, int nY, double &dX, double &dY);

		void UpdateOutputDC();
		void ClosePath();

		void MoveTo(double dX, double dY);
		void MoveTo(int nX, int nY);
		void MoveTo(TEmfPointD &oPoint);
		void MoveTo(TEmfPointL &oPoint);
		void MoveTo(TEmfPointS &oPoint);

		void LineTo(double dX, double dY);
		void LineTo(int nX, int nY);
		void LineTo(TEmfPointD &oPoint);
		void LineTo(TEmfPointL &oPoint);
		void LineTo(TEmfPointS &oPoint);

		void CurveTo(int nX1, int nY1, int nX2, int nY2, int nXe, int nYe);
		void CurveTo(TEmfPointS &oPoint1, TEmfPointS &oPoint2, TEmfPointS &oPointE);
		void CurveTo(TEmfPointL &oPoint1, TEmfPointL &oPoint2, TEmfPointL &oPointE);

		void ArcTo(int nL, int nT, int nR, int nB, double dStart, double dSweep);

		void DrawPath(bool bStroke, bool bFill, bool bClosePath = true);
		void DrawText(std::wstring &wsString, unsigned int unCharsCount, int _nX, int _nY, int *pnDx, int iGraphicsMode, TEmfScale oScale);
		void DrawTextA(TEmfEmrText &oText, int iGraphicsMode, TEmfScale oScale = TEmfScale(1, 1));
		void DrawTextW(TEmfEmrText &oText, int iGraphicsMode, TEmfScale oScale = TEmfScale(1, 1));

	private:
		friend class CEmfPlayer;
		friend class CEmfClip;

		friend class CEmfParser;
		friend class CEmfxParser;
		friend class CEmfPlusParser;

		TEmfHeader        m_oHeader;
		unsigned int      m_ulRecordSize;
		CEmfDC*           m_pDC;
		CEmfPlayer        m_oPlayer;
		CEmfPath*         m_pPath;
		TEmfXForm         m_oTransform;

		CEmfInterpretatorBase   *m_pInterpretator;

		bool              m_bEof;
	private:
		virtual bool ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight) = 0;

		TEmfPointL GetStartPointForArc(const TEmfRectL &oBox, double dStartAngle);

		void HANDLE_EMR_HEADER(TEmfHeader& oTEmfHeader);
		void HANDLE_EMR_ALPHABLEND(TEmfAlphaBlend& oTEmfAlphaBlend);
		void HANDLE_EMR_STRETCHDIBITS(TEmfStretchDIBITS& oTEmfStretchDIBITS);
		void HANDLE_EMR_BITBLT(TEmfBitBlt& oTEmfBitBlt);
		void HANDLE_EMR_SETDIBITSTODEVICE(TEmfSetDiBitsToDevice& oTEmfSetDiBitsToDevice);
		void HANDLE_EMR_STRETCHBLT(TEmfStretchBLT& oTEmfStretchBLT);
		void HANDLE_EMR_EOF();
		void HANDLE_EMR_SAVEDC();
		void HANDLE_EMR_RESTOREDC(int &nIndexDC);
		void HANDLE_EMR_MODIFYWORLDTRANSFORM(TEmfXForm& oXForm, unsigned int& unMode);
		void HANDLE_EMR_SETWORLDTRANSFORM(TEmfXForm& oXForm);
		void HANDLE_EMR_CREATEBRUSHINDIRECT(unsigned int& unBrushIndex, CEmfLogBrushEx* pBrush);
		void HANDLE_EMR_SETTEXTCOLOR(TEmfColor& oColor);
		void HANDLE_EMR_SELECTOBJECT(unsigned int& unObjectIndex);
		void HANDLE_EMR_EXTCREATEFONTINDIRECTW(unsigned int& unIndex, CEmfLogFont* oLogFont);
		void HANDLE_EMR_SETTEXTALIGN(unsigned int &unAlign);
		void HANDLE_EMR_SETBKMODE(unsigned int &unBgMode);
		void HANDLE_EMR_DELETEOBJECT(unsigned int &unObjectIndex);
		void HANDLE_EMR_SETMITERLIMIT(unsigned int &unMeterLimit);
		void HANDLE_EMR_EXTCREATEPEN(unsigned int& unPenIndex, CEmfLogPen* pPen, const std::vector<unsigned int>& arUnused);
		void HANDLE_EMR_CREATEPEN(unsigned int& unPenIndex, unsigned int& unWidthX, CEmfLogPen* pPen);
		void HANDLE_EMR_SETPOLYFILLMODE(unsigned int& unFillMode);
		void HANDLE_EMR_BEGINPATH();
		void HANDLE_EMR_ENDPATH();
		void HANDLE_EMR_CLOSEFIGURE();
		void HANDLE_EMR_FLATTENPATH();
		void HANDLE_EMR_WIDENPATH();
		void HANDLE_EMR_ABORTPATH();
		void HANDLE_EMR_MOVETOEX(TEmfPointL& oPoint);
		void HANDLE_EMR_SETARCDIRECTION(unsigned int& unDirection);
		void HANDLE_EMR_FILLPATH(TEmfRectL& oBounds);
		void HANDLE_EMR_SETMAPMODE(unsigned int& unMapMode);
		void HANDLE_EMR_SETWINDOWORGEX(TEmfPointL& oOrigin);
		void HANDLE_EMR_SETWINDOWEXTEX(TEmfSizeL& oExtent);
		void HANDLE_EMR_SCALEWINDOWEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom);
		void HANDLE_EMR_SETVIEWPORTORGEX(TEmfPointL& oOrigin);
		void HANDLE_EMR_SETVIEWPORTEXTEX(TEmfSizeL& oExtent);
		void HANDLE_EMR_SCALEVIEWPORTEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom);
		void HANDLE_EMR_SETSTRETCHBLTMODE(unsigned int& unStretchMode);
		void HANDLE_EMR_SETICMMODE(unsigned int& unICMMode);
		void HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(unsigned int& unBrushIndex, TEmfDibPatternBrush& oDibBrush);
		void HANDLE_EMR_CREATEMONOBRUSH(unsigned int& unBrushIndex, TEmfDibPatternBrush& oDibBrush);
		void HANDLE_EMR_SELECTCLIPPATH(unsigned int& unRegionMode);
		void HANDLE_EMR_SETBKCOLOR(TEmfColor& oColor);
		void HANDLE_EMR_EXCLUDECLIPRECT(TEmfRectL& oClip);
		void HANDLE_EMR_EXTSELECTCLIPRGN(unsigned int& unRgnDataSize, unsigned int& unRegionMode);
		void HANDLE_EMR_SETMETARGN();
		void HANDLE_EMR_SETROP2(unsigned int& unRop2Mode);
		void HANDLE_EMR_CREATEPALETTE(unsigned int& unPaletteIndex, CEmfLogPalette* oEmfLogPalette);
		void HANDLE_EMR_SELECTPALETTE(unsigned int& unPaletteIndex);
		void HANDLE_EMR_REALIZEPALETTE();
		void HANDLE_EMR_INTERSECTCLIPRECT(TEmfRectL& oClip);
		void HANDLE_EMR_SETLAYOUT(unsigned int& unLayoutMode);
		void HANDLE_EMR_SETBRUSHORGEX(TEmfPointL& oOrigin);
		void HANDLE_EMR_ANGLEARC(TEmfPointL& oCenter, unsigned int& unRadius, double& dStartAngle, double& dSweepAngle);
		void HANDLE_EMR_ARC(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd);
		void HANDLE_EMR_ARCTO(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd);
		void HANDLE_EMR_CHORD(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd);
		void HANDLE_EMR_ELLIPSE(TEmfRectL& oBox);
		void HANDLE_EMR_EXTTEXTOUTA(TEmfExtTextoutA& oTEmfExtTextoutA);
		void HANDLE_EMR_EXTTEXTOUTW(TEmfExtTextoutW& oTEmfExtTextoutW);
		void HANDLE_EMR_LINETO(TEmfPointL& oPoint);
		void HANDLE_EMR_PIE(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd);
		void HANDLE_EMR_POLYBEZIER(TEmfRectL& oBounds, std::vector<TEmfPointL>& arPoints);
		void HANDLE_EMR_POLYBEZIER(TEmfRectL& oBounds, std::vector<TEmfPointS>& arPoints);
		template<typename T> void HANDLE_EMR_POLYBEZIER_BASE(TEmfRectL& oBounds, std::vector<T>& arPoints)
		{
			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMR_POLYBEZIER(oBounds, arPoints);

			MoveTo(arPoints[0]);

			for (unsigned int unIndex = 1; unIndex < arPoints.size(); unIndex += 3)
				CurveTo(arPoints[unIndex], arPoints[unIndex + 1], arPoints[unIndex + 2]);

			DrawPath(true, false);
		}
		void HANDLE_EMR_POLYBEZIERTO(TEmfRectL& oBounds, std::vector<TEmfPointL>& arPoints);
		void HANDLE_EMR_POLYBEZIERTO(TEmfRectL& oBounds, std::vector<TEmfPointS>& arPoints);
		template<typename T> void HANDLE_EMR_POLYBEZIERTO_BASE(TEmfRectL& oBounds, std::vector<T>& arPoints)
		{
			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMR_POLYBEZIERTO(oBounds, arPoints);

			for (unsigned int unIndex = 0; unIndex < arPoints.size(); unIndex += 3)
				CurveTo(arPoints[unIndex], arPoints[unIndex + 1], arPoints[unIndex + 2]);
		}
		void HANDLE_EMR_POLYDRAW(TEmfRectL &oBounds, TEmfPointL *arPoints, unsigned int &unCount, unsigned char *pAbTypes);
		void HANDLE_EMR_POLYDRAW(TEmfRectL &oBounds, TEmfPointS *arPoints, unsigned int &unCount, unsigned char *pAbTypes);
		template<typename T> void HANDLE_EMR_POLYDRAW_BASE(TEmfRectL &oBounds, T *arPoints, unsigned int &unCount, unsigned char *pAbTypes)
		{
			T* pPoint1 = NULL, *pPoint2 = NULL;
			for (unsigned int unIndex = 0, unPointIndex = 0; unIndex < unCount; unIndex++)
			{
				unsigned char unType = pAbTypes[unIndex];
				T* pPoint = arPoints + unIndex;
				if (PT_MOVETO == unType)
				{
					MoveTo(*pPoint);
					unPointIndex = 0;
				}
				else if (PT_LINETO & unType)
				{
					LineTo(*pPoint);
					if (PT_CLOSEFIGURE & unType)
						ClosePath();
					unPointIndex = 0;
				}
				else if (PT_BEZIERTO & unType)
				{
					if (0 == unPointIndex)
					{
						pPoint1 = pPoint;
						unPointIndex = 1;
					}
					else if (1 == unPointIndex)
					{
						pPoint2 = pPoint;
						unPointIndex = 2;
					}
					else if (2 == unPointIndex)
					{
						CurveTo(*pPoint1, *pPoint2, *pPoint);
						unPointIndex = 0;

						if (PT_CLOSEFIGURE & unType)
							ClosePath();
					}
					else
					{
						SetError();
						break;
					}
				}
			}

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMR_POLYDRAW(oBounds, arPoints, unCount, pAbTypes);
		}
		void HANDLE_EMR_POLYGON(TEmfRectL& oBounds, std::vector<TEmfPointL>& arPoints);
		void HANDLE_EMR_POLYGON(TEmfRectL& oBounds, std::vector<TEmfPointS>& arPoints);
		template<typename T> void HANDLE_EMR_POLYGON_BASE(TEmfRectL& oBounds, std::vector<T>& arPoints)
		{
			if (arPoints.empty())
				return;

			MoveTo(arPoints[0]);

			for (unsigned int unIndex = 1; unIndex < arPoints.size(); ++unIndex)
				LineTo(arPoints[unIndex]);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMR_POLYGON(oBounds, arPoints);

			ClosePath();
			DrawPath(true, true);
		}
		void HANDLE_EMR_POLYLINE(TEmfRectL& oBounds, std::vector<TEmfPointL>& arPoints);
		void HANDLE_EMR_POLYLINE(TEmfRectL& oBounds, std::vector<TEmfPointS>& arPoints);
		template<typename T> void HANDLE_EMR_POLYLINE_BASE(TEmfRectL& oBounds, std::vector<T>& arPoints)
		{
			if (arPoints.empty())
				return;

			MoveTo(arPoints[0]);

			for (unsigned int unIndex = 1; unIndex < arPoints.size(); ++unIndex)
				LineTo(arPoints[unIndex]);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMR_POLYLINE(oBounds, arPoints);

			DrawPath(true, false);
		}
		void HANDLE_EMR_POLYLINETO(TEmfRectL& oBounds, std::vector<TEmfPointL>& arPoints);
		void HANDLE_EMR_POLYLINETO(TEmfRectL& oBounds, std::vector<TEmfPointS>& arPoints);
		template<typename T> void HANDLE_EMR_POLYLINETO_BASE(TEmfRectL& oBounds, std::vector<T>& arPoints)
		{
			for (unsigned int unIndex = 0; unIndex < arPoints.size(); ++unIndex)
				LineTo(arPoints[unIndex]);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMR_POLYLINETO(oBounds, arPoints);
		}
		void HANDLE_EMR_POLYPOLYGON(TEmfRectL& oBounds, std::vector<std::vector<TEmfPointL>>& arPoints);
		void HANDLE_EMR_POLYPOLYGON(TEmfRectL& oBounds, std::vector<std::vector<TEmfPointS>>& arPoints);
		template<typename T> void HANDLE_EMR_POLYPOLYGON_BASE(TEmfRectL& oBounds, std::vector<std::vector<T>>& arPoints)
		{
			for (unsigned int unPolygonIndex = 0; unPolygonIndex < arPoints.size(); ++unPolygonIndex)
			{
				MoveTo(arPoints[unPolygonIndex][0]);

				for (unsigned int unPointIndex = 1; unPointIndex < arPoints[unPolygonIndex].size(); ++unPointIndex)
					LineTo(arPoints[unPolygonIndex][unPointIndex]);

				ClosePath();
			}

			DrawPath(true, true);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMR_POLYPOLYGON(oBounds, arPoints);
		}
		void HANDLE_EMR_POLYPOLYLINE(TEmfRectL& oBounds, std::vector<std::vector<TEmfPointL>>& arPoints);
		void HANDLE_EMR_POLYPOLYLINE(TEmfRectL& oBounds, std::vector<std::vector<TEmfPointS>>& arPoints);
		template<typename T> void HANDLE_EMR_POLYPOLYLINE_BASE(TEmfRectL& oBounds, std::vector<std::vector<T>>& arPoints)
		{
			for (unsigned int unPolylineIndex = 0; unPolylineIndex < arPoints.size(); ++unPolylineIndex)
			{
				MoveTo(arPoints[unPolylineIndex][0]);

				for (unsigned int unPointIndex = 1; unPointIndex < arPoints[unPolylineIndex].size(); ++unPointIndex)
					LineTo(arPoints[unPolylineIndex][unPointIndex]);

				ClosePath();
			}

			DrawPath(true, false);

			if (NULL != m_pInterpretator)
				m_pInterpretator->HANDLE_EMR_POLYPOLYLINE(oBounds, arPoints);
		}
		//TODO: Реализовать сохранение полигонов в полигоне
		void HANDLE_EMR_RECTANGLE(TEmfRectL& oBox);
		void HANDLE_EMR_ROUNDRECT(TEmfRectL& oBox, TEmfSizeL& oCorner);
		void HANDLE_EMR_SETPIXELV(TEmfPointL& oPoint, TEmfColor& oColor);
		void HANDLE_EMR_SMALLTEXTOUT(TEmfSmallTextout& oText);
		void HANDLE_EMR_STROKEANDFILLPATH(TEmfRectL& oBounds);
		void HANDLE_EMR_STROKEPATH(TEmfRectL& oBounds);

		void HANDLE_EMR_UNKNOWN(const unsigned int& unRecordSize);
		void HANDLE_EMR_FILLRGN(const TEmfRectL& oBounds, unsigned int unIhBrush, const TRegionDataHeader& oRegionDataHeader, const std::vector<TEmfRectL>& arRects);
		void HANDLE_EMR_PAINTRGN(const TEmfRectL& oBounds, const TRegionDataHeader& oRegionDataHeader, const std::vector<TEmfRectL>& arRects);
		void HANDLE_EMR_FRAMERGN(const TEmfRectL& oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader& oRegionDataHeader, const std::vector<TEmfRectL>& arRects);

	};

}
#endif // CEMFPARSERBASE_H
