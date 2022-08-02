#ifndef CWMFINTERPRETATORBASE_H
#define CWMFINTERPRETATORBASE_H

#include "../../Common/IOutputDevice.h"
#include "../../Common/MetaFileUtils.h"

#include "../WmfTypes.h"

namespace MetaFile
{
	class CWmfInterpretatorBase : public IOutputDevice
	{
	public:
		virtual InterpretatorType GetType() const = 0;

		virtual void HANDLE_META_HEADER(const TWmfPlaceable&, const TWmfHeader&) = 0;
		//-----------------------------------------------------------
		// 2.3.1 Bitmap records
		//-----------------------------------------------------------
		virtual void HANDLE_META_BITBLT(const TWmfBitBlt& oWmfBitBlt, CDataStream& oDataStream) = 0;
		virtual void HANDLE_META_DIBBITBLT(const TWmfBitBlt& oWmfBitBlt, CDataStream& oDataStream) = 0;
		virtual void HANDLE_META_DIBSTRETCHBLT(const TWmfStretchBlt& oWmfStretchBlt, CDataStream& oDataStream) = 0;
		virtual void HANDLE_META_SETDIBTODEV(const TWmfSetDibToDev& oWmfSetDibToDev, CDataStream& oDataStream) = 0;
		virtual void HANDLE_META_STRETCHBLT(const TWmfStretchBlt& oWmfStretchBlt, CDataStream& oDataStream) = 0;
		virtual void HANDLE_META_STRETCHDIB(const TWmfStretchDib& oWmfStretchDib, CDataStream& oDataStream) = 0;
		//-----------------------------------------------------------
		// 2.3.2 Control records
		//-----------------------------------------------------------
		virtual void HANDLE_META_EOF() = 0;
		//-----------------------------------------------------------
		// 2.3.3 Drawing records
		//-----------------------------------------------------------
		virtual void HANDLE_META_ARC(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft) = 0;
		virtual void HANDLE_META_CHORD(short shYEndArc, short shXEndArc, short shYStartArc, short shXStartArc, short shBottom, short shRight, short shTop, short shLeft) = 0;
		virtual void HANDLE_META_ELLIPSE(short shBottom, short shRight, short shTop, short shLeft) = 0;
		virtual void HANDLE_META_EXTTEXTOUT(short shY, short shX, short shStringLength, unsigned short ushFwOptions, const TWmfRect& oRectangle, unsigned char* pString, short* pDx) = 0;
		virtual void HANDLE_META_FILLREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex) = 0;
		virtual void HANDLE_META_FRAMEREGION(unsigned short ushRegionIndex, unsigned short ushBrushIndex, short shHeight, short shWidth) = 0;
		virtual void HANDLE_META_INVERTREGION(unsigned short ushRegionIndex) = 0;
		virtual void HANDLE_META_LINETO(short shY, short shX) = 0;
		virtual void HANDLE_META_PAINTREGION(unsigned short ushRegionIndex) = 0;
		virtual void HANDLE_META_PATBLT(unsigned int unRasterOperation, short shH, short shW, short shY, short shX) = 0;
		virtual void HANDLE_META_PIE(short shXRadial1, short shYRadial1, short shXRadial2, short shYRadial2, short shB, short shR, short shT, short shL) = 0;
		virtual void HANDLE_META_POLYLINE(const std::vector<TWmfPointS>& arPoints) = 0;
		virtual void HANDLE_META_POLYGON(const std::vector<TWmfPointS>& arPoints) = 0;
		virtual void HANDLE_META_POLYPOLYGON(const std::vector<std::vector<TWmfPointS>>& arPolygons) = 0;
		virtual void HANDLE_META_RECTANGLE(short shB, short shR, short shT, short shL) = 0;
		virtual void HANDLE_META_ROUNDRECT(short shH, short shW, short shB, short shR, short shT, short shL) = 0;
		virtual void HANDLE_META_SETPIXEL(const TWmfColor &oColor, short shY, short shX) = 0;
		virtual void HANDLE_META_TEXTOUT(short shStringLength, unsigned char* pString, short shY, short shX) = 0;
		//-----------------------------------------------------------
		// 2.3.4 Object records
		//-----------------------------------------------------------
		virtual void HANDLE_META_CREATEBRUSHINDIRECT(const TWmfLogBrush& oBrush) = 0;
		virtual void HANDLE_META_CREATEFONTINDIRECT(const CWmfFont& oFont) = 0;
		virtual void HANDLE_META_CREATEPALETTE(const CWmfPalette& oPalette) = 0;
		virtual void HANDLE_META_CREATEPATTERNBRUSH(const CWmfBrush& oBrush, CDataStream& oDataStream) = 0;
		virtual void HANDLE_META_CREATEPENINDIRECT(const CWmfPen& oPen) = 0;
		virtual void HANDLE_META_CREATEREGION(const CWmfRegion& oRegion) = 0;
		virtual void HANDLE_META_DELETEOBJECT(unsigned short ushIndex) = 0;
		virtual void HANDLE_META_DIBCREATEPATTERNBRUSH(unsigned short ushStyle, unsigned short ushColorUsage, const CWmfBrush& oBrush, CDataStream& oDataStream) = 0;
		virtual void HANDLE_META_SELECTCLIPREGION(unsigned short ushIndex) = 0;
		virtual void HANDLE_META_SELECTOBJECT(unsigned short ushIndex) = 0;
		virtual void HANDLE_META_SELECTPALETTE(unsigned short ushIndex) = 0;
		//-----------------------------------------------------------
		// 2.3.5 State records
		//-----------------------------------------------------------
		virtual void HANDLE_META_EXCLUDECLIPRECT(short shLeft, short shTop, short shRight, short shBottom) = 0;
		virtual void HANDLE_META_INTERSECTCLIPRECT(short shLeft, short shTop, short shRight, short shBottom) = 0;
		virtual void HANDLE_META_MOVETO(short shX, short shY) = 0;
		virtual void HANDLE_META_OFFSETCLIPRGN(short shOffsetX, short shOffsetY) = 0;
		virtual void HANDLE_META_OFFSETVIEWPORTORG(short shXOffset, short shYOffset) = 0;
		virtual void HANDLE_META_OFFSETWINDOWORG(short shXOffset, short shYOffset) = 0;
		virtual void HANDLE_META_RESTOREDC() = 0;
		virtual void HANDLE_META_SAVEDC() = 0;
		virtual void HANDLE_META_SCALEVIEWPORTEXT(short yDenom, short yNum, short xDenom, short xNum) = 0;
		virtual void HANDLE_META_SCALEWINDOWEXT(short yDenom, short yNum, short xDenom, short xNum) = 0;
		virtual void HANDLE_META_SETBKCOLOR(const TWmfColor& oColor) = 0;
		virtual void HANDLE_META_SETBKMODE(unsigned short ushMode) = 0;
		virtual void HANDLE_META_SETLAYOUT(unsigned short ushLayout) = 0;
		virtual void HANDLE_META_SETMAPMODE(unsigned short ushMapMode) = 0;
		virtual void HANDLE_META_SETPOLYFILLMODE(unsigned short ushMode) = 0;
		virtual void HANDLE_META_SETROP2(unsigned short ushMode) = 0;
		virtual void HANDLE_META_SETSTRETCHBLTMODE(unsigned short ushMode) = 0;
		virtual void HANDLE_META_SETTEXTALIGN(unsigned short ushTextAlign) = 0;
		virtual void HANDLE_META_SETTEXTCHAREXTRA(unsigned short ushCharSpacing) = 0;
		virtual void HANDLE_META_SETTEXTCOLOR(const TWmfColor& oColor) = 0;
		virtual void HANDLE_META_SETTEXTJUSTIFICATION(unsigned short ushBreakCount, unsigned short ushBreakExtra) = 0;
		virtual void HANDLE_META_SETVIEWPORTEXT(short shX, short shY) = 0;
		virtual void HANDLE_META_SETVIEWPORTORG(short shX, short shY) = 0;
		virtual void HANDLE_META_SETWINDOWEXT(short shX, short shY) = 0;
		virtual void HANDLE_META_SETWINDOWORG(short shX, short shY) = 0;
		//-----------------------------------------------------------
		// 2.3.6 State records
		//-----------------------------------------------------------
		virtual void HANDLE_META_ESCAPE(unsigned short ushEscapeFunction, unsigned short ushByteCount) = 0;
		//-----------------------------------------------------------
		// Неизвестные записи
		//-----------------------------------------------------------
		virtual void HANDLE_META_UNKNOWN(CDataStream& oDataStream) = 0;
	};
}

#endif // CWMFINTERPRETATORBASE_H
