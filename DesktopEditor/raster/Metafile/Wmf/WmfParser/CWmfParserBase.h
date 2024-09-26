#ifndef CWMFPARSERBASE_H
#define CWMFPARSERBASE_H

#include "../../Common/MetaFile.h"

#include "../WmfPlayer.h"

#include "../WmfInterpretator/CWmfInterpretatorBase.h"

#if defined(DrawText)
#undef DrawText
#endif

#define PRINT_WMF_RECORD(type) do {} while(false);
#define PRINT_WMF_LOG(text)    do {} while(false);

#ifdef _DEBUG
#define LOG_WMF_RECORDS 1

#ifdef LOG_WMF_RECORDS
	#if 1 == LOG_WMF_RECORDS
		#include <iostream>

		#define PRINTING_WMF_RECORDS      1

		#define PRINT_WMF_LOG(text) \
			std::cout << "[LOG] " << text << std::endl

		#ifdef PRINTING_WMF_RECORDS
		#if 1 == PRINTING_WMF_RECORDS
			#define AddRecord(name) {name, #name}

			static const std::map<UINT, std::string> mWmfRecords =
			{
				AddRecord(META_EOF),
				AddRecord(META_SETBKCOLOR),
				AddRecord(META_SETBKMODE),
				AddRecord(META_SETMAPMODE),
				AddRecord(META_SETROP2),
				AddRecord(META_SETRELABS),
				AddRecord(META_SETPOLYFILLMODE),
				AddRecord(META_SETSTRETCHBLTMODE),
				AddRecord(META_SETTEXTCHAREXTRA),
				AddRecord(META_SETTEXTCOLOR),
				AddRecord(META_SETTEXTJUSTIFICATION),
				AddRecord(META_SETWINDOWORG),
				AddRecord(META_SETWINDOWEXT),
				AddRecord(META_SETVIEWPORTORG),
				AddRecord(META_SETVIEWPORTEXT),
				AddRecord(META_OFFSETWINDOWORG),
				AddRecord(META_SCALEWINDOWEXT),
				AddRecord(META_OFFSETVIEWPORTORG),
				AddRecord(META_SCALEVIEWPORTEXT),
				AddRecord(META_LINETO),
				AddRecord(META_MOVETO),
				AddRecord(META_EXCLUDECLIPRECT),
				AddRecord(META_INTERSECTCLIPRECT),
				AddRecord(META_ARC),
				AddRecord(META_ELLIPSE),
				AddRecord(META_FLOODFILL),
				AddRecord(META_PIE),
				AddRecord(META_RECTANGLE),
				AddRecord(META_ROUNDRECT),
				AddRecord(META_PATBLT),
				AddRecord(META_SAVEDC),
				AddRecord(META_SETPIXEL),
				AddRecord(META_OFFSETCLIPRGN),
				AddRecord(META_TEXTOUT),
				AddRecord(META_BITBLT),
				AddRecord(META_STRETCHBLT),
				AddRecord(META_POLYGON),
				AddRecord(META_POLYLINE),
				AddRecord(META_ESCAPE),
				AddRecord(META_RESTOREDC),
				AddRecord(META_FILLREGION),
				AddRecord(META_FRAMEREGION),
				AddRecord(META_INVERTREGION),
				AddRecord(META_PAINTREGION),
				AddRecord(META_SELECTCLIPREGION),
				AddRecord(META_SELECTOBJECT),
				AddRecord(META_SETTEXTALIGN),
				AddRecord(META_CHORD),
				AddRecord(META_SETMAPPERFLAGS),
				AddRecord(META_EXTTEXTOUT),
				AddRecord(META_SETDIBTODEV),
				AddRecord(META_SELECTPALETTE),
				AddRecord(META_REALIZEPALETTE),
				AddRecord(META_ANIMATEPALETTE),
				AddRecord(META_SETPALENTRIES),
				AddRecord(META_POLYPOLYGON),
				AddRecord(META_RESIZEPALETTE),
				AddRecord(META_DIBBITBLT),
				AddRecord(META_DIBSTRETCHBLT),
				AddRecord(META_DIBCREATEPATTERNBRUSH),
				AddRecord(META_STRETCHDIB),
				AddRecord(META_EXTFLOODFILL),
				AddRecord(META_SETLAYOUT),
				AddRecord(META_DELETEOBJECT),
				AddRecord(META_CREATEPALETTE),
				AddRecord(META_CREATEPATTERNBRUSH),
				AddRecord(META_CREATEPENINDIRECT),
				AddRecord(META_CREATEFONTINDIRECT),
				AddRecord(META_CREATEBRUSHINDIRECT),
				AddRecord(META_CREATEREGION)
			};

			#define PRINT_WMF_RECORD(type) \
			{\
				std::map<UINT, std::string>::const_iterator itFound = mWmfRecords.find(type); \
				if (mWmfRecords.cend() != itFound) \
					std::cout << itFound->second << std::endl; \
				else \
					std::cout << "Unknown record: " << type << std::endl; \
			}
		#endif
		#endif
	#endif
#endif
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

		virtual WmfParserType   GetType()                const  = 0;

		void            PlayMetaFile()                         override;
		void            ClearFile()                            override;
		const TRectL&   GetDCBounds()                    const override;
		const CClip*    GetClip()                        const override;
		double          GetPixelHeight()                 const override;
		double          GetPixelWidth()                  const override;
		int             GetTextColor()                   const override;
		const IFont*    GetFont()                        const override;
		const IBrush*   GetBrush()                       const override;
		const IPen*     GetPen()                         const override;
		unsigned int    GetTextAlign()                   const override;
		unsigned int    GetTextBgMode()                  const override;
		int             GetTextBgColor()                 const override;
		unsigned int    GetFillMode()                    const override;
		TPointD         GetCurPos()                      const override;
		const TXForm&   GetInverseTransform()            const override;
		const TXForm&   GetTransform(int = GM_ADVANCED)        override;
		unsigned int    GetMiterLimit()                  const override;
		unsigned int    GetRop2Mode()                    const override;
		int             GetCharSpace()                   const override;
		bool            IsWindowFlippedY()               const override;
		bool            IsWindowFlippedX()               const override;
		unsigned int    GetMapMode()                     const override;
		USHORT          GetDpi()                         const override;
		const IRegion*  GetRegion()                      const override;
		unsigned int    GetArcDirection()                const override;
		const CPath*    GetPath()                        const override;

		void SetInterpretator(IOutputDevice* pOutput);
		void SetInterpretator(const wchar_t *wsFilePath, InterpretatorType oInterpretatorType, unsigned int unWidth = 0, unsigned int unHeight = 0);
		void SetInterpretator(IOutputDevice* pOutput, const wchar_t *wsFilePath);
		void SetInterpretator(InterpretatorType oInterpretatorType, unsigned int unWidth = 0, unsigned int unHeight = 0);

		CWmfDC* GetDC();
		TRectD  GetBounds();

		CWmfInterpretatorBase* GetInterpretator();

	private:
		void    SkipVoid();
		void    TranslatePoint(short shX, short shY, double& dX, double &dY) const;
		void    UpdateDCRect();
		bool    IsPlaceable() const;
		int     GetRecordRemainingBytesCount() const;
		inline double GetSweepAngle(const double& dStartAngle, const double& dEndAngle) const;

		void MoveTo(short shX, short shY);
		void LineTo(short shX, short shY);
		void ArcTo(short shL, short shT, short shR, short shB, double dStart, double dSweep);
		void ClosePath();
		void DrawPath(bool bStroke, bool bFill);
		void DrawText(const unsigned char* pString, unsigned int unCharsCount, short _shX, short _shY, short* pDx);

		void RegisterPoint(short shX, short shY);

		bool ReadImage(unsigned short ushColorUsage, BYTE** ppBgraBuffer, unsigned int* pulWidth, unsigned int* pulHeight);
		void DrawImage(const TRectL& oDestRect, const TRectL& oSrcRect, unsigned int unColorUsage, unsigned int unRasterOperation);

		static BYTE* ClipBuffer(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, TRectL& oNewRect);

		void UpdateOutputDC();
	private:
		friend class CWmfPlayer;

		friend class CWmfParser;

		unsigned int   m_unRecordSize;
		unsigned int   m_unRecordPos;

		TWmfPlaceable  m_oPlaceable;
		TWmfHeader     m_oHeader;
		TRectL         m_oDCRect;

		CWmfPlayer     m_oPlayer;
		CWmfDC*        m_pDC;

		TRectL         m_oBoundingBox;
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
		void HANDLE_META_EXTTEXTOUT(short shY, short shX, short shStringLength, unsigned short ushFwOptions, const TRectS &oRectangle, unsigned char* pString, short* pDx);
		void HANDLE_META_FILLREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex);
		void HANDLE_META_FRAMEREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex, short shHeight, short shWidth);
		void HANDLE_META_INVERTREGION(unsigned short ushRegionIndex);
		void HANDLE_META_LINETO(short shY, short shX);
		void HANDLE_META_PAINTREGION(unsigned short ushRegionIndex);
		void HANDLE_META_PATBLT(unsigned int unRasterOperation, short shH, short shW, short shY, short shX);
		void HANDLE_META_PIE(short shXRadial1, short shYRadial1, short shXRadial2, short shYRadial2, short shB, short shR, short shT, short shL);
		void HANDLE_META_POLYLINE(const std::vector<TPointS>& arPoints);
		void HANDLE_META_POLYGON(const std::vector<TPointS>& arPoints);
		void HANDLE_META_POLYPOLYGON(const std::vector<std::vector<TPointS>>& arPolygons);
		void HANDLE_META_RECTANGLE(short shB, short shR, short shT, short shL);
		void HANDLE_META_ROUNDRECT(short shH, short shW, short shB, short shR, short shT, short shL);
		void HANDLE_META_SETPIXEL(const TRGBA& oColor, short shY, short shX);
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
		void HANDLE_META_SETBKCOLOR(TRGBA& oColor);
		void HANDLE_META_SETBKMODE(unsigned short ushMode);
		void HANDLE_META_SETLAYOUT(unsigned short ushLayout);
		void HANDLE_META_SETMAPMODE(unsigned short ushMapMode);
		void HANDLE_META_SETPOLYFILLMODE(unsigned short ushMode);
		void HANDLE_META_SETROP2(unsigned short ushMode);
		void HANDLE_META_SETSTRETCHBLTMODE(unsigned short ushMode);
		void HANDLE_META_SETTEXTALIGN(unsigned short ushTextAlign);
		void HANDLE_META_SETTEXTCHAREXTRA(unsigned short ushCharSpacing);
		void HANDLE_META_SETTEXTCOLOR(TRGBA& oColor);
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
