#include "CEmfInterpretatorAll.h"

namespace MetaFile
{
        CEmfInterpretatorAll::CEmfInterpretatorAll(IOutputDevice* pIOutputDevice, const wchar_t *wsFilePath):
                CEmfInterpretatorRender(pIOutputDevice),
                CEmfInterpretatorXml(wsFilePath){}

        InterpretatorType CEmfInterpretatorAll::GetType()
        {
                return InterpretatorType::RenderAndXml;
        }

        void CEmfInterpretatorAll::Begin()
        {
                CEmfInterpretatorRender ::Begin();
                CEmfInterpretatorXml    ::Begin();
        }

        void CEmfInterpretatorAll::End()
        {
                CEmfInterpretatorRender ::End();
                CEmfInterpretatorXml    ::End();
        }

        void CEmfInterpretatorAll::DrawBitmap(double dX, double dY, double dW, double dH, BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight)
        {
                CEmfInterpretatorRender::DrawBitmap(dX, dY, dW, dH, pBuffer, unWidth, unHeight);
        }

        void CEmfInterpretatorAll::DrawString(std::wstring &wsText, unsigned int unCharsCount, double dX, double dY, double *pDx, int iGraphicsMode)
        {
                CEmfInterpretatorRender::DrawString(wsText, unCharsCount, dX, dY, pDx, iGraphicsMode);
        }

        void CEmfInterpretatorAll::StartPath()
        {
                CEmfInterpretatorRender::StartPath();
        }

        void CEmfInterpretatorAll::MoveTo(double dX, double dY)
        {
                CEmfInterpretatorRender::MoveTo(dX, dY);
        }

        void CEmfInterpretatorAll::LineTo(double dX, double dY)
        {
                CEmfInterpretatorRender::LineTo(dX, dY);
        }

        void CEmfInterpretatorAll::CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe)
        {
                CEmfInterpretatorRender::CurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
        }

        void CEmfInterpretatorAll::ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
        {
                CEmfInterpretatorRender::ArcTo(dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
        }

        void CEmfInterpretatorAll::ClosePath()
        {
                CEmfInterpretatorRender::ClosePath();
        }

        void CEmfInterpretatorAll::DrawPath(int nType)
        {
                CEmfInterpretatorRender::DrawPath(nType);
        }

        void CEmfInterpretatorAll::EndPath()
        {
                CEmfInterpretatorRender::EndPath();
        }

        void CEmfInterpretatorAll::ResetClip()
        {
                CEmfInterpretatorRender::ResetClip();
        }

        void CEmfInterpretatorAll::IntersectClip(double dLeft, double dTop, double dRight, double dBottom)
        {
                CEmfInterpretatorRender::IntersectClip(dLeft, dTop, dRight, dBottom);
        }

        void CEmfInterpretatorAll::StartClipPath(unsigned int unMode, int nFillMode)
        {
                CEmfInterpretatorRender::StartClipPath(unMode, nFillMode);
        }

        void CEmfInterpretatorAll::EndClipPath(unsigned int unMode)
        {
                CEmfInterpretatorRender::EndClipPath(unMode);
        }

        void CEmfInterpretatorAll::UpdateDC()
        {
                CEmfInterpretatorRender::UpdateDC();
        }

        void CEmfInterpretatorAll::SetTransform(double &dM11, double &dM12, double &dM21, double &dM22, double &dX, double &dY)
        {
                CEmfInterpretatorRender::SetTransform(dM11, dM12, dM21, dM22, dX, dY);
        }

        void CEmfInterpretatorAll::GetTransform(double *pdM11, double *pdM12, double *pdM21, double *pdM22, double *pdX, double *pdY)
        {
                CEmfInterpretatorRender::GetTransform(pdM11, pdM12, pdM21, pdM22, pdX, pdY);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader)
        {
                CEmfInterpretatorXml::HANDLE_EMR_HEADER(oTEmfHeader);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend &oTEmfAlphaBlend, CDataStream &oDataStream)
        {
                CEmfInterpretatorXml::HANDLE_EMR_ALPHABLEND(oTEmfAlphaBlend, oDataStream);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS &oTEmfStretchDIBITS, CDataStream &oDataStream)
        {
                CEmfInterpretatorXml::HANDLE_EMR_STRETCHDIBITS(oTEmfStretchDIBITS, oDataStream);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_BITBLT(const TEmfBitBlt &oTEmfBitBlt, CDataStream &oDataStream)
        {
                CEmfInterpretatorXml::HANDLE_EMR_BITBLT(oTEmfBitBlt, oDataStream);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice &oTEmfSetDiBitsToDevice, CDataStream &oDataStream)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETDIBITSTODEVICE(oTEmfSetDiBitsToDevice, oDataStream);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT &oTEmfStretchBLT, CDataStream &oDataStream)
        {
                CEmfInterpretatorXml::HANDLE_EMR_STRETCHBLT(oTEmfStretchBLT, oDataStream);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_EOF(const unsigned int &unCount, const unsigned int &unOffset, const unsigned int &unSizeLast)
        {
                CEmfInterpretatorXml::HANDLE_EMR_EOF(unCount, unOffset, unSizeLast);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SAVEDC()
        {
                CEmfInterpretatorXml::HANDLE_EMR_SAVEDC();
        }

        void CEmfInterpretatorAll::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
        {
                CEmfInterpretatorXml::HANDLE_EMR_RESTOREDC(nIndexDC);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_MODIFYWORLDTRANSFORM(const TXForm &oXForm, const unsigned int &unMode)
        {
                CEmfInterpretatorXml::HANDLE_EMR_MODIFYWORLDTRANSFORM(oXForm, unMode);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETWORLDTRANSFORM(const TXForm &oXForm)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETWORLDTRANSFORM(oXForm);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int &unBrushIndex, const CEmfLogBrushEx *pBrush)
        {
                CEmfInterpretatorXml::HANDLE_EMR_CREATEBRUSHINDIRECT(unBrushIndex, pBrush);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETTEXTCOLOR(const TEmfColor &oColor)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETTEXTCOLOR(oColor);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SELECTOBJECT(const unsigned int &unObjectIndex)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SELECTOBJECT(unObjectIndex);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_EXTCREATEFONTINDIRECTW(const unsigned int &unIndex, CEmfLogFont *oLogFont)
        {
                CEmfInterpretatorXml::HANDLE_EMR_EXTCREATEFONTINDIRECTW(unIndex, oLogFont);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETTEXTALIGN(const unsigned int &unAlign)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETTEXTALIGN(unAlign);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETBKMODE(const unsigned int &unBgMode)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETBKMODE(unBgMode);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_DELETEOBJECT(const unsigned int &unObjectIndex)
        {
                CEmfInterpretatorXml::HANDLE_EMR_DELETEOBJECT(unObjectIndex);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETMITERLIMIT(unMeterLimit);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_EXTCREATEPEN(const unsigned int &unPenIndex, CEmfLogPen *pPen)
        {
                CEmfInterpretatorXml::HANDLE_EMR_EXTCREATEPEN(unPenIndex, pPen);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_CREATEPEN(const unsigned int &unPenIndex, const unsigned int &unWidthX, const CEmfLogPen *pPen)
        {
                CEmfInterpretatorXml::HANDLE_EMR_CREATEPEN(unPenIndex, unWidthX, pPen);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETPOLYFILLMODE(const unsigned int &unFillMode)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETPOLYFILLMODE(unFillMode);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_BEGINPATH()
        {
                CEmfInterpretatorXml::HANDLE_EMR_BEGINPATH();
        }

        void CEmfInterpretatorAll::HANDLE_EMR_ENDPATH()
        {
                CEmfInterpretatorXml::HANDLE_EMR_ENDPATH();
        }

        void CEmfInterpretatorAll::HANDLE_EMR_CLOSEFIGURE()
        {
                CEmfInterpretatorXml::HANDLE_EMR_CLOSEFIGURE();
        }

        void CEmfInterpretatorAll::HANDLE_EMR_FLATTENPATH()
        {
                CEmfInterpretatorXml::HANDLE_EMR_FLATTENPATH();
        }

        void CEmfInterpretatorAll::HANDLE_EMR_WIDENPATH()
        {
                CEmfInterpretatorXml::HANDLE_EMR_WIDENPATH();
        }

        void CEmfInterpretatorAll::HANDLE_EMR_ABORTPATH()
        {
                CEmfInterpretatorXml::HANDLE_EMR_ABORTPATH();
        }

        void CEmfInterpretatorAll::HANDLE_EMR_MOVETOEX(const TEmfPointL &oPoint)
        {
                CEmfInterpretatorXml::HANDLE_EMR_MOVETOEX(oPoint);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETARCDIRECTION(const unsigned int &unDirection)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETARCDIRECTION(unDirection);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_FILLPATH(const TEmfRectL &oBounds)
        {
                CEmfInterpretatorXml::HANDLE_EMR_FILLPATH(oBounds);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETMAPMODE(const unsigned int &unMapMode)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETMAPMODE(unMapMode);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETWINDOWORGEX(const TEmfPointL &oOrigin)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETWINDOWORGEX(oOrigin);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETWINDOWEXTEX(const TEmfSizeL &oExtent)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETWINDOWEXTEX(oExtent);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL &oOrigin)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETVIEWPORTORGEX(oOrigin);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL &oExtent)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETVIEWPORTEXTEX(oExtent);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETSTRETCHBLTMODE(const unsigned int &unStretchMode)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETSTRETCHBLTMODE(unStretchMode);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETICMMODE(const unsigned int &unICMMode)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETICMMODE(unICMMode);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int &unBrushIndex, const TEmfDibPatternBrush &oDibBrush, CDataStream &oDataStream)
        {
                CEmfInterpretatorXml::HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(unBrushIndex, oDibBrush, oDataStream);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SELECTCLIPPATH(unRegionMode);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETBKCOLOR(const TEmfColor &oColor)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETBKCOLOR(oColor);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
        {
                CEmfInterpretatorXml::HANDLE_EMR_EXCLUDECLIPRECT(oClip);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
        {
                CEmfInterpretatorXml::HANDLE_EMR_EXTSELECTCLIPRGN(unRgnDataSize, unRegionMode, oDataStream);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETMETARGN()
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETMETARGN();
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETROP2(const unsigned int &unRop2Mode)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETROP2(unRop2Mode);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_CREATEPALETTE(const unsigned int &unPaletteIndex, const CEmfLogPalette *oEmfLogPalette)
        {
                CEmfInterpretatorXml::HANDLE_EMR_CREATEPALETTE(unPaletteIndex, oEmfLogPalette);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SELECTPALETTE(const unsigned int &unPaletteIndex)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SELECTPALETTE(unPaletteIndex);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_REALIZEPALETTE()
        {
                CEmfInterpretatorXml::HANDLE_EMR_REALIZEPALETTE();
        }

        void CEmfInterpretatorAll::HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
        {
                CEmfInterpretatorXml::HANDLE_EMR_INTERSECTCLIPRECT(oClip);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETLAYOUT(const unsigned int &unLayoutMode)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETLAYOUT(unLayoutMode);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL &oOrigin)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETBRUSHORGEX(oOrigin);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
        {
                CEmfInterpretatorXml::HANDLE_EMR_ANGLEARC(oCenter, unRadius, dStartAngle, dSweepAngle);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {
                CEmfInterpretatorXml::HANDLE_EMR_ARC(oBox, oStart, oEnd);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {
                CEmfInterpretatorXml::HANDLE_EMR_ARCTO(oBox, oStart, oEnd);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {
                CEmfInterpretatorXml::HANDLE_EMR_CHORD(oBox, oStart, oEnd);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
        {
                CEmfInterpretatorXml::HANDLE_EMR_ELLIPSE(oBox);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
        {
                CEmfInterpretatorXml::HANDLE_EMR_EXTTEXTOUTA(oTEmfExtTextoutA);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
        {
                CEmfInterpretatorXml::HANDLE_EMR_EXTTEXTOUTW(oTEmfExtTextoutW);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
        {
                CEmfInterpretatorXml::HANDLE_EMR_LINETO(oPoint);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
        {
                CEmfInterpretatorXml::HANDLE_EMR_PIE(oBox, oStart, oEnd);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYBEZIER(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYBEZIER(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYBEZIERTO(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYBEZIERTO(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYDRAW(oBounds, arPoints, unCount, pAbTypes);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYDRAW(oBounds, arPoints, unCount, pAbTypes);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYGON(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYGON(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYLINE(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYLINE(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYLINETO(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
        {
                CEmfInterpretatorXml::HANDLE_EMR_POLYLINETO(oBounds, arPoints);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_RECTANGLE(const TEmfRectL &oBox)
        {
                CEmfInterpretatorXml::HANDLE_EMR_RECTANGLE(oBox);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
        {
                CEmfInterpretatorXml::HANDLE_EMR_ROUNDRECT(oBox, oCorner);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SETPIXELV(oPoint, oColor);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
        {
                CEmfInterpretatorXml::HANDLE_EMR_SMALLTEXTOUT(oText);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
        {
                CEmfInterpretatorXml::HANDLE_EMR_STROKEANDFILLPATH(oBounds);
        }

        void CEmfInterpretatorAll::HANDLE_EMR_STROKEPATH(const TEmfRectL &oBounds)
        {
                CEmfInterpretatorXml::HANDLE_EMR_STROKEPATH(oBounds);
        }
}
