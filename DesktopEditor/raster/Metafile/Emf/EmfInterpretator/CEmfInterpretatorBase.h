#ifndef CEMFINTERPRETATORBASE_H
#define CEMFINTERPRETATORBASE_H

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
		virtual void HANDLE_EMR_SCALEWINDOWEXTEX(int, int, int, int) = 0;
		virtual void HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL&) = 0;
		virtual void HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL&) = 0;
		virtual void HANDLE_EMR_SCALEVIEWPORTEXTEX(int, int, int, int) = 0;
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

		virtual void HANDLE_EMR_UNKNOWN(CDataStream &) = 0;
		virtual void HANDLE_EMR_FILLRGN(const TEmfRectL&, unsigned int, const TRegionDataHeader&, const std::vector<TEmfRectL>&) = 0;
		virtual void HANDLE_EMR_PAINTRGN(const TEmfRectL&, const TRegionDataHeader&, const std::vector<TEmfRectL>&) = 0;
		virtual void HANDLE_EMR_FRAMERGN(const TEmfRectL&, unsigned int, int, int, const TRegionDataHeader&, const std::vector<TEmfRectL>&) = 0;

		// Emf+
		// 2.3.1 Clipping Record Types
		virtual void HANDLE_EMFPLUS_OFFSETCLIP(double, double) = 0;
		virtual void HANDLE_EMFPLUS_RESETCLIP() = 0;
		virtual void HANDLE_EMFPLUS_SETCLIPPATH(short, const CEmfPlusPath*) = 0;
		virtual void HANDLE_EMFPLUS_SETCLIPRECT(short, const TEmfPlusRectF&) = 0;
		virtual void HANDLE_EMFPLUS_SETCLIPREGION(short, short, const CEmfPlusRegion*) = 0;

		// 2.3.2 Comment Record Types
		virtual void HANDLE_EMFPLUS_COMMENT(CDataStream &, unsigned int) = 0;

		// 2.3.3 Control Record Types
		virtual void HANDLE_EMFPLUS_ENDOFFILE() = 0;
		virtual void HANDLE_EMFPLUS_GETDC() = 0;
		virtual void HANDLE_EMFPLUS_HEADER(unsigned int, unsigned int, unsigned int) = 0;

		// 2.3.4 Drawing Record Types
		virtual void HANDLE_EMFPLUS_CLEAR(const TEmfPlusARGB&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWARC(BYTE, double, double, const TEmfPlusRectF&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWBEZIERS(short, const std::vector<TEmfPlusPointF>&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWCLOSEDCURVE(short, double, const std::vector<TEmfPlusPointF>&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWCURVE(short, double, unsigned int, unsigned int,const std::vector<TEmfPlusPointF>&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWDRIVERSTRING(short, unsigned int, unsigned int, unsigned int, TEmfPlusXForm*, const std::wstring&, const std::vector<TPointD>&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWELLIPSE(short, const TEmfPlusRectF&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWIMAGE(short, unsigned int, const TEmfPlusRectF&, const TEmfPlusRectF&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWIMAGEPOINTS(short, unsigned int, const TEmfPlusRectF&, const TEmfPlusRectF&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWLINES(short, const std::vector<TEmfPlusPointF>&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWPATH(short, unsigned int, const CEmfPath*) = 0;
		virtual void HANDLE_EMFPLUS_DRAWPIE(short, double, double, const TEmfPlusRectF&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWRECTS(short, const std::vector<TEmfPlusRectF>&) = 0;
		virtual void HANDLE_EMFPLUS_DRAWSTRING(short, unsigned int, unsigned int, const std::wstring&, const TEmfPlusRectF&) = 0;
		virtual void HANDLE_EMFPLUS_FILLCLOSEDCURVE(unsigned int, double, const std::vector<TEmfPlusRectF>&) = 0;
		virtual void HANDLE_EMFPLUS_FILLELLIPSE(unsigned int, const TEmfPlusRectF&) = 0;
		virtual void HANDLE_EMFPLUS_FILLPATH(short, unsigned int, const  CEmfPlusPath*) = 0;
		virtual void HANDLE_EMFPLUS_FILLPIE(unsigned int, double, double, const TEmfPlusRectF&) = 0;
		virtual void HANDLE_EMFPLUS_FILLPOLYGON(unsigned int, const std::vector<TEmfPlusPointF>&) = 0;
		virtual void HANDLE_EMFPLUS_FILLRECTS(unsigned int, const std::vector<TEmfPlusRectF>&) = 0;
		virtual void HANDLE_EMFPLUS_FILLREGION(short, unsigned int) = 0;

		// 2.3.5 Object Record Types
		virtual void HANDLE_EMFPLUS_OBJECT(const CEmfPlusObject*, unsigned int) = 0;
		virtual void HANDLE_EMFPLUS_SERIALIZABLEOBJECT(const TGUID&, unsigned int) = 0;

		// 2.3.6 Property Record Types
		virtual void HANDLE_EMFPLUS_SETANTIALIASMODE(short) = 0;
		virtual void HANDLE_EMFPLUS_SETCOMPOSITINGMODE(short) = 0;
		virtual void HANDLE_EMFPLUS_SETCOMPOSITINGQUALITY(short) = 0;
		virtual void HANDLE_EMFPLUS_SETINTERPOLATIONMODE(short) = 0;
		virtual void HANDLE_EMFPLUS_SETPIXELOFFSETMODE(short) = 0;
		virtual void HANDLE_EMFPLUS_SETRENDERINGORIGIN(int, int) = 0;
		virtual void HANDLE_EMFPLUS_SETTEXTCONTRAST(short) = 0;
		virtual void HANDLE_EMFPLUS_SETTEXTRENDERINGHINT(short) = 0;

		// 2.3.7 State Record Types
		virtual void HANDLE_EMFPLUS_BEGINCONTAINER(short, const TEmfPlusRectF&, const TEmfPlusRectF&, unsigned int) = 0;
		virtual void HANDLE_EMFPLUS_BEGINCONTAINERNOPARAMS(unsigned int) = 0;
		virtual void HANDLE_EMFPLUS_ENDCONTAINER(unsigned int) = 0;
		virtual void HANDLE_EMFPLUS_RESTORE(unsigned int) = 0;
		virtual void HANDLE_EMFPLUS_SAVE(unsigned int) = 0;

		// 2.3.8 Terminal Server Record Types
		virtual void HANDLE_EMFPLUS_SETTSCLIP(short, const std::vector<TEmfPlusRectF>&) = 0;
		virtual void HANDLE_EMFPLUS_SETTSGRAPHICS(unsigned char, unsigned char, unsigned char, unsigned char, short, short, unsigned short, unsigned char, unsigned char, const TEmfPlusXForm&) = 0;

		// 2.3.9 Transform Record Types
		virtual void HANDLE_EMFPLUS_MULTIPLYWORLDTRANSFORM(short, const TEmfPlusXForm&) = 0;
		virtual void HANDLE_EMFPLUS_RESETWORLDTRANSFORM() = 0;
		virtual void HANDLE_EMFPLUS_ROTATEWORLDTRANSFORM(short, double) = 0;
		virtual void HANDLE_EMFPLUS_SCALEWORLDTRANSFORM(short, double, double) = 0;
		virtual void HANDLE_EMFPLUS_SETPAGETRANSFORM(short, double) = 0;
		virtual void HANDLE_EMFPLUS_SETWORLDTRANSFORM(const TEmfPlusXForm&) = 0;
		virtual void HANDLE_EMFPLUS_TRANSLATEWORLDTRANSFORM(short, double, double) = 0;
	};
}

#endif // CEMFINTERPRETATORBASE_H
