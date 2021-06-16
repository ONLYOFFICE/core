#ifndef CEMFINTERPRETARORXML_H
#define CEMFINTERPRETARORXML_H

//#include "CEmfInterpretarorBase.h"
#include "../../Common/IOutputDevice.h"
#include "../../CXmlOutput.h"

namespace MetaFile
{
    class CEmfInterpretarorXml /*: public IOutputDevice*/
    {
    public:
        CEmfInterpretarorXml();
        virtual ~CEmfInterpretarorXml();

        void Save_EMR_HEADER(const TEmfHeader& oTEmfHeader, CDataStream &oDataStream, const unsigned int &unRecordSize);
        void Save_EMR_ALPHABLEND(const TEmfAlphaBlend& oTEmfAlphaBlend, CDataStream &oDataStream, const unsigned int &unRecordSize);
        void Save_EMR_STRETCHDIBITS(const TEmfStretchDIBITS& oTEmfStretchDIBITS, CDataStream &oDataStream, const unsigned int &unRecordSize);
        void Save_EMR_BITBLT(const TEmfBitBlt& oTEmfBitBlt, CDataStream &oDataStream, const unsigned int &unRecordSize);
        void Save_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice& oTEmfSetDiBitsToDevice, CDataStream &oDataStream, const unsigned int &unRecordSize);
        void Save_EMR_STRETCHBLT(const TEmfStretchBLT& oTEmfStretchBLT, CDataStream &oDataStream, const unsigned int &unRecordSize);
        void Save_EMR_EOF(const unsigned int& unCount, const unsigned int& unOffset, const unsigned int& unSizeLast);
        void Save_EMR_SAVEDC();
        void Save_EMR_RESTOREDC(const int &nIndexDC);
        void Save_EMR_MODIFYWORLDTRANSFORM(const TEmfXForm& oXForm, const unsigned int& unMode);
        void Save_EMR_SETWORLDTRANSFORM(const TEmfXForm& oXForm);
        void Save_EMR_CREATEBRUSHINDIRECT(const unsigned int& unBrushIndex, const CEmfLogBrushEx* pBrush);
        void Save_EMR_SETTEXTCOLOR(const TEmfColor& oColor);
        void Save_EMR_SELECTOBJECT(const unsigned int& unObjectIndex);
        void Save_EMR_EXTCREATEFONTINDIRECTW(const unsigned int& unIndex, CEmfLogFont* oLogFont, const unsigned int unRecordSize);
        void Save_EMR_SETTEXTALIGN(const unsigned int &unAlign);
        void Save_EMR_SETBKMODE(const unsigned int &unBgMode);
        void Save_EMR_DELETEOBJECT(const unsigned int &unObjectIndex);
        void Save_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit);
        void Save_EMR_EXTCREATEPEN(const unsigned int& unPenIndex, CEmfLogPen* pPen, const unsigned int& unRecordSize);
        void Save_EMR_CREATEPEN(const unsigned int& unPenIndex, const unsigned int& unWidthX, const CEmfLogPen* pPen);
        void Save_EMR_SETPOLYFILLMODE(const unsigned int& unFillMode);
        void Save_EMR_BEGINPATH();
        void Save_EMR_ENDPATH();
        void Save_EMR_CLOSEFIGURE();
        void Save_EMR_FLATTENPATH();
        void Save_EMR_WIDENPATH();
        void Save_EMR_ABORTPATH();
        void Save_EMR_MOVETOEX(const TEmfPointL& oPoint);
        void Save_EMR_SETARCDIRECTION(const unsigned int& unDirection);
        void Save_EMR_FILLPATH(const TEmfRectL& oBounds);
        void Save_EMR_SETMAPMODE(const unsigned int& unMapMode);
        void Save_EMR_SETWINDOWORGEX(const TEmfPointL& oOrigin);
        void Save_EMR_SETWINDOWEXTEX(const TEmfSizeL& oExtent);
        void Save_EMR_SETVIEWPORTORGEX(const TEmfPointL& oOrigin);
        void Save_EMR_SETVIEWPORTEXTEX(const TEmfSizeL& oExtent);
        void Save_EMR_SETSTRETCHBLTMODE(const unsigned int& unStretchMode);
        void Save_EMR_SETICMMODE(const unsigned int& unICMMode);
        void Save_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int& unBrushIndex, const TEmfDibPatternBrush& oDibBrush, CDataStream &oDataStream, const unsigned int &unRecordSize);
        void Save_EMR_SELECTCLIPPATH(const unsigned int& unRegionMode);
        void Save_EMR_SETBKCOLOR(const TEmfColor& oColor);
        void Save_EMR_EXCLUDECLIPRECT(const TEmfRectL& oClip);
        void Save_EMR_EXTSELECTCLIPRGN(const unsigned int& unRgnDataSize, const unsigned int& unRegionMode, CDataStream &oDataStream, const unsigned int &unRecordSize);
        void Save_EMR_SETMETARGN();
        void Save_EMR_SETROP2(const unsigned int& unRop2Mode);
        void Save_EMR_CREATEPALETTE(const unsigned int& unPaletteIndex, const CEmfLogPalette* oEmfLogPalette, const unsigned int &unRecordSize);
        void Save_EMR_SELECTPALETTE(const unsigned int& unPaletteIndex);
        void Save_EMR_REALIZEPALETTE();
        void Save_EMR_INTERSECTCLIPRECT(const TEmfRectL& oClip);
        void Save_EMR_SETLAYOUT(const unsigned int& unLayoutMode);
        void Save_EMR_SETBRUSHORGEX(const TEmfPointL& oOrigin);
        void Save_EMR_ANGLEARC(const TEmfPointL& oCenter, const unsigned int& unRadius, const double& dStartAngle, const double& dSweepAngle);
        void Save_EMR_ARC(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd);
        void Save_EMR_ARCTO(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd);
    private:
        CXmlOutput *m_pOutputXml;
    };
}

#endif // CEMFINTERPRETARORXML_H
