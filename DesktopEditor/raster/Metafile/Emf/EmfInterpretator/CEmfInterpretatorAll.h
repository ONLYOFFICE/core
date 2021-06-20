#ifndef CEMFINTERPRETATORALL_H
#define CEMFINTERPRETATORALL_H

#include "CEmfInterpretatorRender.h"
#include "CEmfInterpretatorXml.h"

namespace MetaFile
{
    class CEmfInterpretatorAll : public CEmfInterpretatorRender, public CEmfInterpretatorXml
    {
    public:
        CEmfInterpretatorAll();

        //CEmfInterpretatorRender
        void SetOutputDevice(IOutputDevice* pIOutputDevice) override;

        void Begin() override;
        void End() override;

        void DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight) override;

        void DrawString(std::wstring& wsText, unsigned int unCharsCount, double dX, double dY, double* pDx, int iGraphicsMode = 1) override;

	void StartPath() override;
	void MoveTo(double dX, double dY) override;
	void LineTo(double dX, double dY) override;
	void CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe) override;
	void ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle) override;
	void ClosePath() override;
	void DrawPath(int nType = 0) override;
	void EndPath() override;

	void ResetClip() override;
	void IntersectClip(double dLeft, double dTop, double dRight, double dBottom) override;
	void StartClipPath(unsigned int unMode, int nFillMode = -1) override;
	void EndClipPath(unsigned int unMode) override;

	void UpdateDC() override;
	void SetTransform(double& dM11, double& dM12, double& dM21, double& dM22, double& dX, double& dY) override;
	void GetTransform(double* pdM11, double* pdM12, double* pdM21, double* pdM22, double* pdX, double* pdY) override;

	//CEmfInterpretatorXml
	void Save_EMR_HEADER(const TEmfHeader& oTEmfHeader, CDataStream &oDataStream, const unsigned int &unRecordSize) override ;
	void Save_EMR_ALPHABLEND(const TEmfAlphaBlend& oTEmfAlphaBlend, CDataStream &oDataStream, const unsigned int &unRecordSize) override ;
	void Save_EMR_STRETCHDIBITS(const TEmfStretchDIBITS& oTEmfStretchDIBITS, CDataStream &oDataStream, const unsigned int &unRecordSize) override ;
	void Save_EMR_BITBLT(const TEmfBitBlt& oTEmfBitBlt, CDataStream &oDataStream, const unsigned int &unRecordSize) override ;
	void Save_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice& oTEmfSetDiBitsToDevice, CDataStream &oDataStream, const unsigned int &unRecordSize) override ;
	void Save_EMR_STRETCHBLT(const TEmfStretchBLT& oTEmfStretchBLT, CDataStream &oDataStream, const unsigned int &unRecordSize) override ;
	void Save_EMR_EOF(const unsigned int& unCount, const unsigned int& unOffset, const unsigned int& unSizeLast) override ;
	void Save_EMR_SAVEDC() override ;
	void Save_EMR_RESTOREDC(const int &nIndexDC) override ;
	void Save_EMR_MODIFYWORLDTRANSFORM(const TEmfXForm& oXForm, const unsigned int& unMode) override ;
	void Save_EMR_SETWORLDTRANSFORM(const TEmfXForm& oXForm) override ;
	void Save_EMR_CREATEBRUSHINDIRECT(const unsigned int& unBrushIndex, const CEmfLogBrushEx* pBrush) override ;
	void Save_EMR_SETTEXTCOLOR(const TEmfColor& oColor) override ;
	void Save_EMR_SELECTOBJECT(const unsigned int& unObjectIndex) override ;
	void Save_EMR_EXTCREATEFONTINDIRECTW(const unsigned int& unIndex, CEmfLogFont* oLogFont, const unsigned int unRecordSize) override ;
	void Save_EMR_SETTEXTALIGN(const unsigned int &unAlign) override ;
	void Save_EMR_SETBKMODE(const unsigned int &unBgMode) override ;
	void Save_EMR_DELETEOBJECT(const unsigned int &unObjectIndex) override ;
	void Save_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit) override ;
	void Save_EMR_EXTCREATEPEN(const unsigned int& unPenIndex, CEmfLogPen* pPen, const unsigned int& unRecordSize) override ;
	void Save_EMR_CREATEPEN(const unsigned int& unPenIndex, const unsigned int& unWidthX, const CEmfLogPen* pPen) override ;
	void Save_EMR_SETPOLYFILLMODE(const unsigned int& unFillMode) override ;
	void Save_EMR_BEGINPATH() override ;
	void Save_EMR_ENDPATH() override ;
	void Save_EMR_CLOSEFIGURE() override ;
	void Save_EMR_FLATTENPATH() override ;
	void Save_EMR_WIDENPATH() override ;
	void Save_EMR_ABORTPATH() override ;
	void Save_EMR_MOVETOEX(const TEmfPointL& oPoint) override ;
	void Save_EMR_SETARCDIRECTION(const unsigned int& unDirection) override ;
	void Save_EMR_FILLPATH(const TEmfRectL& oBounds) override ;
	void Save_EMR_SETMAPMODE(const unsigned int& unMapMode) override ;
	void Save_EMR_SETWINDOWORGEX(const TEmfPointL& oOrigin) override ;
	void Save_EMR_SETWINDOWEXTEX(const TEmfSizeL& oExtent) override ;
	void Save_EMR_SETVIEWPORTORGEX(const TEmfPointL& oOrigin) override ;
	void Save_EMR_SETVIEWPORTEXTEX(const TEmfSizeL& oExtent) override ;
	void Save_EMR_SETSTRETCHBLTMODE(const unsigned int& unStretchMode) override ;
	void Save_EMR_SETICMMODE(const unsigned int& unICMMode) override ;
	void Save_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int& unBrushIndex, const TEmfDibPatternBrush& oDibBrush, CDataStream &oDataStream, const unsigned int &unRecordSize) override ;
	void Save_EMR_SELECTCLIPPATH(const unsigned int& unRegionMode) override ;
	void Save_EMR_SETBKCOLOR(const TEmfColor& oColor) override ;
	void Save_EMR_EXCLUDECLIPRECT(const TEmfRectL& oClip) override ;
	void Save_EMR_EXTSELECTCLIPRGN(const unsigned int& unRgnDataSize, const unsigned int& unRegionMode, CDataStream &oDataStream, const unsigned int &unRecordSize) override ;
	void Save_EMR_SETMETARGN() override ;
	void Save_EMR_SETROP2(const unsigned int& unRop2Mode) override ;
	void Save_EMR_CREATEPALETTE(const unsigned int& unPaletteIndex, const CEmfLogPalette* oEmfLogPalette, const unsigned int &unRecordSize) override ;
	void Save_EMR_SELECTPALETTE(const unsigned int& unPaletteIndex) override ;
	void Save_EMR_REALIZEPALETTE() override ;
	void Save_EMR_INTERSECTCLIPRECT(const TEmfRectL& oClip) override ;
	void Save_EMR_SETLAYOUT(const unsigned int& unLayoutMode) override ;
	void Save_EMR_SETBRUSHORGEX(const TEmfPointL& oOrigin) override ;
	void Save_EMR_ANGLEARC(const TEmfPointL& oCenter, const unsigned int& unRadius, const double& dStartAngle, const double& dSweepAngle) override ;
	void Save_EMR_ARC(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd) override ;
	void Save_EMR_ARCTO(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd) override ;
	void Save_EMR_CHORD(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd) override ;
	void Save_EMR_ELLIPSE(const TEmfRectL& oBox) override ;
	void Save_EMR_EXTTEXTOUTA(const TEmfExtTextoutA& oTEmfExtTextoutA, const unsigned int &unRecordSize) override ;
	void Save_EMR_EXTTEXTOUTW(const TEmfExtTextoutW& oTEmfExtTextoutW, const unsigned int &unRecordSize) override ;
	void Save_EMR_LINETO(const TEmfPointL& oPoint) override ;
	void Save_EMR_PIE(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd) override ;
	void Save_EMR_POLYBEZIER(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override ;
	void Save_EMR_POLYBEZIER(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override ;
	void Save_EMR_POLYBEZIERTO(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override ;
	void Save_EMR_POLYBEZIERTO(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override ;
	void Save_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes) override ;
	void Save_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes) override ;
	void Save_EMR_POLYGON(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override ;
	void Save_EMR_POLYGON(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override ;
	void Save_EMR_POLYLINE(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override ;
	void Save_EMR_POLYLINE(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override ;
	void Save_EMR_POLYLINETO(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override ;
	void Save_EMR_POLYLINETO(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override ;
	//TODO: Реализовать сохранение полигонов в полигоне
	void Save_EMR_RECTANGLE(const TEmfRectL& oBox) override ;
	void Save_EMR_ROUNDRECT(const TEmfRectL& oBox, const TEmfSizeL& oCorner) override ;
	void Save_EMR_SETPIXELV(const TEmfPointL& oPoint, const TEmfColor& oColor) override ;
	void Save_EMR_SMALLTEXTOUT(const TEmfSmallTextout& oText, const unsigned int& unRecordSize) override ;
	void Save_EMR_STROKEANDFILLPATH(const TEmfRectL& oBounds) override ;
	void Save_EMR_STROKEPATH(const TEmfRectL& oBounds) override ;
    };
}

#endif // CEMFINTERPRETATORALL_H
