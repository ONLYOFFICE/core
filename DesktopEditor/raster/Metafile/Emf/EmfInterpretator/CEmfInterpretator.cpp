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

        void CEmfInterpretator::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader, CDataStream &oDataStream, const unsigned int &unRecordSize)
        {
                int unExplicitRecordSize    = 88;
                int unType                  = EMR_HEADER;

                unFileSize += unExplicitRecordSize;

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

        void CEmfInterpretator::HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream, const unsigned int &unRecordSize)
        {
        }

        void CEmfInterpretator::HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream, const unsigned int &unRecordSize)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream, const unsigned int &unRecordSize)
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

                if (oTEmfBitBlt.cbBitsSrc > 0)
                        m_pOutStream->write((char *)oDataStream.GetCurPtr(), sizeof (BYTE) * oTEmfBitBlt.cbBitsSrc);
        }

        void CEmfInterpretator::HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream, const unsigned int &unRecordSize)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream, const unsigned int &unRecordSize)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_EOF(const unsigned int &unCount, const unsigned int &unOffset, const unsigned int &unSizeLast)
        {
                int unExplicitRecordSize    = 20;
                int unType                  = EMR_EOF;

                m_pOutStream->write((char *)&unType,                 sizeof (int));
                m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

                m_pOutStream->write((char *)&unCount,               sizeof (unsigned int));
                m_pOutStream->write((char *)&unOffset,              sizeof (unsigned int));
                m_pOutStream->write((char *)&unSizeLast,            sizeof (unsigned int));
        }

        void CEmfInterpretator::HANDLE_EMR_SAVEDC()
        {

        }

        void CEmfInterpretator::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
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

        void CEmfInterpretator::HANDLE_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
        {
                int unExplicitRecordSize    = 12;
                int unType                  = EMR_SELECTOBJECT;

                m_pOutStream->write((char *)&unType,                sizeof (int));
                m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

                m_pOutStream->write((char *)&unObjectIndex,  sizeof (unsigned int));
        }

        void CEmfInterpretator::HANDLE_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont, const unsigned int unRecordSize)
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

        void CEmfInterpretator::HANDLE_EMR_SETTEXTALIGN(const unsigned int &unAlign)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETBKMODE(const unsigned int &unBgMode)
        {
                int unExplicitRecordSize    = 12;
                int unType                  = EMR_SETBKMODE;

                m_pOutStream->write((char *)&unType,                sizeof (int));
                m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

                m_pOutStream->write((char *)&unBgMode,              sizeof (unsigned int));
        }

        void CEmfInterpretator::HANDLE_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
        {
                int unExplicitRecordSize    = 12;
                int unType                  = EMR_DELETEOBJECT;

                m_pOutStream->write((char *)&unType,                sizeof (int));
                m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

                m_pOutStream->write((char *)&unObjectIndex,  sizeof (unsigned int));
        }

        void CEmfInterpretator::HANDLE_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen, const unsigned int &unRecordSize)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_BEGINPATH()
        {

        }

        void CEmfInterpretator::HANDLE_EMR_ENDPATH()
        {

        }

        void CEmfInterpretator::HANDLE_EMR_CLOSEFIGURE()
        {

        }

        void CEmfInterpretator::HANDLE_EMR_FLATTENPATH()
        {

        }

        void CEmfInterpretator::HANDLE_EMR_WIDENPATH()
        {

        }

        void CEmfInterpretator::HANDLE_EMR_ABORTPATH()
        {

        }

        void CEmfInterpretator::HANDLE_EMR_MOVETOEX(const TEmfPointL &oPoint)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETARCDIRECTION(const unsigned int &unDirection)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_FILLPATH(const TEmfRectL &oBounds)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETMAPMODE(const unsigned int &unMapMode)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETICMMODE(const unsigned int &unICMMode)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream, const unsigned int &unRecordSize)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETBKCOLOR(const TEmfColor &oColor)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream, const unsigned int &unRecordSize)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETMETARGN()
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETROP2(const unsigned int &unRop2Mode)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette, const unsigned int &unRecordSize)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_REALIZEPALETTE()
        {

        }

        void CEmfInterpretator::HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA, const unsigned int &unRecordSize)
        {
        }

        void CEmfInterpretator::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW, const unsigned int &unRecordSize)
        {
                const unsigned int unDxCount = oTEmfExtTextoutW.wEmrText.Options & ETO_PDY ? 2 * oTEmfExtTextoutW.wEmrText.Chars : oTEmfExtTextoutW.wEmrText.Chars;

                int unExplicitRecordSize    = 76 + oTEmfExtTextoutW.wEmrText.Chars * 2 + unDxCount * 4;
                int unType                  = EMR_EXTTEXTOUTW;

                m_pOutStream->write((char *)&unType,                sizeof (int));
                m_pOutStream->write((char *)&unExplicitRecordSize,  sizeof (int));

                WriteRectangle(oTEmfExtTextoutW.Bounds);

                m_pOutStream->write((char *)&oTEmfExtTextoutW.iGraphicsMode,    sizeof (unsigned int));
                m_pOutStream->write((char *)&oTEmfExtTextoutW.exScale,          sizeof (float));
                m_pOutStream->write((char *)&oTEmfExtTextoutW.eyScale,          sizeof (float));

                WritePoint(oTEmfExtTextoutW.wEmrText.Reference);

                m_pOutStream->write((char *)&oTEmfExtTextoutW.wEmrText.Chars,       sizeof (unsigned int));
                m_pOutStream->write((char *)&oTEmfExtTextoutW.wEmrText.offString,   sizeof (unsigned int));
                m_pOutStream->write((char *)&oTEmfExtTextoutW.wEmrText.Options,     sizeof (unsigned int));

                WriteRectangle(oTEmfExtTextoutW.wEmrText.Rectangle);

                m_pOutStream->write((char *)&oTEmfExtTextoutW.wEmrText.offDx,       sizeof (unsigned int));
                m_pOutStream->write((char *)&oTEmfExtTextoutW.wEmrText.OutputString,sizeof (unsigned short) * oTEmfExtTextoutW.wEmrText.Chars);

                if (unDxCount > 0)
                        m_pOutStream->write((char *)&oTEmfExtTextoutW.wEmrText.OutputDx,sizeof (unsigned int) * unDxCount);
        }

        void CEmfInterpretator::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_RECTANGLE(const TEmfRectL &oBox)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText, const unsigned int &unRecordSize)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
        {

        }

        void CEmfInterpretator::HANDLE_EMR_STROKEPATH(const TEmfRectL &oBounds)
        {

        }

        void CEmfInterpretator::WriteRectangle(const TEmfRectL &oBounds)
        {
                m_pOutStream->write((char *)&oBounds.lLeft,   sizeof(int));
                m_pOutStream->write((char *)&oBounds.lTop,    sizeof(int));
                m_pOutStream->write((char *)&oBounds.lRight,  sizeof(int));
                m_pOutStream->write((char *)&oBounds.lBottom, sizeof(int));
        }

        void CEmfInterpretator::WriteSize(const TEmfSizeL &oSize)
        {
                m_pOutStream->write((char *)&oSize.cx,   sizeof(unsigned int));
                m_pOutStream->write((char *)&oSize.cy,   sizeof(unsigned int));
        }

        void CEmfInterpretator::WriteColor(const TEmfColor &oColor)
        {
                m_pOutStream->write((char *)&oColor.r, sizeof (unsigned char));
                m_pOutStream->write((char *)&oColor.g, sizeof (unsigned char));
                m_pOutStream->write((char *)&oColor.b, sizeof (unsigned char));
                m_pOutStream->write((char *)&oColor.a, sizeof (unsigned char));
        }

        void CEmfInterpretator::WriteForm(const TXForm &oForm)
        {
                m_pOutStream->write((char *)&oForm.M11,  sizeof (float));
                m_pOutStream->write((char *)&oForm.M12,  sizeof (float));
                m_pOutStream->write((char *)&oForm.M21,  sizeof (float));
                m_pOutStream->write((char *)&oForm.M22,  sizeof (float));
                m_pOutStream->write((char *)&oForm.Dx,   sizeof (float));
                m_pOutStream->write((char *)&oForm.Dy,   sizeof (float));
        }

        void CEmfInterpretator::WritePoint(const TEmfPointL &oPoint)
        {
                m_pOutStream->write((char *)&oPoint.x, sizeof (int));
                m_pOutStream->write((char *)&oPoint.y, sizeof (int));
        }
}


