#ifndef CWMFPARSERBASE_H
#define CWMFPARSERBASE_H

#include "../../Common/MetaFile.h"

#include "../WmfPlayer.h"

#include "../WmfInterpretator/CWmfInterpretatorBase.h"

#if defined(DrawText)
#undef DrawText
#endif

namespace MetaFile
{
	enum WmfParserType
	{
		WmfParser
	};

	class CWmfPlayer;

	class  CWmfParserBase : public IMetaFileBase
	{
	public:
		CWmfParserBase();
		~CWmfParserBase();

		virtual bool            ReadFromBuffer(unsigned char*, unsigned int)    = 0;
		virtual bool            OpenFromFile(const wchar_t*)    = 0;

		virtual void            PlayFile()                      = 0;
		virtual void            Scan()                          = 0;

		virtual WmfParserType   GetType()                       = 0;

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
		unsigned int	GetTextBgMode()                  override;
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

		void SetInterpretator(IOutputDevice* pOutput);
		void SetInterpretator(const wchar_t *wsFilePath, InterpretatorType oInterpretatorType, unsigned int unWidth = 0, unsigned int unHeight = 0);
		void SetInterpretator(IOutputDevice* pOutput, const wchar_t *wsFilePath);
		void SetInterpretator(InterpretatorType oInterpretatorType, unsigned int unWidth = 0, unsigned int unHeight = 0);

		CWmfDC* GetDC();
		TRectD  GetBounds();

		CWmfInterpretatorBase* GetInterpretator();

	private:
		void    SkipVoid();
		void    TranslatePoint(short shX, short shY, double& dX, double &dY);
		TRect   GetBoundingBox();
		bool    IsPlaceable();
		int     GetRecordRemainingBytesCount();
		inline double GetSweepAngle(const double& dStartAngle, const double& dEndAngle);

		void MoveTo(short shX, short shY);
		void LineTo(short shX, short shY);
		void ArcTo(short shL, short shT, short shR, short shB, double dStart, double dSweep);
		void ClosePath();
		void DrawPath(bool bStroke, bool bFill);
		void DrawText(const unsigned char* pString, unsigned int unCharsCount, short _shX, short _shY, short* pDx);

		void RegisterPoint(short shX, short shY);

		bool ReadImage(unsigned short ushColorUsage, BYTE** ppBgraBuffer, unsigned int* pulWidth, unsigned int* pulHeight);
		void DrawImage(const TRect& oDestRect, const TRect& oSrcRect, unsigned int unColorUsage, unsigned int unRasterOperation);

		static BYTE* ClipBuffer(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, TRect& oNewRect);

		void UpdateOutputDC();
	private:
		friend class CWmfPlayer;

		friend class CWmfParser;

		unsigned int   m_unRecordSize;
		unsigned int   m_unRecordPos;

		TWmfPlaceable  m_oPlaceable;
		TWmfHeader     m_oHeader;
		TRect          m_oRect;
		TRect          m_oDCRect;

		CWmfPlayer     m_oPlayer;
		CWmfDC*        m_pDC;

		TRect          m_oBoundingBox;
		bool           m_bFirstPoint;

		TXForm         m_oTransform;

		CWmfInterpretatorBase   *m_pInterpretator;

		CWmfEscapeBuffer	m_oEscapeBuffer;

		bool           m_bEof;
	private:
		//                virtual bool ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight) = 0;

		void HANDLE_META_HEADER();
		//-----------------------------------------------------------
		// 2.3.1 Bitmap records
		//-----------------------------------------------------------
		void HANDLE_META_BITBLT(const TWmfBitBlt& oWmfBitBlt, CDataStream& oDataStream);
		void HANDLE_META_DIBBITBLT(const TWmfBitBlt& oWmfBitBlt, CDataStream& oDataStream);
		void HANDLE_META_DIBSTRETCHBLT(const TWmfStretchBlt& oWmfStretchBlt, CDataStream& oDataStream);
		void HANDLE_META_SETDIBTODEV(const TWmfSetDibToDev& oWmfSetDibToDev, CDataStream& oDataStream);
		void HANDLE_META_STRETCHBLT(const TWmfStretchBlt& oWmfStretchBlt, CDataStream& oDataStream);
		void HANDLE_META_STRETCHDIB(const TWmfStretchDib& oWmfStretchDib, CDataStream& oDataStream);
		//-----------------------------------------------------------
		// 2.3.2 Control records
		//-----------------------------------------------------------
		void HANDLE_META_EOF();
		//-----------------------------------------------------------
		// 2.3.3 Drawing records
		//-----------------------------------------------------------
		void HANDLE_META_ARC(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft);
		void HANDLE_META_CHORD(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft);
		void HANDLE_META_ELLIPSE(short shBottom, short shRight, short shTop, short shLeft);
		void HANDLE_META_EXTTEXTOUT(short shY, short shX, short shStringLength, unsigned short ushFwOptions, const TWmfRect& oRectangle, unsigned char* pString, short* pDx);
		void HANDLE_META_FILLREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex);
		void HANDLE_META_FRAMEREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex, short shHeight, short shWidth);
		void HANDLE_META_INVERTREGION(unsigned short ushRegionIndex);
		void HANDLE_META_LINETO(short shY, short shX);
		void HANDLE_META_PAINTREGION(unsigned short ushRegionIndex);
		void HANDLE_META_PATBLT(unsigned int unRasterOperation, short shH, short shW, short shY, short shX);
		void HANDLE_META_PIE(short shXRadial1, short shYRadial1, short shXRadial2, short shYRadial2, short shB, short shR, short shT, short shL);
		void HANDLE_META_POLYLINE(const std::vector<TWmfPointS>& arPoints);
		void HANDLE_META_POLYGON(const std::vector<TWmfPointS>& arPoints);
		void HANDLE_META_POLYPOLYGON(const std::vector<std::vector<TWmfPointS>>& arPolygons);
		void HANDLE_META_RECTANGLE(short shB, short shR, short shT, short shL);
		void HANDLE_META_ROUNDRECT(short shH, short shW, short shB, short shR, short shT, short shL);
		void HANDLE_META_SETPIXEL(const TWmfColor& oColor, short shY, short shX);
		void HANDLE_META_TEXTOUT(short shStringLength, unsigned char* pString, short shY, short shX);
		//-----------------------------------------------------------
		// 2.3.4 Object records
		//-----------------------------------------------------------
		void HANDLE_META_CREATEBRUSHINDIRECT(TWmfLogBrush& oBrush);
		void HANDLE_META_CREATEFONTINDIRECT(CWmfFont& oFont);
		void HANDLE_META_CREATEPALETTE(CWmfPalette& oPalette);
		void HANDLE_META_CREATEPATTERNBRUSH(CWmfBrush& oBrush, CDataStream& oDataStream);
		void HANDLE_META_CREATEPENINDIRECT(CWmfPen& oPen);
		void HANDLE_META_CREATEREGION(CWmfRegion& oRegion);
		void HANDLE_META_DELETEOBJECT(unsigned short ushIndex);
		void HANDLE_META_DIBCREATEPATTERNBRUSH(unsigned short ushStyle, unsigned short ushColorUsage);
		void HANDLE_META_SELECTCLIPREGION(unsigned short ushIndex);
		void HANDLE_META_SELECTOBJECT(unsigned short ushIndex);
		void HANDLE_META_SELECTPALETTE(unsigned short ushIndex);
		//-----------------------------------------------------------
		// 2.3.5 State records
		//-----------------------------------------------------------
		void HANDLE_META_EXCLUDECLIPRECT(short shBottom, short shRight, short shTop, short shLeft);
		void HANDLE_META_INTERSECTCLIPRECT(short shBottom, short shRight, short shTop, short shLeft);
		void HANDLE_META_MOVETO(short shX, short shY);
		void HANDLE_META_OFFSETCLIPRGN(short shOffsetX, short shOffsetY);
		void HANDLE_META_OFFSETVIEWPORTORG(short shXOffset, short shYOffset);
		void HANDLE_META_OFFSETWINDOWORG(short shXOffset, short shYOffset);
		void HANDLE_META_RESTOREDC();
		void HANDLE_META_SAVEDC();
		void HANDLE_META_SCALEVIEWPORTEXT(short yDenom, short yNum, short xDenom, short xNum);
		void HANDLE_META_SCALEWINDOWEXT(short yDenom, short yNum, short xDenom, short xNum);
		void HANDLE_META_SETBKCOLOR(TWmfColor& oColor);
		void HANDLE_META_SETBKMODE(unsigned short ushMode);
		void HANDLE_META_SETLAYOUT(unsigned short ushLayout);
		void HANDLE_META_SETMAPMODE(unsigned short ushMapMode);
		void HANDLE_META_SETPOLYFILLMODE(unsigned short ushMode);
		void HANDLE_META_SETROP2(unsigned short ushMode);
		void HANDLE_META_SETSTRETCHBLTMODE(unsigned short ushMode);
		void HANDLE_META_SETTEXTALIGN(unsigned short ushTextAlign);
		void HANDLE_META_SETTEXTCHAREXTRA(unsigned short ushCharSpacing);
		void HANDLE_META_SETTEXTCOLOR(TWmfColor& oColor);
		void HANDLE_META_SETTEXTJUSTIFICATION(unsigned short ushBreakCount, unsigned short ushBreakExtra);
		void HANDLE_META_SETVIEWPORTEXT(short shX, short shY);
		void HANDLE_META_SETVIEWPORTORG(short shX, short shY);
		void HANDLE_META_SETWINDOWEXT(short shX, short shY);
		void HANDLE_META_SETWINDOWORG(short shX, short shY);
		//-----------------------------------------------------------
		// 2.3.6 State records
		//-----------------------------------------------------------
		void HANDLE_META_ESCAPE(unsigned short ushEscapeFunction, unsigned short ushByteCount);
		//-----------------------------------------------------------
		// Неизвестные записи
		//-----------------------------------------------------------
		void HANDLE_META_UNKNOWN(CDataStream& oDataStream);
	};

}

#endif // CWMFPARSERBASE_H
