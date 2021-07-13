#include "CEmfInterpretator.h"

#include <fstream>

#include <iostream>

namespace MetaFile
{

    CEmfInterpretator::CEmfInterpretator(const wchar_t* wsFilepath) :
        m_pOutStream(new std::ofstream(wsFilepath, std::ios::binary | std::ios::out))
    {}

    CEmfInterpretator::~CEmfInterpretator()
    {
        m_pOutStream->close();
        delete m_pOutStream;
    }

    InterpretatorType CEmfInterpretator::GetType()
    {
        return InterpretatorType::Emf;
    }

    void CEmfInterpretator::HANDLER_EMR_HEADER(const TEmfHeader &oTEmfHeader, CDataStream &oDataStream, const unsigned int &unRecordSize)
    {
        int unExplicitRecordSize    = unRecordSize + 8;
        int unType                  = EMR_HEADER;

        m_pOutStream->write((char *)&unType,                sizeof (int));
        m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

        WriteRectangle(oTEmfHeader.oBounds);
        WriteRectangle(oTEmfHeader.oFrame);

        m_pOutStream->write((char*)&oTEmfHeader.ulSignature,        sizeof (unsigned int));
        m_pOutStream->write((char*)&oTEmfHeader.ulVersion,          sizeof (unsigned int));
        m_pOutStream->write((char*)&oTEmfHeader.ulSize,             sizeof (unsigned int));
        m_pOutStream->write((char*)&oTEmfHeader.ulRecords,          sizeof (unsigned int));
        m_pOutStream->write((char*)&oTEmfHeader.ushObjects,         sizeof (unsigned short));
        m_pOutStream->write((char*)&oTEmfHeader.ushReserved,        sizeof (unsigned short));
        m_pOutStream->write((char*)&oTEmfHeader.ulSizeDescription,  sizeof (unsigned int));
        m_pOutStream->write((char*)&oTEmfHeader.ulOffsetDescription,sizeof (unsigned int));
        m_pOutStream->write((char*)&oTEmfHeader.ulPalEntries,       sizeof (unsigned int));

        WriteSize(oTEmfHeader.oDevice);
        WriteSize(oTEmfHeader.oMillimeters);
    }

    void CEmfInterpretator::HANDLER_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream, const unsigned int &unRecordSize)
    {
    }

    void CEmfInterpretator::HANDLER_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream, const unsigned int &unRecordSize)
    {
        int unExplicitRecordSize    = unRecordSize + 8;
        int unType                  = EMR_BITBLT;

        m_pOutStream->write((char *)&unType,                sizeof (int));
        m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

        WriteRectangle(oTEmfBitBlt.Bounds);

        m_pOutStream->write((char *)&oTEmfBitBlt.xDest,     sizeof (int));
        m_pOutStream->write((char *)&oTEmfBitBlt.yDest,     sizeof (int));
        m_pOutStream->write((char *)&oTEmfBitBlt.cxDest,    sizeof (int));
        m_pOutStream->write((char *)&oTEmfBitBlt.cyDest,    sizeof (int));
        m_pOutStream->write((char *)&oTEmfBitBlt.\
                                    BitBltRasterOperation,  sizeof (unsigned int));
        m_pOutStream->write((char *)&oTEmfBitBlt.xSrc,      sizeof (int));
        m_pOutStream->write((char *)&oTEmfBitBlt.ySrc,      sizeof (int));

        WriteForm(oTEmfBitBlt.XfromSrc);
        WriteColor(oTEmfBitBlt.BkColorSrc);

        m_pOutStream->write((char *)&oTEmfBitBlt.UsageSrc,  sizeof (unsigned int));
        m_pOutStream->write((char *)&oTEmfBitBlt.offBmiSrc, sizeof (unsigned int));
        m_pOutStream->write((char *)&oTEmfBitBlt.cbBmiSrc,  sizeof (unsigned int));
        m_pOutStream->write((char *)&oTEmfBitBlt.offBitsSrc,sizeof (unsigned int));
        m_pOutStream->write((char *)&oTEmfBitBlt.cbBitsSrc, sizeof (unsigned int));

        if (oTEmfBitBlt.cbBitsSrc == 0)
            return;

        m_pOutStream->write((char *)oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfBitBlt.cbBitsSrc);
    }

    void CEmfInterpretator::HANDLER_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_EOF(const unsigned int &unCount, const unsigned int &unOffset, const unsigned int &unSizeLast)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SAVEDC()
    {

    }

    void CEmfInterpretator::HANDLER_EMR_RESTOREDC(const int &nIndexDC)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
    {
        int unExplicitRecordSize    = 24;
        int unType                  = EMR_CREATEBRUSHINDIRECT;

        m_pOutStream->write((char *)&unType,                sizeof (int));
        m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

        m_pOutStream->write((char *)&unBrushIndex,          sizeof (unsigned int));
        m_pOutStream->write((char *)&(pBrush->BrushStyle),  sizeof (unsigned int));

        WriteColor(pBrush->Color);

        m_pOutStream->write((char *)&(pBrush->BrushHatch),  sizeof (unsigned int));
    }

    void CEmfInterpretator::HANDLER_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
    {
        int unExplicitRecordSize    = 12;
        int unType                  = EMR_SELECTOBJECT;

        m_pOutStream->write((char *)&unType,                sizeof (int));
        m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

        m_pOutStream->write((char *)&unObjectIndex,  sizeof (unsigned int));
    }

    void CEmfInterpretator::HANDLER_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont, const unsigned int unRecordSize)
    {
        int unExplicitRecordSize    = unRecordSize + 8;
        int unType                  = EMR_EXTCREATEFONTINDIRECTW;

        m_pOutStream->write((char *)&unType,                sizeof (int));
        m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

        m_pOutStream->write((char *)&unIndex,               sizeof (unsigned int));

        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.Height, sizeof (int));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.Width,  sizeof (int));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.Escapement,  sizeof (int));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.Orientation,  sizeof (int));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.Weight,  sizeof (int));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.Italic,  sizeof (unsigned char));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.Underline,  sizeof (unsigned char));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.StrikeOut,  sizeof (unsigned char));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.CharSet,  sizeof (unsigned char));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.OutPrecision,  sizeof (unsigned char));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.ClipPrecision,  sizeof (unsigned char));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.Quality,  sizeof (unsigned char));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.PitchAndFamily ,  sizeof (unsigned char));
        m_pOutStream->write((char *)&oLogFont->LogFontEx.LogFont.FaceName, sizeof (unsigned short) * 16);
        m_pOutStream->write((char *)&oLogFont->LogFontEx.FullName,      sizeof (unsigned short) * 32);
        m_pOutStream->write((char *)&oLogFont->LogFontEx.Style,         sizeof (unsigned short) * 32);
        m_pOutStream->write((char *)&oLogFont->LogFontEx.Script,        sizeof (unsigned short) * 32);

        if (!oLogFont->IsFixedLength())
        {
            m_pOutStream->write((char *)&oLogFont->DesignVector.Signature,     sizeof (unsigned int));
            m_pOutStream->write((char *)&oLogFont->DesignVector.NumAxes,       sizeof (unsigned int));

            for (unsigned int i = 0; i < oLogFont->DesignVector.NumAxes; ++i)
                m_pOutStream->write((char *)&oLogFont->DesignVector.Values[i], sizeof (int));
        }
    }

    void CEmfInterpretator::HANDLER_EMR_SETTEXTALIGN(const unsigned int &unAlign)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETBKMODE(const unsigned int &unBgMode)
    {
        int unExplicitRecordSize    = 12;
        int unType                  = EMR_SETBKMODE;

        m_pOutStream->write((char *)&unType,                sizeof (int));
        m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

        m_pOutStream->write((char *)&unBgMode,              sizeof (unsigned int));
    }

    void CEmfInterpretator::HANDLER_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_BEGINPATH()
    {

    }

    void CEmfInterpretator::HANDLER_EMR_ENDPATH()
    {

    }

    void CEmfInterpretator::HANDLER_EMR_CLOSEFIGURE()
    {

    }

    void CEmfInterpretator::HANDLER_EMR_FLATTENPATH()
    {

    }

    void CEmfInterpretator::HANDLER_EMR_WIDENPATH()
    {

    }

    void CEmfInterpretator::HANDLER_EMR_ABORTPATH()
    {

    }

    void CEmfInterpretator::HANDLER_EMR_MOVETOEX(const TEmfPointL &oPoint)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETARCDIRECTION(const unsigned int &unDirection)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_FILLPATH(const TEmfRectL &oBounds)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETMAPMODE(const unsigned int &unMapMode)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETICMMODE(const unsigned int &unICMMode)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETBKCOLOR(const TEmfColor &oColor)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETMETARGN()
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETROP2(const unsigned int &unRop2Mode)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_REALIZEPALETTE()
    {

    }

    void CEmfInterpretator::HANDLER_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_ELLIPSE(const TEmfRectL &oBox)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_LINETO(const TEmfPointL &oPoint)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_RECTANGLE(const TEmfRectL &oBox)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText, const unsigned int &unRecordSize)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
    {

    }

    void CEmfInterpretator::HANDLER_EMR_STROKEPATH(const TEmfRectL &oBounds)
    {

    }

    void CEmfInterpretator::WriteRectangle(const TEmfRectL &oBounds)
    {
        m_pOutStream->write((char*)&oBounds.lLeft,   sizeof(int));
        m_pOutStream->write((char*)&oBounds.lTop,    sizeof(int));
        m_pOutStream->write((char*)&oBounds.lRight,  sizeof(int));
        m_pOutStream->write((char*)&oBounds.lBottom, sizeof(int));
    }

    void CEmfInterpretator::WriteSize(const TEmfSizeL &oSize)
    {
        m_pOutStream->write((char*)&oSize.cx,   sizeof(unsigned int));
        m_pOutStream->write((char*)&oSize.cy,   sizeof(unsigned int));
    }

    void CEmfInterpretator::WriteColor(const TEmfColor &oColor)
    {
        m_pOutStream->write((char*)&oColor.r, sizeof (unsigned char));
        m_pOutStream->write((char*)&oColor.g, sizeof (unsigned char));
        m_pOutStream->write((char*)&oColor.b, sizeof (unsigned char));
        m_pOutStream->write((char*)&oColor.a, sizeof (unsigned char));
    }

    void CEmfInterpretator::WriteForm(const TXForm &oForm)
    {
        m_pOutStream->write((char*)&oForm.M11,  sizeof (float));
        m_pOutStream->write((char*)&oForm.M12,  sizeof (float));
        m_pOutStream->write((char*)&oForm.M21,  sizeof (float));
        m_pOutStream->write((char*)&oForm.M22,  sizeof (float));
        m_pOutStream->write((char*)&oForm.Dx,   sizeof (float));
        m_pOutStream->write((char*)&oForm.Dy,   sizeof (float));
    }
}


