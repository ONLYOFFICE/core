#ifndef CEMFINTERPRETATORBASE_H
#define CEMFINTERPRETATORBASE_H

#include "../../CXmlOutput.h"
#include "../../Common/IOutputDevice.h"
#include "../../Common/MetaFile.h"

namespace MetaFile
{
	class CEmfInterpretatorBase : public IOutputDevice
	{
	public:
		virtual InterpretatorType   GetType() const = 0;

		virtual void CreateConditional(IMetaFileBase*) = 0;
		virtual void ChangeConditional() = 0;

		virtual void HANDLE_EMR_HEADER(const TEmfHeader&) = 0;
		virtual void HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend&, CDataStream&) = 0;
		virtual void HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS&, CDataStream&) = 0;
		virtual void HANDLE_EMR_BITBLT(const TEmfBitBlt&, CDataStream&) = 0;
		virtual void HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice&, CDataStream&) = 0;
		virtual void HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT& oTEmfStretchBLT, CDataStream &oDataStream) = 0;
		virtual void HANDLE_EMR_EOF() = 0;
		virtual void HANDLE_EMR_SAVEDC() = 0;
		virtual void HANDLE_EMR_RESTOREDC(const int&) = 0;
		virtual void HANDLE_EMR_MODIFYWORLDTRANSFORM(const TEmfXForm&, const unsigned int&) = 0;
		virtual void HANDLE_EMR_SETWORLDTRANSFORM(const TEmfXForm&) = 0;
		virtual void HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int&, const CEmfLogBrushEx*) = 0;
		virtual void HANDLE_EMR_SETTEXTCOLOR(const TEmfColor&) = 0;
		virtual void HANDLE_EMR_SELECTOBJECT(const unsigned int&) = 0;
		virtual void HANDLE_EMR_EXTCREATEFONTINDIRECTW(const unsigned int&, CEmfLogFont*) = 0;
		virtual void HANDLE_EMR_SETTEXTALIGN(const unsigned int&) = 0;
		virtual void HANDLE_EMR_SETBKMODE(const unsigned int&) = 0;
		virtual void HANDLE_EMR_DELETEOBJECT(const unsigned int&) = 0;
		virtual void HANDLE_EMR_SETMITERLIMIT(const unsigned int&) = 0;
		virtual void HANDLE_EMR_EXTCREATEPEN(const unsigned int&, CEmfLogPen*, const std::vector<unsigned int>&) = 0;
		virtual void HANDLE_EMR_CREATEPEN(const unsigned int&, const unsigned int&, const CEmfLogPen*) = 0;
		virtual void HANDLE_EMR_SETPOLYFILLMODE(const unsigned int&) = 0;
		virtual void HANDLE_EMR_BEGINPATH() = 0;
		virtual void HANDLE_EMR_ENDPATH() = 0;
		virtual void HANDLE_EMR_CLOSEFIGURE() = 0;
		virtual void HANDLE_EMR_FLATTENPATH() = 0;
		virtual void HANDLE_EMR_WIDENPATH() = 0;
		virtual void HANDLE_EMR_ABORTPATH() = 0;
		virtual void HANDLE_EMR_MOVETOEX(const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_SETARCDIRECTION(const unsigned int&) = 0;
		virtual void HANDLE_EMR_FILLPATH(const TEmfRectL&) = 0;
		virtual void HANDLE_EMR_SETMAPMODE(const unsigned int&) = 0;
		virtual void HANDLE_EMR_SETWINDOWORGEX(const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_SETWINDOWEXTEX(const TEmfSizeL&) = 0;
		virtual void HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL&) = 0;
		virtual void HANDLE_EMR_SETSTRETCHBLTMODE(const unsigned int&) = 0;
		virtual void HANDLE_EMR_SETICMMODE(const unsigned int&) = 0;
		virtual void HANDLE_EMR_CREATEMONOBRUSH(const unsigned int&, const TEmfDibPatternBrush&, CDataStream&) = 0;
		virtual void HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int&, const TEmfDibPatternBrush&, CDataStream&) = 0;
		virtual void HANDLE_EMR_SELECTCLIPPATH(const unsigned int&) = 0;
		virtual void HANDLE_EMR_SETBKCOLOR(const TEmfColor&) = 0;
		virtual void HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL&) = 0;
		virtual void HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int&, const unsigned int&, CDataStream&) = 0;
		virtual void HANDLE_EMR_SETMETARGN() = 0;
		virtual void HANDLE_EMR_SETROP2(const unsigned int&) = 0;
		virtual void HANDLE_EMR_CREATEPALETTE(const unsigned int&, const CEmfLogPalette*) = 0;
		virtual void HANDLE_EMR_SELECTPALETTE(const unsigned int&) = 0;
		virtual void HANDLE_EMR_REALIZEPALETTE() = 0;
		virtual void HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL&) = 0;
		virtual void HANDLE_EMR_SETLAYOUT(const unsigned int&) = 0;
		virtual void HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_ANGLEARC(const TEmfPointL&, const unsigned int&, const double&, const double&) = 0;
		virtual void HANDLE_EMR_ARC(const TEmfRectL&, const TEmfPointL&, const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_ARCTO(const TEmfRectL&, const TEmfPointL&, const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_CHORD(const TEmfRectL&, const TEmfPointL&, const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_ELLIPSE(const TEmfRectL&) = 0;
		virtual void HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA&) = 0;
		virtual void HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW&) = 0;
		virtual void HANDLE_EMR_LINETO(const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_PIE(const TEmfRectL&, const TEmfPointL&, const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_POLYBEZIER(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
		virtual void HANDLE_EMR_POLYBEZIER(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
		virtual void HANDLE_EMR_POLYBEZIERTO(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
		virtual void HANDLE_EMR_POLYBEZIERTO(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
		virtual void HANDLE_EMR_POLYDRAW(const TEmfRectL&, TEmfPointL*, const unsigned int&, const unsigned char*) = 0;
		virtual void HANDLE_EMR_POLYDRAW(const TEmfRectL&, TEmfPointS*, const unsigned int&, const unsigned char*) = 0;
		virtual void HANDLE_EMR_POLYGON(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
		virtual void HANDLE_EMR_POLYGON(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
		virtual void HANDLE_EMR_POLYLINE(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
		virtual void HANDLE_EMR_POLYLINE(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
		virtual void HANDLE_EMR_POLYLINETO(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
		virtual void HANDLE_EMR_POLYLINETO(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
		virtual void HANDLE_EMR_POLYPOLYGON(const TEmfRectL&, const std::vector<std::vector<TEmfPointL>>&) = 0;
		virtual void HANDLE_EMR_POLYPOLYGON(const TEmfRectL&, const std::vector<std::vector<TEmfPointS>>&) = 0;
		virtual void HANDLE_EMR_POLYPOLYLINE(const TEmfRectL&, const std::vector<std::vector<TEmfPointL>>&) = 0;
		virtual void HANDLE_EMR_POLYPOLYLINE(const TEmfRectL&, const std::vector<std::vector<TEmfPointS>>&) = 0;
		//TODO: Реализовать сохранение полигонов в полигоне
		virtual void HANDLE_EMR_RECTANGLE(const TEmfRectL&) = 0;
		virtual void HANDLE_EMR_ROUNDRECT(const TEmfRectL&, const TEmfSizeL&) = 0;
		virtual void HANDLE_EMR_SETPIXELV(const TEmfPointL&, const TEmfColor&) = 0;
		virtual void HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout&) = 0;
		virtual void HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL&) = 0;
		virtual void HANDLE_EMR_STROKEPATH(const TEmfRectL&) = 0;

		virtual void HANDLE_EMR_UNKNOWN(CDataStream &oDataStream) = 0;
		virtual void HANDLE_EMR_FILLRGN(const TEmfRectL& oBounds, unsigned int unIhBrush, const TRegionDataHeader& oRegionDataHeader, const std::vector<TEmfRectL>& arRects) = 0;

		virtual void HANDLE_EMFPLUS_HEADER(bool bIsEmfPlusDual, bool bIsReferenceDevice, unsigned int unDpiX, unsigned int unDpiY) = 0;
		virtual void HANDLE_EMFPLUS_CLEAR(TEmfPlusARGB oColor) = 0;
		virtual void HANDLE_EMFPLUS_DRAWARC(char chPenId, double dStartAngle, double dSweepAngle, TEmfPlusRect oRect) = 0;
		virtual void HANDLE_EMFPLUS_DRAWARC(char chPenId, double dStartAngle, double dSweepAngle, TEmfPlusRectF oRect) = 0;
		virtual void HANDLE_EMFPLUS_DRAWBEZIERS(char chPenId, std::vector<TEmfPlusPointR> arPoints) = 0;
		virtual void HANDLE_EMFPLUS_DRAWBEZIERS(char chPenId, std::vector<TEmfPlusPointF> arPoints) = 0;
		virtual void HANDLE_EMFPLUS_DRAWBEZIERS(char chPenId, std::vector<TEmfPlusPoint> arPoints) = 0;
		virtual void HANDLE_EMFPLUS_DRAWCLOSEDCURVE(char chPenId, double dTension, std::vector<TEmfPlusPointR> arPoints) = 0;
		virtual void HANDLE_EMFPLUS_DRAWCLOSEDCURVE(char chPenId, double dTension, std::vector<TEmfPlusPointF> arPoints) = 0;
		virtual void HANDLE_EMFPLUS_DRAWCLOSEDCURVE(char chPenId, double dTension, std::vector<TEmfPlusPoint> arPoints) = 0;
		virtual void HANDLE_EMFPLUS_DRAWCURVE(char chPenId, double dTension, unsigned int unOffset, unsigned int unNumSegments, std::vector<TEmfPlusPoint> arPoints) = 0;
		virtual void HANDLE_EMFPLUS_DRAWCURVE(char chPenId, double dTension, unsigned int unOffset, unsigned int unNumSegments, std::vector<TEmfPlusPointF> arPoints) = 0;
		virtual void HANDLE_EMFPLUS_DRAWELLIPSE(char chPenId, TEmfPlusRect oRect) = 0;
		virtual void HANDLE_EMFPLUS_DRAWELLIPSE(char chPenId, TEmfPlusRectF oRect) = 0;
		virtual void HANDLE_EMFPLUS_DRAWDRIVERSTRING(char chFontId, unsigned int unBrushId, unsigned int unDriverStringOptionsFlags, unsigned int unMatrixPresent, const std::wstring& wsString, const std::vector<TEmfPlusPointF>& arGlyphPos) = 0;
		virtual void HANDLE_EMFPLUS_DRAWIMAGE(char chEmfPlusImageId, unsigned int unImageAttributesId, int nSrcUnit, const TEmfPlusRectF& oSrcRect, const TEmfPlusRect& oRectData) = 0;
		virtual void HANDLE_EMFPLUS_DRAWIMAGE(char chEmfPlusImageId, unsigned int unImageAttributesId, int nSrcUnit, const TEmfPlusRectF& oSrcRect, const TEmfPlusRectF& oRectData) = 0;
	};
}

#endif // CEMFINTERPRETATORBASE_H
