#ifndef CEMFINTERPRETATORBASE_H
#define CEMFINTERPRETATORBASE_H

#include "../EmfTypes.h"
#include "../EmfObjects.h"
#include "../../CXmlOutput.h"
#include "../../Common/IOutputDevice.h"

namespace MetaFile
{
    enum InterpretatorType
    {
        Render,
        XML,
        RenderAndXml
    };

    class CEmfInterpretatorBase : public IOutputDevice
    {
    public:
        virtual InterpretatorType   GetType()   = 0;

        virtual void HANDLER_EMR_HEADER(const TEmfHeader&, CDataStream&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_ALPHABLEND(const TEmfAlphaBlend&, CDataStream&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_STRETCHDIBITS(const TEmfStretchDIBITS&, CDataStream&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_BITBLT(const TEmfBitBlt&, CDataStream&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice&, CDataStream&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_STRETCHBLT(const TEmfStretchBLT& oTEmfStretchBLT, CDataStream &oDataStream, const unsigned int&) = 0;
        virtual void HANDLER_EMR_EOF(const unsigned int&, const unsigned int&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_SAVEDC() = 0;
        virtual void HANDLER_EMR_RESTOREDC(const int&) = 0;
        virtual void HANDLER_EMR_MODIFYWORLDTRANSFORM(const TEmfXForm&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_SETWORLDTRANSFORM(const TEmfXForm&) = 0;
        virtual void HANDLER_EMR_CREATEBRUSHINDIRECT(const unsigned int&, const CEmfLogBrushEx*) = 0;
        virtual void HANDLER_EMR_SETTEXTCOLOR(const TEmfColor&) = 0;
        virtual void HANDLER_EMR_SELECTOBJECT(const unsigned int&) = 0;
        virtual void HANDLER_EMR_EXTCREATEFONTINDIRECTW(const unsigned int&, CEmfLogFont*, const unsigned int) = 0;
        virtual void HANDLER_EMR_SETTEXTALIGN(const unsigned int&) = 0;
        virtual void HANDLER_EMR_SETBKMODE(const unsigned int&) = 0;
        virtual void HANDLER_EMR_DELETEOBJECT(const unsigned int&) = 0;
        virtual void HANDLER_EMR_SETMITERLIMIT(const unsigned int&) = 0;
        virtual void HANDLER_EMR_EXTCREATEPEN(const unsigned int&, CEmfLogPen*, const unsigned int&) = 0;
        virtual void HANDLER_EMR_CREATEPEN(const unsigned int&, const unsigned int&, const CEmfLogPen*) = 0;
        virtual void HANDLER_EMR_SETPOLYFILLMODE(const unsigned int&) = 0;
        virtual void HANDLER_EMR_BEGINPATH() = 0;
        virtual void HANDLER_EMR_ENDPATH() = 0;
        virtual void HANDLER_EMR_CLOSEFIGURE() = 0;
        virtual void HANDLER_EMR_FLATTENPATH() = 0;
        virtual void HANDLER_EMR_WIDENPATH() = 0;
        virtual void HANDLER_EMR_ABORTPATH() = 0;
        virtual void HANDLER_EMR_MOVETOEX(const TEmfPointL&) = 0;
        virtual void HANDLER_EMR_SETARCDIRECTION(const unsigned int&) = 0;
        virtual void HANDLER_EMR_FILLPATH(const TEmfRectL&) = 0;
        virtual void HANDLER_EMR_SETMAPMODE(const unsigned int&) = 0;
        virtual void HANDLER_EMR_SETWINDOWORGEX(const TEmfPointL&) = 0;
        virtual void HANDLER_EMR_SETWINDOWEXTEX(const TEmfSizeL&) = 0;
        virtual void HANDLER_EMR_SETVIEWPORTORGEX(const TEmfPointL&) = 0;
        virtual void HANDLER_EMR_SETVIEWPORTEXTEX(const TEmfSizeL&) = 0;
        virtual void HANDLER_EMR_SETSTRETCHBLTMODE(const unsigned int&) = 0;
        virtual void HANDLER_EMR_SETICMMODE(const unsigned int&) = 0;
        virtual void HANDLER_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int&, const TEmfDibPatternBrush&, CDataStream&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_SELECTCLIPPATH(const unsigned int&) = 0;
        virtual void HANDLER_EMR_SETBKCOLOR(const TEmfColor&) = 0;
        virtual void HANDLER_EMR_EXCLUDECLIPRECT(const TEmfRectL&) = 0;
        virtual void HANDLER_EMR_EXTSELECTCLIPRGN(const unsigned int&, const unsigned int&, CDataStream&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_SETMETARGN() = 0;
        virtual void HANDLER_EMR_SETROP2(const unsigned int&) = 0;
        virtual void HANDLER_EMR_CREATEPALETTE(const unsigned int&, const CEmfLogPalette*, const unsigned int&) = 0;
        virtual void HANDLER_EMR_SELECTPALETTE(const unsigned int&) = 0;
        virtual void HANDLER_EMR_REALIZEPALETTE() = 0;
        virtual void HANDLER_EMR_INTERSECTCLIPRECT(const TEmfRectL&) = 0;
        virtual void HANDLER_EMR_SETLAYOUT(const unsigned int&) = 0;
        virtual void HANDLER_EMR_SETBRUSHORGEX(const TEmfPointL&) = 0;
        virtual void HANDLER_EMR_ANGLEARC(const TEmfPointL&, const unsigned int&, const double&, const double&) = 0;
        virtual void HANDLER_EMR_ARC(const TEmfRectL&, const TEmfPointL&, const TEmfPointL&) = 0;
        virtual void HANDLER_EMR_ARCTO(const TEmfRectL&, const TEmfPointL&, const TEmfPointL&) = 0;
        virtual void HANDLER_EMR_CHORD(const TEmfRectL&, const TEmfPointL&, const TEmfPointL&) = 0;
        virtual void HANDLER_EMR_ELLIPSE(const TEmfRectL&) = 0;
        virtual void HANDLER_EMR_EXTTEXTOUTA(const TEmfExtTextoutA&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_EXTTEXTOUTW(const TEmfExtTextoutW&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_LINETO(const TEmfPointL&) = 0;
        virtual void HANDLER_EMR_PIE(const TEmfRectL&, const TEmfPointL&, const TEmfPointL&) = 0;
        virtual void HANDLER_EMR_POLYBEZIER(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
        virtual void HANDLER_EMR_POLYBEZIER(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
        virtual void HANDLER_EMR_POLYBEZIERTO(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
        virtual void HANDLER_EMR_POLYBEZIERTO(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
        virtual void HANDLER_EMR_POLYDRAW(const TEmfRectL&, TEmfPointL*, const unsigned int&, const unsigned char*) = 0;
        virtual void HANDLER_EMR_POLYDRAW(const TEmfRectL&, TEmfPointS*, const unsigned int&, const unsigned char*) = 0;
        virtual void HANDLER_EMR_POLYGON(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
        virtual void HANDLER_EMR_POLYGON(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
        virtual void HANDLER_EMR_POLYLINE(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
        virtual void HANDLER_EMR_POLYLINE(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
        virtual void HANDLER_EMR_POLYLINETO(const TEmfRectL&, const std::vector<TEmfPointL>&) = 0;
        virtual void HANDLER_EMR_POLYLINETO(const TEmfRectL&, const std::vector<TEmfPointS>&) = 0;
        //TODO: Реализовать сохранение полигонов в полигоне
        virtual void HANDLER_EMR_RECTANGLE(const TEmfRectL&) = 0;
        virtual void HANDLER_EMR_ROUNDRECT(const TEmfRectL&, const TEmfSizeL&) = 0;
        virtual void HANDLER_EMR_SETPIXELV(const TEmfPointL&, const TEmfColor&) = 0;
        virtual void HANDLER_EMR_SMALLTEXTOUT(const TEmfSmallTextout&, const unsigned int&) = 0;
        virtual void HANDLER_EMR_STROKEANDFILLPATH(const TEmfRectL&) = 0;
        virtual void HANDLER_EMR_STROKEPATH(const TEmfRectL&) = 0;
    };
}

#endif // CEMFINTERPRETATORBASE_H
