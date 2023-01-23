#ifndef CEMFINTERPRETATORRENDER_H
#define CEMFINTERPRETATORRENDER_H

#include "CEmfInterpretatorBase.h"
#include "../../Common/MetaFileRenderer.h"

namespace MetaFile
{
	class CEmfInterpretatorRender : public CEmfInterpretatorBase
	{
	public:
		CEmfInterpretatorRender(IOutputDevice* pIOutputDevice);

		InterpretatorType   GetType() const override;

		void CreateConditional(IMetaFileBase* pFile) override;
		void ChangeConditional() override;

		void Begin() override;
		void End() override;

		void DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight) override;

		void DrawString(std::wstring& wsText, unsigned int unCharsCount, double dX, double dY, double* pDx,
						int iGraphicsMode = 1, double dXScale = 1, double dYScale = 1) override;

		void DrawDriverString(const std::wstring& wsString, const std::vector<TPointD>& arPoints) override;

		void StartPath() override;
		void MoveTo(double dX, double dY) override;
		void LineTo(double dX, double dY) override;
		void CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe) override;
		void ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle) override;
		void ClosePath() override;
		void DrawPath(int nType = 0) override;
		void EndPath() override;

		void ResetClip() override;
		void IntersectClip(const TRectD& oClip) override;
		void ExcludeClip(const TRectD& oClip, const TRectD& oBB) override;
		void PathClip(IPath* pPath, int nClipMode, TXForm* pTransform = NULL) override;
		void StartClipPath(unsigned int unMode, int nFillMode = -1) override;
		void EndClipPath(unsigned int unMode) override;

		void UpdateDC() override;
		void SetTransform(double& dM11, double& dM12, double& dM21, double& dM22, double& dX, double& dY) override;
		void GetTransform(double* pdM11, double* pdM12, double* pdM21, double* pdM22, double* pdX, double* pdY) override;

		CMetaFileRenderer* GetRenderer() const;

	private:
		CMetaFileRenderer *m_pMetaFileRenderer;

	public:
		//Следующие методы ничего не делают
		void HANDLE_EMR_HEADER(const TEmfHeader& oTEmfHeader) override {};
		void HANDLE_EMR_ALPHABLEND(const TEmfAlphaBlend& oTEmfAlphaBlend, CDataStream &oDataStream) override {};
		void HANDLE_EMR_STRETCHDIBITS(const TEmfStretchDIBITS& oTEmfStretchDIBITS, CDataStream &oDataStream) override {};
		void HANDLE_EMR_BITBLT(const TEmfBitBlt& oTEmfBitBlt, CDataStream &oDataStream) override {};
		void HANDLE_EMR_SETDIBITSTODEVICE(const TEmfSetDiBitsToDevice& oTEmfSetDiBitsToDevice, CDataStream &oDataStream) override {};
		void HANDLE_EMR_STRETCHBLT(const TEmfStretchBLT& oTEmfStretchBLT, CDataStream &oDataStream) override {};
		void HANDLE_EMR_EOF() override {};
		void HANDLE_EMR_SAVEDC() override {};
		void HANDLE_EMR_RESTOREDC(const int &nIndexDC) override {};
		void HANDLE_EMR_MODIFYWORLDTRANSFORM(const TEmfXForm& oXForm, const unsigned int& unMode) override {};
		void HANDLE_EMR_SETWORLDTRANSFORM(const TEmfXForm& oXForm) override {};
		void HANDLE_EMR_CREATEBRUSHINDIRECT(const unsigned int& unBrushIndex, const CEmfLogBrushEx* pBrush) override {};
		void HANDLE_EMR_SETTEXTCOLOR(const TEmfColor& oColor) override {};
		void HANDLE_EMR_SELECTOBJECT(const unsigned int& unObjectIndex) override {};
		void HANDLE_EMR_EXTCREATEFONTINDIRECTW(const unsigned int& unIndex, CEmfLogFont* oLogFont) override {};
		void HANDLE_EMR_SETTEXTALIGN(const unsigned int &unAlign) override {};
		void HANDLE_EMR_SETBKMODE(const unsigned int &unBgMode) override {};
		void HANDLE_EMR_DELETEOBJECT(const unsigned int &unObjectIndex) override {};
		void HANDLE_EMR_SETMITERLIMIT(const unsigned int &unMeterLimit) override {};
		void HANDLE_EMR_EXTCREATEPEN(const unsigned int& unPenIndex, CEmfLogPen* pPen, const std::vector<unsigned int>& arUnused) override {};
		void HANDLE_EMR_CREATEPEN(const unsigned int& unPenIndex, const unsigned int& unWidthX, const CEmfLogPen* pPen) override {};
		void HANDLE_EMR_SETPOLYFILLMODE(const unsigned int& unFillMode) override {};
		void HANDLE_EMR_BEGINPATH() override {};
		void HANDLE_EMR_ENDPATH() override {};
		void HANDLE_EMR_CLOSEFIGURE() override {};
		void HANDLE_EMR_FLATTENPATH() override {};
		void HANDLE_EMR_WIDENPATH() override {};
		void HANDLE_EMR_ABORTPATH() override {};
		void HANDLE_EMR_MOVETOEX(const TEmfPointL& oPoint) override {};
		void HANDLE_EMR_SETARCDIRECTION(const unsigned int& unDirection) override {};
		void HANDLE_EMR_FILLPATH(const TEmfRectL& oBounds) override {};
		void HANDLE_EMR_SETMAPMODE(const unsigned int& unMapMode) override {};
		void HANDLE_EMR_SETWINDOWORGEX(const TEmfPointL& oOrigin) override {};
		void HANDLE_EMR_SETWINDOWEXTEX(const TEmfSizeL& oExtent) override {};
		void HANDLE_EMR_SCALEWINDOWEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom) override {};
		void HANDLE_EMR_SETVIEWPORTORGEX(const TEmfPointL& oOrigin) override {};
		void HANDLE_EMR_SETVIEWPORTEXTEX(const TEmfSizeL& oExtent) override {};
		void HANDLE_EMR_SCALEVIEWPORTEXTEX(int nXNum, int nXDenom, int nYNum, int nYDenom) override {};
		void HANDLE_EMR_SETSTRETCHBLTMODE(const unsigned int& unStretchMode) override {};
		void HANDLE_EMR_SETICMMODE(const unsigned int& unICMMode) override {};
		void HANDLE_EMR_CREATEMONOBRUSH(const unsigned int& unBrushIndex, const TEmfDibPatternBrush& oDibBrush, CDataStream &oDataStream) override {};
		void HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(const unsigned int& unBrushIndex, const TEmfDibPatternBrush& oDibBrush, CDataStream &oDataStream) override {};
		void HANDLE_EMR_SELECTCLIPPATH(const unsigned int& unRegionMode) override {};
		void HANDLE_EMR_SETBKCOLOR(const TEmfColor& oColor) override {};
		void HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL& oClip) override {};
		void HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int& unRgnDataSize, const unsigned int& unRegionMode, CDataStream &oDataStream) override {};
		void HANDLE_EMR_SETMETARGN() override {};
		void HANDLE_EMR_SETROP2(const unsigned int& unRop2Mode) override {};
		void HANDLE_EMR_CREATEPALETTE(const unsigned int& unPaletteIndex, const CEmfLogPalette* oEmfLogPalette) override {};
		void HANDLE_EMR_SELECTPALETTE(const unsigned int& unPaletteIndex) override {};
		void HANDLE_EMR_REALIZEPALETTE() override {};
		void HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL& oClip) override {};
		void HANDLE_EMR_SETLAYOUT(const unsigned int& unLayoutMode) override {};
		void HANDLE_EMR_SETBRUSHORGEX(const TEmfPointL& oOrigin) override {};
		void HANDLE_EMR_ANGLEARC(const TEmfPointL& oCenter, const unsigned int& unRadius, const double& dStartAngle, const double& dSweepAngle) override {};
		void HANDLE_EMR_ARC(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd) override {};
		void HANDLE_EMR_ARCTO(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd) override {};
		void HANDLE_EMR_CHORD(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd) override {};
		void HANDLE_EMR_ELLIPSE(const TEmfRectL& oBox) override {};
		void HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA& oTEmfExtTextoutA) override {};
		void HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW& oTEmfExtTextoutW) override {};
		void HANDLE_EMR_LINETO(const TEmfPointL& oPoint) override {};
		void HANDLE_EMR_PIE(const TEmfRectL& oBox, const TEmfPointL& oStart, const TEmfPointL& oEnd) override {};
		void HANDLE_EMR_POLYBEZIER(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override {};
		void HANDLE_EMR_POLYBEZIER(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override {};
		void HANDLE_EMR_POLYBEZIERTO(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override {};
		void HANDLE_EMR_POLYBEZIERTO(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override {};
		void HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes) override {};
		void HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes) override {};
		void HANDLE_EMR_POLYGON(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override {};
		void HANDLE_EMR_POLYGON(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override {};
		void HANDLE_EMR_POLYLINE(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override {};
		void HANDLE_EMR_POLYLINE(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override {};
		void HANDLE_EMR_POLYLINETO(const TEmfRectL& oBounds, const std::vector<TEmfPointL>& arPoints) override {};
		void HANDLE_EMR_POLYLINETO(const TEmfRectL& oBounds, const std::vector<TEmfPointS>& arPoints) override {};
		void HANDLE_EMR_POLYPOLYGON(const TEmfRectL& oBounds, const std::vector<std::vector<TEmfPointL>>& arPoints) override {};
		void HANDLE_EMR_POLYPOLYGON(const TEmfRectL& oBounds, const std::vector<std::vector<TEmfPointS>>& arPoints) override {};
		void HANDLE_EMR_POLYPOLYLINE(const TEmfRectL& oBounds, const std::vector<std::vector<TEmfPointL>>& arPoints) override {};
		void HANDLE_EMR_POLYPOLYLINE(const TEmfRectL& oBounds, const std::vector<std::vector<TEmfPointS>>& arPoints) override {};
		//TODO: Реализовать сохранение полигонов в полигоне
		void HANDLE_EMR_RECTANGLE(const TEmfRectL& oBox) override {};
		void HANDLE_EMR_ROUNDRECT(const TEmfRectL& oBox, const TEmfSizeL& oCorner) override {};
		void HANDLE_EMR_SETPIXELV(const TEmfPointL& oPoint, const TEmfColor& oColor) override {};
		void HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout& oText) override {};
		void HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL& oBounds) override {};
		void HANDLE_EMR_STROKEPATH(const TEmfRectL& oBounds) override {};

		void HANDLE_EMR_UNKNOWN(CDataStream &oDataStream) override {};
		void HANDLE_EMR_FILLRGN(const TEmfRectL& oBounds, unsigned int unIhBrush, const TRegionDataHeader& oRegionDataHeader, const std::vector<TEmfRectL>& arRects) override {};
		void HANDLE_EMR_PAINTRGN(const TEmfRectL& oBounds, const TRegionDataHeader& oRegionDataHeader, const std::vector<TEmfRectL>& arRects) override {};
		void HANDLE_EMR_FRAMERGN(const TEmfRectL& oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader& oRegionDataHeader, const std::vector<TEmfRectL>& arRects) override {};

		// Emf+
		// 2.3.1 Clipping Record Types
		void HANDLE_EMFPLUS_OFFSETCLIP(double, double) override {};
		void HANDLE_EMFPLUS_RESETCLIP() override {};
		void HANDLE_EMFPLUS_SETCLIPPATH(short, const CEmfPlusPath*) override {};
		void HANDLE_EMFPLUS_SETCLIPRECT(short, const TEmfPlusRectF&) override {};
		void HANDLE_EMFPLUS_SETCLIPREGION(short, short, const CEmfPlusRegion*) override {};

		// 2.3.2 Comment Record Types
		void HANDLE_EMFPLUS_COMMENT(CDataStream &, unsigned int) override {};

		// 2.3.3 Control Record Types
		void HANDLE_EMFPLUS_ENDOFFILE() override {};
		void HANDLE_EMFPLUS_GETDC() override {};
		void HANDLE_EMFPLUS_HEADER(unsigned int, unsigned int, unsigned int) override {};

		// 2.3.4 Drawing Record Types
		void HANDLE_EMFPLUS_CLEAR(const TEmfPlusARGB&) override {};
		void HANDLE_EMFPLUS_DRAWARC(BYTE, double, double, const TEmfPlusRectF&) override {};
		void HANDLE_EMFPLUS_DRAWBEZIERS(short, const std::vector<TEmfPlusPointF>&) override {};
		void HANDLE_EMFPLUS_DRAWCLOSEDCURVE(short, double, const std::vector<TEmfPlusPointF>&) override {};
		void HANDLE_EMFPLUS_DRAWCURVE(short, double, unsigned int, unsigned int,const std::vector<TEmfPlusPointF>&) override {};
		void HANDLE_EMFPLUS_DRAWDRIVERSTRING(short, unsigned int, unsigned int, unsigned int, TEmfPlusXForm*, const std::wstring&, const std::vector<TPointD>&) override {};
		void HANDLE_EMFPLUS_DRAWELLIPSE(short, const TEmfPlusRectF&) override {};
		void HANDLE_EMFPLUS_DRAWIMAGE(short, unsigned int, const TEmfPlusRectF&, const TEmfPlusRectF&) override {};
		void HANDLE_EMFPLUS_DRAWIMAGEPOINTS(short, unsigned int, const TEmfPlusRectF&, const TEmfPlusRectF&) override {};
		void HANDLE_EMFPLUS_DRAWLINES(short, const std::vector<TEmfPlusPointF>&) override {};
		void HANDLE_EMFPLUS_DRAWPATH(short, unsigned int, const CEmfPath*) override {};
		void HANDLE_EMFPLUS_DRAWPIE(short, double, double, const TEmfPlusRectF&) override {};
		void HANDLE_EMFPLUS_DRAWRECTS(short, const std::vector<TEmfPlusRectF>&) override {};
		void HANDLE_EMFPLUS_DRAWSTRING(short, unsigned int, unsigned int, const std::wstring&, const TEmfPlusRectF&) override {};
		void HANDLE_EMFPLUS_FILLCLOSEDCURVE(unsigned int, double, const std::vector<TEmfPlusRectF>&) override {};
		void HANDLE_EMFPLUS_FILLELLIPSE(unsigned int, const TEmfPlusRectF&) override {};
		void HANDLE_EMFPLUS_FILLPATH(short, unsigned int, const  CEmfPlusPath*) override {};
		void HANDLE_EMFPLUS_FILLPIE(unsigned int, double, double, const TEmfPlusRectF&) override {};
		void HANDLE_EMFPLUS_FILLPOLYGON(unsigned int, const std::vector<TEmfPlusPointF>&) override {};
		void HANDLE_EMFPLUS_FILLRECTS(unsigned int, const std::vector<TEmfPlusRectF>&) override {};
		void HANDLE_EMFPLUS_FILLREGION(short, unsigned int) override {};

		// 2.3.5 Object Record Types
		void HANDLE_EMFPLUS_OBJECT(const CEmfPlusObject*, unsigned int) override {};
		void HANDLE_EMFPLUS_SERIALIZABLEOBJECT(const TGUID&, unsigned int) override {};

		// 2.3.6 Property Record Types
		void HANDLE_EMFPLUS_SETANTIALIASMODE(short) override {};
		void HANDLE_EMFPLUS_SETCOMPOSITINGMODE(short) override {};
		void HANDLE_EMFPLUS_SETCOMPOSITINGQUALITY(short) override {};
		void HANDLE_EMFPLUS_SETINTERPOLATIONMODE(short) override {};
		void HANDLE_EMFPLUS_SETPIXELOFFSETMODE(short) override {};
		void HANDLE_EMFPLUS_SETRENDERINGORIGIN(int, int) override {};
		void HANDLE_EMFPLUS_SETTEXTCONTRAST(short) override {};
		void HANDLE_EMFPLUS_SETTEXTRENDERINGHINT(short) override {};

		// 2.3.7 State Record Types
		void HANDLE_EMFPLUS_BEGINCONTAINER(short, const TEmfPlusRectF&, const TEmfPlusRectF&, unsigned int) override {};
		void HANDLE_EMFPLUS_BEGINCONTAINERNOPARAMS(unsigned int) override {};
		void HANDLE_EMFPLUS_ENDCONTAINER(unsigned int) override {};
		void HANDLE_EMFPLUS_RESTORE(unsigned int) override {};
		void HANDLE_EMFPLUS_SAVE(unsigned int) override {};

		// 2.3.8 Terminal Server Record Types
		void HANDLE_EMFPLUS_SETTSCLIP(short, const std::vector<TEmfPlusRectF>&) override {};
		void HANDLE_EMFPLUS_SETTSGRAPHICS(unsigned char, unsigned char, unsigned char, unsigned char, short, short, unsigned short, unsigned char, unsigned char, const TEmfPlusXForm&) override {};

		// 2.3.9 Transform Record Types
		void HANDLE_EMFPLUS_MULTIPLYWORLDTRANSFORM(short, const TEmfPlusXForm&) override {};
		void HANDLE_EMFPLUS_RESETWORLDTRANSFORM() override {};
		void HANDLE_EMFPLUS_ROTATEWORLDTRANSFORM(short, double) override {};
		void HANDLE_EMFPLUS_SCALEWORLDTRANSFORM(short, double, double) override {};
		void HANDLE_EMFPLUS_SETPAGETRANSFORM(short, double) override {};
		void HANDLE_EMFPLUS_SETWORLDTRANSFORM(const TEmfPlusXForm&) override {};
		void HANDLE_EMFPLUS_TRANSLATEWORLDTRANSFORM(short, double, double) override {};
	};
}

#endif // CEMFINTERPRETATORRENDER_H
