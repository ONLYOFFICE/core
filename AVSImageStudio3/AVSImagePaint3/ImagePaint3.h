#pragma once
			
#include "resource.h"
#include <registration.h>
#include <PaintStruct.h>
#include <ImageColors.h>
#include <ImageCore.h>
#include <memoryutils.h>
#include <ChromoKey.h>
#include <TransformConstants.h>
	
[object, uuid("316B3ED0-EA39-43b4-8B95-A068F0FF3DCB"), dual, pointer_default(unique)]
__interface IImagePaint3 : IDispatch
{
	[id(101)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(102)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(201)] HRESULT Start([in, out, satype("BYTE")] SAFEARRAY** Image);
	[id(202)] HRESULT StartMedia([in] IUnknown* pInterfaceInput, [out] IUnknown** pInterfaceOutput);
	[id(203)] HRESULT Stop();

	[id(301)] HRESULT DrawLine([in] double X1, [in] double Y1, [in] double X2, [in] double Y2);
	[id(302)] HRESULT DrawRectangle([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] VARIANT_BOOL Solid);
	[id(303)] HRESULT DrawEllipse([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] VARIANT_BOOL Solid);
	[id(304)] HRESULT DrawInvRectangle([in] double Left, [in] double Top, [in] double Right, [in] double Bottom);
	[id(305)] HRESULT DrawInvEllipse([in] double Left, [in] double Top, [in] double Right, [in] double Bottom);
	[id(306)] HRESULT DrawPolyline([in, satype("double")] SAFEARRAY** Array); // 1d array (xy)-(xy)-...
	[id(307)] HRESULT DrawPolygon([in, satype("double")] SAFEARRAY** Array, [in] VARIANT_BOOL Solid);  // 1d array
	[id(308)] HRESULT DrawPie([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double StartAngle, [in] double SweepAngle, [in] VARIANT_BOOL Solid);
	[id(309)] HRESULT DrawSector([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double StartAngle, [in] double SweepAngle, [in] double Ratio, [in] VARIANT_BOOL Solid);
	[id(310)] HRESULT DrawSimpleSector([in] double CenterX, [in] double CenterY, [in] double Radius1, [in] double Radius2, [in] double StartAngle, [in] double SweepAngle, [in] VARIANT_BOOL Solid);
	[id(311)] HRESULT DrawSimplePie([in] double CenterX, [in] double CenterY, [in] double Radius1, [in] double StartAngle, [in] double SweepAngle, [in] VARIANT_BOOL Solid);

	[id(401)] HRESULT DrawImage([in] double Left, [in] double Top, [in, satype("BYTE")] SAFEARRAY** Image);
	[id(402)] HRESULT DrawMediaImage([in] double Left, [in] double Top, [in] IUnknown** Image);
	[id(403)] HRESULT DrawImageScale([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in, satype("BYTE")] SAFEARRAY** Image);
	[id(404)] HRESULT DrawMediaImageScale([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] IUnknown** Image);
	[id(405)] HRESULT DrawImageColorKeyRange([in] long Left, [in] long Top, [in, satype("BYTE")] SAFEARRAY** Image, [in] long Color1, [in] long Color2);
	[id(406)] HRESULT DrawMediaImageColorKeyRange([in] long Left, [in] long Top, [in] IUnknown** Image, [in] long Color1, [in] long Color2);
	[id(407)] HRESULT DrawImageColorKeyThreshold([in] long Left, [in] long Top, [in, satype("BYTE")] SAFEARRAY** Image, [in] long Color, [in] long Threshold);
	[id(408)] HRESULT DrawMediaImageColorKeyThreshold([in] long Left, [in] long Top, [in] IUnknown** Image, [in] long Color, [in] long Threshold);
	[id(409)] HRESULT DrawBorder([in] long Size, [in] long Type);
	[id(410)] HRESULT DrawMovObject([in] double Frame, [in] long Type, [in] long Divisions, [in] long Color, [in] long Alpha);
	[id(411)] HRESULT DrawCredits([in] double Frame, [in] BSTR Text, [in] long Type);

	[id(501)] HRESULT DrawText([in] double Left, [in] double Top, [in] BSTR Text);
	[id(502)] HRESULT DrawTextScale([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] BSTR Text);
	[id(503)] HRESULT DrawTextInRect([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] BSTR Text);

	[id(1001), propget] HRESULT settingsSmoothText([out, retval] VARIANT_BOOL* val);
	[id(1001), propput] HRESULT settingsSmoothText([in] VARIANT_BOOL val);
	[id(1002), propget] HRESULT settingsSmoothLines([out, retval] VARIANT_BOOL* val);
	[id(1002), propput] HRESULT settingsSmoothLines([in] VARIANT_BOOL val);
	[id(1003), propget] HRESULT settingsAlignmentVertical([out, retval] long* pVal);
	[id(1003), propput] HRESULT settingsAlignmentVertical([in] long newVal);
	[id(1004), propget] HRESULT settingsAlignmentHorizontal([out, retval] long* pVal);
	[id(1004), propput] HRESULT settingsAlignmentHorizontal([in] long newVal);
	[id(1005), propget] HRESULT settingsAlphMode([out, retval] long* pVal);
	[id(1005), propput] HRESULT settingsAlphMode([in] long newVal);
	
	[id(1101), propget] HRESULT penColor([out, retval] OLE_COLOR* val);
	[id(1101), propput] HRESULT penColor([in] OLE_COLOR val);
	[id(1102), propget] HRESULT penAlpha([out, retval] long* val);
	[id(1102), propput] HRESULT penAlpha([in] long val);
	[id(1103), propget] HRESULT penSize([out, retval] double* val);
	[id(1103), propput] HRESULT penSize([in] double val);

	[id(1201), propget] HRESULT brushType([out, retval] long* val);
	[id(1201), propput] HRESULT brushType([in] long val);
	[id(1221), propget] HRESULT brushColor1([out, retval] OLE_COLOR* val);
	[id(1221), propput] HRESULT brushColor1([in] OLE_COLOR val);
	[id(1222), propget] HRESULT brushAlpha1([out, retval] long* val);
	[id(1222), propput] HRESULT brushAlpha1([in] long val);
	[id(1223), propget] HRESULT brushColor2([out, retval] OLE_COLOR* val);
	[id(1223), propput] HRESULT brushColor2([in] OLE_COLOR val);
	[id(1224), propget] HRESULT brushAlpha2([out, retval] long* val);
	[id(1224), propput] HRESULT brushAlpha2([in] long val);
	[id(1241), propget] HRESULT brushTexturePath([out, retval] BSTR* val);
	[id(1241), propput] HRESULT brushTexturePath([in] BSTR val);
	[id(1242), propget] HRESULT brushTextureAlpha([out, retval] long* val);
	[id(1242), propput] HRESULT brushTextureAlpha([in] long val);
	[id(1243), propget] HRESULT brushTextureMode([out, retval] long* val);
	[id(1243), propput] HRESULT brushTextureMode([in] long val);
	[id(1244), propput] HRESULT brushTexture([in] BSTR Name, [in, satype("BYTE")] SAFEARRAY** Image);
	
	[id(1301), propget] HRESULT fontName([out, retval] BSTR* val);
	[id(1301), propput] HRESULT fontName([in] BSTR val);
	[id(1302), propget] HRESULT fontSize([out, retval] double* val);
	[id(1302), propput] HRESULT fontSize([in] double val);
	[id(1303), propget] HRESULT fontBold([out, retval] VARIANT_BOOL* val);
	[id(1303), propput] HRESULT fontBold([in] VARIANT_BOOL val);
	[id(1304), propget] HRESULT fontItalic([out, retval] VARIANT_BOOL* val);
	[id(1304), propput] HRESULT fontItalic([in] VARIANT_BOOL val);
	[id(1305), propget] HRESULT fontUnderline([out, retval] VARIANT_BOOL* val);
	[id(1305), propput] HRESULT fontUnderline([in] VARIANT_BOOL val);
	[id(1306), propget] HRESULT fontStrikeout([out, retval] VARIANT_BOOL* val);
	[id(1306), propput] HRESULT fontStrikeout([in] VARIANT_BOOL val);
	
	[id(2101)] HRESULT SetFormat([in, satype("BYTE")] SAFEARRAY** Array);
	[id(2102)] HRESULT GetFormat([out, retval, satype("BYTE")] SAFEARRAY** Array);
	[id(2103)] HRESULT MeasureString([in] BSTR textString, [out] DOUBLE* height, [out] DOUBLE* width);
};
	
[coclass, uuid("643120F8-43B6-47d9-9E5E-AE197F8103F4"), threading(apartment), vi_progid("AVSImagePaint3.ImagePaint3"), progid("AVSImagePaint3.ImagePaint3.1"), version(1.0)]
class ATL_NO_VTABLE ImagePaint3 : public IImagePaint3
{
protected:
	
	// general
	CGdiPlusInit initGdiPlus;
	MemoryUtils::CMemoryUtils m_oMemoryUtils;
	ImageStudio::Core::MediaData m_oMediaData;

	// additional params
	int m_nAdvancedChromaThreshold1;
	int m_nAdvancedChromaThreshold2;

	// image
	int m_width;
	int m_height;
	BYTE* m_pixels;
	BYTE* m_pBackup;
	Bitmap* m_bitmap;
	Graphics* m_graphics;
	int m_nSize;
	
	// painting formats
	Painter::CPen m_pen;
	Painter::CBrush m_brush;
	Painter::CFont m_font;
	Painter::CFormat m_format;
	Painter::CTextureManager m_textures;

	// composition
	int m_nSizeX;
	int m_nSizeY;

	int m_nDrawImageScaleType;
	int m_nDrawImageScaleBackColor;
		
protected:
	
	ImagePaint3()
	{
		m_width = 0;
		m_height = 0;
		m_nSize = 0;
		m_pixels = 0;
		m_pBackup = 0;
		m_bitmap = 0;
		m_graphics = 0;
		initGdiPlus.Init();

		m_nSizeX = 0;
		m_nSizeY = 0;

		m_nAdvancedChromaThreshold1 = 16;
		m_nAdvancedChromaThreshold2 = 16;

		m_nDrawImageScaleType = c_nResizeStretchNearest;
		m_nDrawImageScaleBackColor = 0;
	}
	~ImagePaint3()
	{
		Clear();
	}
	
	BOOL Valid()
	{
		if (!m_graphics)
			return FALSE;

		return TRUE;
	}
	void Clear()
	{
		m_pen.Clear();
		m_brush.Clear();
		m_font.Clear();
		m_textures.Clear();
		m_format.Clear();

		ClearImage();
		ClearBackup();

		m_nSizeX = 0;
		m_nSizeY = 0;
	}
	void ClearImage()
	{
		if (m_graphics)
			delete m_graphics;

		m_graphics = 0;

		if (m_bitmap)
			delete m_bitmap;

		m_bitmap = 0;

		m_width = 0;
		m_height = 0;
		m_nSize = 0;
		m_pixels = 0;
	}
	void Start()
	{
		if (!CRegistratorClient::IsRegistered())
			return;

		// variables
		BITMAPINFO info;

		// prepare header for the image
		info.bmiHeader.biBitCount = 32;
		info.bmiHeader.biClrImportant = 0;
		info.bmiHeader.biClrUsed = 0;
		info.bmiHeader.biCompression = 0;
		info.bmiHeader.biHeight = m_height;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biSizeImage = 0;
		info.bmiHeader.biWidth = m_width;

		// create new bitmap
		m_bitmap = new Bitmap(&info, m_pixels);
		m_nSize = 4*m_width*m_height;

		// check for valid bitmap
		if (!m_bitmap)
			ClearImage();
		else
		{
			// create graphics for painting
			m_graphics = new Graphics(m_bitmap);

			// init common formatting
			if (m_format.AntiAliasPen)
				m_graphics->SetSmoothingMode(SmoothingModeAntiAlias);
			else
				m_graphics->SetSmoothingMode(SmoothingModeNone);
			if (m_format.AntiAliasText)
				m_graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
			else
				m_graphics->SetTextRenderingHint(TextRenderingHintSystemDefault);
		}
	}
	BYTE* SetAlphaChanal(BYTE* pixels, int width, int height)
	{
		BYTE *pAlphaChanal = NULL;
		BYTE *pAlphaChanalTemp = NULL;

		// clear alpha channel with specified value
		BYTE* alpha_channel = (BYTE*)(pixels + 3);
		if (Painter::c_AlphModeReplaceWithBrushAlpha == m_format.ImageAlphMode)
		{
			pAlphaChanal = new BYTE[width*height];
			pAlphaChanalTemp = pAlphaChanal;
			
			for (int index = 0; index < width*height; ++index, alpha_channel += 4)
			{
				*pAlphaChanalTemp++ = *alpha_channel;
				*alpha_channel = m_brush.Alpha1;
			}
		}
		// clear alpha channel with combine value
		else if (Painter::c_AlphModeCombine == m_format.ImageAlphMode)
		{
			pAlphaChanal = new BYTE[width*height];
			pAlphaChanalTemp = pAlphaChanal;
			
			for (int index = 0; index < width*height; ++index, alpha_channel += 4)
			{
				*pAlphaChanalTemp++ = *alpha_channel;
				*alpha_channel *= (REAL)(m_brush.Alpha1)/255.F;
			}
		}
		return pAlphaChanal;
	}
	void RestoreAlphaChanal(BYTE* pixels, int width, int height, BYTE* pAlphaChanal)
	{
		// restore alpha channel with old value
		if (Painter::c_AlphModeDoNothing != m_format.ImageAlphMode)
		{
			_ASSERT(pAlphaChanal);
			
			BYTE* alpha_channel = (BYTE*)(pixels + 3);
			BYTE* pAlphaChanalTemp = pAlphaChanal;

			for (int index = 0; index < width*height; ++index, alpha_channel += 4)
				*alpha_channel = *pAlphaChanalTemp++;
			
			delete pAlphaChanal;
		}
	}
	inline BOOL GetPathEndPoints(GraphicsPath& path, PointF& pointBeg, PointF& pointEnd)
	{
		int nCount = path.GetPointCount();

		if (nCount < 1)
			return FALSE;

		PointF* pPoints = new PointF[nCount];  path.GetPathPoints(pPoints, nCount);

		pointBeg = *pPoints;
		pointEnd = *(pPoints + nCount - 1);

		if (pPoints)
			delete pPoints;

		return TRUE;
	}
	
	// Helpers for image painting
	void DrawImageScale(double dLeft, double dTop, double dRight, double dBottom, int nWidth, int nHeight, BYTE* pPixels)
	{
		if (!Valid())
			return;

		if (m_brush.Alpha1 == 0 && Painter::c_AlphModeDoNothing != m_format.ImageAlphMode)
			return;

		// clear alpha channel with specified value
		BYTE* pAlphaChanal = SetAlphaChanal(pPixels, nWidth, nHeight);

		// create bitmap to draw
		Bitmap bitmap(nWidth, nHeight, nWidth*4, PixelFormat32bppARGB, pPixels);
		bitmap.RotateFlip(RotateNoneFlipY);
		
		// compute output rectangle
		REAL rLeft = (REAL)min(dLeft, dRight);
		REAL rTop = (REAL)min(dTop, dBottom);
		REAL rRight = (REAL)max(dLeft, dRight);
		REAL rBottom = (REAL)max(dTop, dBottom);

		REAL rNaturalLeft = rLeft;
		REAL rNaturalTop = rTop;
		REAL rNaturalWidth = rRight - rLeft;
		REAL rNaturalHeight = rBottom - rTop;

		double dScaleX = 1.0;
		double dScaleY = 1.0;

		double dNewWidth = abs(dRight - dLeft);
		double dNewHeight = abs(dBottom - dTop);
		double dWidth = (double)nWidth;
		double dHeight = (double)nHeight;
		
		dScaleX = dNewWidth / dWidth;
		dScaleY = dNewHeight / dHeight;

		double dZoom = min(1.0, min(dScaleX, dScaleY));

		switch (m_nDrawImageScaleType)
		{
		case c_nResizeStretchNearest:
		case c_nResizeStretchBillinear:
		case c_nResizeStretchBicubic:
			break;
		case c_nResizeShrinkNearest:
		case c_nResizeShrinkBillinear:
		case c_nResizeShrinkBicubic:
			{
				if (dScaleX > dScaleY)
				{
					// полосы слева и справа
					REAL rOffset = (REAL)((dNewWidth - (dWidth * dNewHeight / dHeight)) / 2);
					rNaturalLeft += rOffset;
					rNaturalWidth -= 2 * rOffset;
				}
				else
				{
					// полосы сверху и снизу
					REAL rOffset = (REAL)((dNewHeight - (dHeight * dNewWidth / dWidth)) / 2);
					rNaturalTop += rOffset;
					rNaturalHeight -= 2 * rOffset;
				}
				break;
			}
		case c_nResizeCropNearest:
		case c_nResizeCropBillinear:
		case c_nResizeCropBicubic:
			{
				rNaturalLeft = 0; rNaturalTop = 0;
				rNaturalWidth = (REAL)nWidth; rNaturalHeight = (REAL)nHeight;
				
				if (dScaleX > dScaleY)
				{
					// ширина - на месте
					double dNewHeightReal = dNewHeight;
					dNewHeight = ((dNewWidth * dHeight / dWidth));
					double dOffset = (dNewHeight - dNewHeightReal) / 2;
					dOffset *= (dHeight / dNewHeight);
					
					REAL rOffset = (REAL)dOffset;
					//REAL rOffset = (REAL)(((dWidth * dNewHeight / dHeight) - dWidth) / 2);
					rNaturalTop += rOffset;
					rNaturalHeight -= 2 * rOffset;
				}
				else
				{
					// высота - на месте
					double dNewWidthReal = dNewWidth;
					dNewWidth = ((dNewHeight * dWidth / dHeight));
					double dOffset = (dNewWidth - dNewWidthReal) / 2;
					dOffset *= (dWidth / dNewWidth);
					
					REAL rOffset = (REAL)dOffset;
					//REAL rOffset = (REAL)(((dHeight * dNewWidth / dWidth) - dHeight) / 2);
					rNaturalLeft += rOffset;
					rNaturalWidth -= 2 * rOffset;
				}
				Rect rect(rLeft, rTop, rRight - rLeft, rBottom - rTop);
				m_graphics->DrawImage(&bitmap, rect, rNaturalLeft, rNaturalTop, rNaturalWidth, rNaturalHeight, UnitPixel);
			
				// restore alpha chanal for source image
				RestoreAlphaChanal(pPixels, nWidth, nHeight, pAlphaChanal);
				return;
			}
		};

		Rect rect(rLeft, rTop, rRight - rLeft, rBottom - rTop);
		if ((rLeft < rNaturalLeft) || (rTop < rNaturalTop) && (-1 != m_nDrawImageScaleBackColor))
		{
			Color oBackColor((BYTE)(m_nDrawImageScaleBackColor), (BYTE)(m_nDrawImageScaleBackColor >> 8),
							(BYTE)(m_nDrawImageScaleBackColor >> 16));
			
			SolidBrush* pBrush = new SolidBrush(oBackColor);
			m_graphics->FillRectangle(pBrush, rect);
			RELEASEOBJECT(pBrush);
		}

		rect.X = rNaturalLeft;
		rect.Y = rNaturalTop;
		rect.Width = rNaturalWidth;
		rect.Height = rNaturalHeight;

		// draw one image inside another
		m_graphics->DrawImage(&bitmap, rect, 0, 0, nWidth, nHeight, UnitPixel);

		// restore alpha chanal for source image
		RestoreAlphaChanal(pPixels, nWidth, nHeight, pAlphaChanal);
	}
	void DrawImageColorKeyRange(long dLeft, long dTop, int nWidth, int nHeight, BYTE* pPixels, long nColor1, long nColor2)
	{
		float fH, fL, fS;
		
		ImageStudio::Colors::Color oColor1;
		oColor1.FromCOLORREF(nColor1); oColor1.ToHLS(fH, fL, fS);
		int nHComponent1 = (int)(fH*255.0f/360.0f);

		ImageStudio::Colors::Color oColor2;
		oColor2.FromCOLORREF(nColor2); oColor2.ToHLS(fH, fL, fS);
		int nHComponent2 = (int)(fH*255.0f/360.0f);

		CChromoKey oChromo;
		oChromo.SetImage(pPixels, nWidth, nHeight);
		oChromo.DoChromoKey(m_brush.Alpha1/255., nHComponent1, m_nAdvancedChromaThreshold1, nHComponent2, m_nAdvancedChromaThreshold2);
		oChromo.DrawToGraphics(m_graphics, TRUE, dLeft, dTop);
	}
	void DrawImageColorKeyThreshold(long dLeft, long dTop, int nWidth, int nHeight, BYTE* pPixels, long nColor, long nThreshold)
	{
		float fH, fL, fS;
		
		ImageStudio::Colors::Color oColor1;
		oColor1.FromCOLORREF(nColor); oColor1.ToHLS(fH, fL, fS);
		int nHComponent = (int)(fH*255.0f/360.0f);

		if (nThreshold > 100)
			nThreshold = 100;
		if (nThreshold < 0)
			nThreshold = 0;

		int nOldMode = m_graphics->SetSmoothingMode(SmoothingModeAntiAlias);

		CChromoKey oChromo;
		oChromo.SetImage(pPixels, nWidth, nHeight);
		oChromo.DoChromoKey(m_brush.Alpha1/255., nHComponent, nThreshold);
		oChromo.DrawToGraphics(m_graphics, TRUE, dLeft, dTop);

		m_graphics->SetSmoothingMode((SmoothingMode)nOldMode);
	}	
	
	// Helpers inline functions
	inline BYTE Byte(double value)
	{
		return (value < 0) ? 0 : (value > 255) ? 255 : (BYTE)value; 
	}
	inline void CreateBackup()
	{
		ClearBackup();

		m_pBackup = new BYTE[m_nSize];
		
		m_oMemoryUtils.memcpy(m_pBackup, m_pixels, m_nSize);
	}
	inline void ClearBackup()
	{
		if (m_pBackup)
			delete[] m_pBackup; 
		
		m_pBackup = 0; 
	}
	inline double Clamp(double v)
	{
		return (v < 0) ? 0 : (v > 1) ? 1 : v; 
	}
	
	// Helpers for draw border 
	BYTE GetPointBillinear(double dPointX, double dPointY, BYTE* pData, int nWidth, int nHeight, int nChannel, int nChannels, BYTE nColor)
	{
		int nX = (int)floor(dPointX);
		int nY = (int)floor(dPointY);

		if (nX == nWidth - 1)	{ dPointX -= 1; nX -= 1; }
		if (nY == nHeight - 1)	{ dPointY -= 1; nY -= 1; }

		if (nX < 0 || nX + 1 >= nWidth || nY < 0 || nY + 1 >= nHeight)
			return nColor;

		double dX = dPointX - nX;
		double dY = dPointY - nY;

		int nIndex = nChannel + nChannels*(nX + nY*nWidth);

		BYTE byLT = pData[nIndex];
		BYTE byRT = pData[nIndex + nChannels];
		BYTE byRB = pData[nIndex + nChannels + nChannels*nWidth];
		BYTE byLB = pData[nIndex + nChannels*nWidth];

		return (BYTE)
		(
			(1 - dX)*(1 - dY)*byLT + // Left - Top corner
			(    dX)*(1 - dY)*byRT + // Right - Top corner
			(    dX)*(    dY)*byRB + // Right - Bottom corner
			(1 - dX)*(    dY)*byLB   // Left - Bottom corner
		);
	}
		
	// Helpers for draw mov object
	BOOL ApplyColor(BYTE* pPixels, int nSize, int nColor, int nAlpha);
	BOOL ApplyCompose(double dFrame, long nType, int nColor, int nAlpha);
	BOOL WipeDoor(double Completeness, int Type, int Color, int Alpha);
	BOOL WipeSide(double Completeness, int Type, int Color, int Alpha);
	BOOL WipeCenter(double Completeness, int Type, int Color, int Alpha);
	BOOL WipeStrips(double Completeness, int Type, int Color, int Alpha);
	BOOL PushDoor(double Completeness, int Type, int Color, int Alpha);
	BOOL PushSide(double Completeness, int Type, int Color, int Alpha);
	BOOL PushStrips(double Completeness, int Type, int Color, int Alpha);
	BOOL FxMosaic(double Completeness, int Type, int Color, int Alpha);
	
public:
	
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CComVariant ParamVal = ParamValue;
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"ChromaThreshold1")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			m_nAdvancedChromaThreshold1 = ParamVal.lVal;
		}
		else if (ParamStr == L"ChromaThreshold2")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			m_nAdvancedChromaThreshold2 = ParamVal.lVal;
		}
		else if (ParamStr == L"SetPenXml")
		{
			ParamVal.ChangeType(VT_BSTR, NULL); 

			CString strXml; strXml = ParamVal.bstrVal;

			m_pen.FromXmlString(strXml);
		}
		else if (ParamStr == L"SetBrushXml")
		{
			ParamVal.ChangeType(VT_BSTR, NULL); 

			CString strXml; strXml = ParamVal.bstrVal;

			m_brush.FromXmlString(strXml);
		}
		else if (ParamStr == L"SetFontXml")
		{
			ParamVal.ChangeType(VT_BSTR, NULL); 

			CString strXml; strXml = ParamVal.bstrVal;

			m_font.FromXmlString(strXml);
		}
		else if (ParamStr == L"SetFormatXml")
		{
			ParamVal.ChangeType(VT_BSTR, NULL); 

			CString strXml; strXml = ParamVal.bstrVal;

			m_format.FromXmlString(strXml);
		}
		else if (ParamStr == L"DrawImageScaleType")
		{
			ParamVal.ChangeType(VT_INT, NULL); 
			m_nDrawImageScaleType = ParamVal.intVal;			
		}
		else if (ParamStr == L"DrawImageScaleBackColor")
		{
			ParamVal.ChangeType(VT_INT, NULL); 
			m_nDrawImageScaleBackColor = ParamVal.lVal;			
		}

		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"ChromaThreshold1")
			(*ParamValue).lVal = (LONG)m_nAdvancedChromaThreshold1;
		else if (ParamStr == L"ChromaThreshold2")
			(*ParamValue).lVal = (LONG)m_nAdvancedChromaThreshold2;

		else if (ParamStr == L"DrawImageScaleType")
		{
			(*ParamValue).vt = VT_INT;
			(*ParamValue).intVal = m_nDrawImageScaleType;
		}
		else if (ParamStr == L"DrawImageScaleBackColor")
		{
			(*ParamValue).vt = VT_INT;
			(*ParamValue).intVal = m_nDrawImageScaleBackColor;
		}

		return S_OK;
	}
	
	STDMETHOD(Start)(SAFEARRAY** Image)
	{
		Stop();

		int nChannels = 0;

		m_pixels = ImageStudioUtils::ExtractImage(Image, m_width, m_height, nChannels);

		if (!m_pixels || nChannels != 4)
			return S_FALSE;

		Start();

		return S_OK;
	}
	STDMETHOD(StartMedia)(IUnknown* pInterfaceInput, IUnknown** pInterfaceOutput)
	{
		Stop();

		if (!pInterfaceInput || !pInterfaceOutput)
			return S_FALSE;

		*pInterfaceOutput = NULL;

		if (!m_oMediaData.Create(pInterfaceInput, FALSE) || !m_oMediaData.IsValidBGRA())
			return S_FALSE;

		(*pInterfaceOutput) = m_oMediaData.GetMediaData(TRUE);

		m_pixels = m_oMediaData.GetBuffer();
		m_width = m_oMediaData.GetWidth();
		m_height = m_oMediaData.GetHeight();

		Start();

		return S_OK;
	}
	STDMETHOD(Stop)()
	{
		if (m_graphics)
			m_graphics->Flush();

		ClearImage();

		m_oMediaData.Destroy();

		return S_OK;
	}
	
	STDMETHOD(DrawLine)(double X1, double Y1, double X2, double Y2)
	{
		if (!Valid())
			return S_OK;

		Pen* pen = m_pen.GetPen();

		m_graphics->DrawLine(pen, (REAL)X1, (REAL)Y1, (REAL)X2, (REAL)Y2);

		return S_OK;
	}
	STDMETHOD(DrawRectangle)(double Left, double Top, double Right, double Bottom, VARIANT_BOOL Solid)
	{
		if (!Valid())
			return S_OK;

		REAL rLeft = (REAL)min(Left, Right);
		REAL rTop = (REAL)min(Top, Bottom);
		REAL rRight = (REAL)max(Left, Right);
		REAL rBottom = (REAL)max(Top, Bottom);

		if (Solid == VARIANT_TRUE)
		{
			RectF rectGradient(rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
			Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

			m_graphics->FillRectangle(brush, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
		}
		else
		{
			Pen* pen = m_pen.GetPen();

			m_graphics->DrawRectangle(pen, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
		}

		return S_OK;
	}
	STDMETHOD(DrawEllipse)(double Left, double Top, double Right, double Bottom, VARIANT_BOOL Solid)
	{
		if (!Valid())
			return S_OK;

		REAL rLeft = (REAL)min(Left, Right);
		REAL rTop = (REAL)min(Top, Bottom);
		REAL rRight = (REAL)max(Left, Right);
		REAL rBottom = (REAL)max(Top, Bottom);

		if (Solid == VARIANT_TRUE)
		{
			RectF rectGradient(rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
			Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

			m_graphics->FillEllipse(brush, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
		}
		else
		{
			Pen* pen = m_pen.GetPen();

			m_graphics->DrawEllipse(pen, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
		}

		return S_OK;
	}

	STDMETHOD(DrawInvRectangle)(double Left, double Top, double Right, double Bottom)
	{
		if (!Valid())
			return S_OK;

		REAL rLeft = (REAL)min(Left, Right);
		REAL rTop = (REAL)min(Top, Bottom);
		REAL rRight = (REAL)max(Left, Right);
		REAL rBottom = (REAL)max(Top, Bottom);

		RectF rectGradient(0, 0, m_width, m_height);
		Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

		// compose region to fill
		Rect rect1(0, 0, m_width, m_height);
		Rect rect2(rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
		Region region(rect1); region.Xor(rect2);

		// draw region
		m_graphics->FillRegion(brush, &region);

		return S_OK;
	}
	STDMETHOD(DrawInvEllipse)(double Left, double Top, double Right, double Bottom)
	{
		if (!Valid())
			return S_OK;

		REAL rLeft = (REAL)min(Left, Right);
		REAL rTop = (REAL)min(Top, Bottom);
		REAL rRight = (REAL)max(Left, Right);
		REAL rBottom = (REAL)max(Top, Bottom);

		RectF rectGradient(0, 0, m_width, m_height);
		Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

		// compose region to fill
//		Rect rect(0, 0, m_width, m_height);		//	antialiasing problems 
		Rect rect(-1, -1, m_width + 2, m_height + 2);
		GraphicsPath path; 
		path.AddEllipse(rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
		path.AddRectangle(rect);

		// draw region
		m_graphics->FillPath(brush, &path);

		return S_OK;
	}
	STDMETHOD(DrawPolyline)(SAFEARRAY** Array)
	{
		if (!Valid() || !Array || !*Array)
			return S_OK;

		int nCount = (*Array)->rgsabound[0].cElements;

		if (nCount % 2 || nCount < 4)
			return S_OK;

		nCount /= 2;

		// variables
		PointF* pPoints = new PointF[nCount];
		double* pDoubles = (double*)((*Array)->pvData);
		
		// compose array of points to draw
		for (int nIndex = 0; nIndex < nCount; ++nIndex, pDoubles += 2)
		{
			pPoints[nIndex].X = (REAL)pDoubles[0];
			pPoints[nIndex].Y = (REAL)pDoubles[1];
		}

		Pen* pen = m_pen.GetPen();

		// draw polygon
		m_graphics->DrawLines(pen, pPoints, nCount);
		
		// delete temporary memory
		delete[] pPoints;

		return S_OK;
	}
	STDMETHOD(DrawPolygon)(SAFEARRAY** Array, VARIANT_BOOL Solid)
	{
		if (!Valid() || !Array || !*Array)
			return S_OK;

		int nCount = (*Array)->rgsabound[0].cElements;

		if (nCount % 2 || nCount < 6)
			return S_OK;

		nCount /= 2;

		// variables
		PointF* pPoints = new PointF[nCount];
		double* pDoubles = (double*)((*Array)->pvData);
		
		// draw polygon
		if (Solid == VARIANT_TRUE)
		{
			// get rect of poligon
			REAL minX = pDoubles[0];
			REAL minY = pDoubles[1];
			REAL maxX = pDoubles[0];
			REAL maxY = pDoubles[1];

			// compose array of points to draw
			for (int nIndex = 0; nIndex < nCount; ++nIndex, pDoubles += 2)
			{
				pPoints[nIndex].X = pDoubles[0];
				pPoints[nIndex].Y = pDoubles[1];
				if (pPoints[nIndex].X < minX)
					minX = pPoints[nIndex].X;
				if (pPoints[nIndex].X > maxX)
					maxX = pPoints[nIndex].X;
				if (pPoints[nIndex].Y < minY)
					minY = pPoints[nIndex].Y;
				if (pPoints[nIndex].Y > maxY)
					maxY = pPoints[nIndex].Y;
			}

			RectF rectGradient(minX, minY, maxX - minX, maxY - minY);
			Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

			m_graphics->FillPolygon(brush, pPoints, nCount);
		}
		else
		{
			// compose array of points to draw
			for (int nIndex = 0; nIndex < nCount; ++nIndex, pDoubles += 2)
			{
				pPoints[nIndex].X = pDoubles[0];
				pPoints[nIndex].Y = pDoubles[1];
			}
			Pen* pen = m_pen.GetPen();

			m_graphics->DrawPolygon(pen, pPoints, nCount);
		}

		// delete temporary memory
		delete[] pPoints;

		return S_OK;
	}
	STDMETHOD(DrawImage)(double Left, double Top, SAFEARRAY** Image)
	{
		int nWidth = 0;
		int nHeight = 0;
		int nChannels = 0;
		BYTE* pPixels = ImageStudioUtils::ExtractImage(Image, nWidth, nHeight, nChannels);

		if (!pPixels || nChannels != 4)
			return S_FALSE;

		DrawImageScale(Left, Top, Left + nWidth, Top + nHeight, nWidth, nHeight, pPixels);

		return S_OK;
	}
	STDMETHOD(DrawImageScale)(double Left, double Top, double Right, double Bottom, SAFEARRAY** Image)
	{
		int nWidth = 0;
		int nHeight = 0;
		int nChannels = 0;
		BYTE* pPixels = ImageStudioUtils::ExtractImage(Image, nWidth, nHeight, nChannels);

		if (!pPixels || nChannels != 4)
			return S_FALSE;

		DrawImageScale(Left, Top, Right, Bottom, nWidth, nHeight, pPixels);

		return S_OK;
	}
	STDMETHOD(DrawImageColorKeyRange)(long Left, long Top, SAFEARRAY** Image, long Color1, long Color2)
	{
		int nWidth = 0;
		int nHeight = 0;
		int nChannels = 0;
		BYTE* pPixels = ImageStudioUtils::ExtractImage(Image, nWidth, nHeight, nChannels);

		if (!pPixels || nChannels != 4)
			return S_FALSE;

		DrawImageColorKeyRange(Left, Top, nWidth, nHeight, pPixels, Color1, Color2);

		return S_OK;
	}
	STDMETHOD(DrawImageColorKeyThreshold)(long Left, long Top, SAFEARRAY** Image, long Color, long Threshold)
	{
		int nWidth = 0;
		int nHeight = 0;
		int nChannels = 0;
		BYTE* pPixels = ImageStudioUtils::ExtractImage(Image, nWidth, nHeight, nChannels);

		if (!pPixels || nChannels != 4)
			return S_FALSE;

		DrawImageColorKeyThreshold(Left, Top, nWidth, nHeight, pPixels, Color, Threshold);

		return S_OK;
	}
	STDMETHOD(DrawMediaImage)(double Left, double Top, IUnknown** pInterface)
	{
		if (!pInterface || !(*pInterface))
			return S_FALSE;

		ImageStudio::Core::MediaData oMediaData;

		if (!oMediaData.Create(*pInterface, FALSE) || !oMediaData.IsValidBGRA())
			return S_FALSE;

		int nWidth = oMediaData.GetWidth();
		int nHeight = oMediaData.GetHeight();
		BYTE* pPixels = oMediaData.GetBuffer();

		DrawImageScale(Left, Top, Left + nWidth, Top + nHeight, nWidth, nHeight, pPixels);

		return S_OK;
	}
	STDMETHOD(DrawMediaImageScale)(double Left, double Top, double Right, double Bottom, IUnknown** pInterface)
	{
		if (!pInterface || !(*pInterface))
			return S_FALSE;

		ImageStudio::Core::MediaData oMediaData;

		if (!oMediaData.Create(*pInterface, FALSE) || !oMediaData.IsValidBGRA())
			return S_FALSE;

		int nWidth = oMediaData.GetWidth();
		int nHeight = oMediaData.GetHeight();
		BYTE* pPixels = oMediaData.GetBuffer();

		DrawImageScale(Left, Top, Right, Bottom, nWidth, nHeight, pPixels);

		return S_OK;
	}
	STDMETHOD(DrawMediaImageColorKeyRange)(long Left, long Top, IUnknown** pInterface , long Color1, long Color2)
	{
		if (!pInterface || !(*pInterface))
			return S_FALSE;

		ImageStudio::Core::MediaData oMediaData;

		if (!oMediaData.Create(*pInterface, FALSE) || !oMediaData.IsValidBGRA())
			return S_FALSE;

		int nWidth = oMediaData.GetWidth();
		int nHeight = oMediaData.GetHeight();
		BYTE* pPixels = oMediaData.GetBuffer();

		DrawImageColorKeyRange(Left, Top, nWidth, nHeight, pPixels, Color1, Color2);

		return S_OK;
	}
	STDMETHOD(DrawMediaImageColorKeyThreshold)(long Left, long Top, IUnknown** pInterface, long Color, long Threshold)
	{
		if (!pInterface || !(*pInterface))
			return S_FALSE;

		ImageStudio::Core::MediaData oMediaData;

		if (!oMediaData.Create(*pInterface, FALSE) || !oMediaData.IsValidBGRA())
			return S_FALSE;

		int nWidth = oMediaData.GetWidth();
		int nHeight = oMediaData.GetHeight();
		BYTE* pPixels = oMediaData.GetBuffer();

		DrawImageColorKeyThreshold(Left, Top, nWidth, nHeight, pPixels, Color, Threshold);

		return S_OK;
	}
	
	STDMETHOD(DrawText)(double Left, double Top, BSTR Text)
	{
		if (!Valid())
			return S_OK;

		Font* font = m_font.GetFont();

		RectF rectGradient; m_brush.GetTextRect(Text, m_graphics, font, Left, Top, &rectGradient);
		Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

		PointF point((REAL)Left, (REAL)Top);

		StringFormat* pFormat = m_format.GetStringFormat(); 

		m_graphics->DrawString(Text, SysStringLen(Text), font, point, pFormat, brush);

		return S_OK;
	}
	STDMETHOD(DrawTextScale)(double Left, double Top, double Right, double Bottom, BSTR Text)
	{
		if (!Valid())
			return S_OK;

		Font* font = m_font.GetFont();
		PointF point((REAL)Left, (REAL)Top);
		Rect rect((REAL)Left, (REAL)Top, (REAL)(Right - Left), (REAL)(Bottom - Top));

		// retrieve current logfont
		RectF fontRect;

		// measure the string
		m_graphics->MeasureString(Text, SysStringLen(Text), font, point, &fontRect);

		// update font size
		double scaleX = rect.Width/(double)fontRect.Width;
		double scaleY = rect.Height/(double)fontRect.Height;

		// apply translation to the device context
		m_graphics->TranslateTransform(point.X, point.Y);
		m_graphics->ScaleTransform(scaleX, scaleY);

		// draw text at start
		point.X = 0;
		point.Y = 0;

		RectF rectGradient; m_brush.GetTextRect(Text, m_graphics, font, point.X, point.Y, &rectGradient);
		Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

		// draw text string
		// TODO: m_graphics->DrawString(Text, SysStringLen(Text), font, point, brush);

		StringFormat format; 
		format.SetAlignment(m_format.GetAlignment(m_format.StringAlignmentHorizontal)); 
		format.SetLineAlignment(m_format.GetAlignment(m_format.StringAlignmentVertical));

		RectF rectText((REAL)0, (REAL)0, (REAL)fontRect.Width, (REAL)fontRect.Height);
		m_graphics->DrawString(Text, SysStringLen(Text), font, rectText, &format, brush);

		// reset the transform to initial state
		m_graphics->ResetTransform();

		return S_OK;
	}

	STDMETHOD(DrawTextInRect)(double Left, double Top, double Right, double Bottom, BSTR Text)
	{
		if (!Valid())
			return S_OK;

		Font* font = m_font.GetFont();

		StringFormat format; 
		format.SetAlignment(m_format.GetAlignment(m_format.StringAlignmentHorizontal)); 
		format.SetLineAlignment(m_format.GetAlignment(m_format.StringAlignmentVertical)); 

	//	RectF rectGradient; m_brush.GetTextRect(Text, m_graphics, font, Left, Top, &rectGradient);
		RectF rectGradient((REAL)Left, (REAL)Top, (REAL)(Right - Left), (REAL)(Bottom - Top));
		Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

		PointF point((REAL)Left, (REAL)Top);

		m_graphics->DrawString(Text, SysStringLen(Text), font, rectGradient, &format, brush);

		return S_OK;
	}

	STDMETHOD(DrawPie)(double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle, VARIANT_BOOL Solid)
	{
		if (!Valid())
			return S_OK;

		REAL rLeft = (REAL)min(Left, Right);
		REAL rTop = (REAL)min(Top, Bottom);
		REAL rRight = (REAL)max(Left, Right);
		REAL rBottom = (REAL)max(Top, Bottom);
		REAL rStartAngle = (REAL)StartAngle;
		REAL rSweepAngle = (REAL)SweepAngle;

		if (Solid == VARIANT_TRUE)
		{

			RectF rectGradient(rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
			Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

			m_graphics->FillPie(brush, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1, rStartAngle, rSweepAngle);
		}
		else
		{
			Pen* pen = m_pen.GetPen();

			m_graphics->DrawPie(pen, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1, rStartAngle, rSweepAngle);
		}
		return S_OK;
	}
	STDMETHOD(DrawSector)(double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle, double Ratio, VARIANT_BOOL Solid)
	{		
		if (!Valid())
			return S_OK;

		REAL rLeft = (REAL)min(Left, Right);
		REAL rTop = (REAL)min(Top, Bottom);
		REAL rRight = (REAL)max(Left, Right);
		REAL rBottom = (REAL)max(Top, Bottom);
		REAL rStartAngle = (REAL)StartAngle;
		REAL rSweepAngle = (REAL)SweepAngle;
		REAL rRatio = Ratio;

		// get rects
		REAL rLeftInt, rTopInt, rWidthInt, rHeightInt, rLeftOut, rTopOut, rWidthOut, rHeightOut; 
		if (rRatio <= 1.0F)
		{
			rLeftOut = rLeft;
			rTopOut = rTop;
			rWidthOut = rRight - rLeft;
			rHeightOut = rBottom - rTop;
			rLeftInt = rLeft + (rWidthOut - rWidthOut*rRatio)/2;
			rTopInt = rTop + (rHeightOut - rHeightOut*rRatio)/2;
			rWidthInt = rWidthOut*rRatio;
			rHeightInt = rHeightOut*rRatio;
		}
		else
		{
			rLeftInt = rLeft;
			rTopInt = rTop;
			rWidthInt = rRight - rLeft;
			rHeightInt = rBottom - rTop;

			rLeftOut = rLeft + (rWidthInt - rWidthInt*rRatio)/2;
			rTopOut = rTopInt + (rHeightInt - rHeightInt*rRatio)/2;
			rWidthOut = rWidthInt*rRatio;
			rHeightOut = rHeightInt*rRatio;
		}

		// create Arc1
		GraphicsPath pathArc1; pathArc1.AddArc(rLeftOut, rTopOut, rWidthOut - 1, rHeightOut - 1, rStartAngle, rSweepAngle);

		// get Beg and End points for Arc1
		PointF pointBeg1, pointEnd1;
		if (!GetPathEndPoints(pathArc1, pointBeg1, pointEnd1))
			return S_OK;

		// create Arc2
		GraphicsPath pathArc2; pathArc2.AddArc(rLeftInt, rTopInt, rWidthInt - 1, rHeightInt - 1, rStartAngle, rSweepAngle);

		// get Beg and End points for Arc2
		PointF pointBeg2, pointEnd2;
		if (!GetPathEndPoints(pathArc2, pointBeg2, pointEnd2))
			return S_OK;

		// create path sector
		GraphicsPath pathSector;
		pathArc1.Reverse();
		pathSector.AddPath(&pathArc1, FALSE);
		pathSector.AddLine(pointBeg1, pointBeg2);
		pathSector.AddArc(rLeftInt, rTopInt, rWidthInt - 1, rHeightInt - 1, rStartAngle, rSweepAngle);
		pathSector.AddLine(pointEnd2, pointEnd1);

		// draw sector
		if (Solid == VARIANT_TRUE)
		{
			RectF rectGradient(rLeftOut, rTopOut, rWidthOut - 1, rHeightOut - 1);
			Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);
			m_graphics->FillPath(brush, &pathSector);
		}
		else
		{
			Pen* pen = m_pen.GetPen();
			m_graphics->DrawPath(pen, &pathSector);
		}

		return S_OK;
	}
	STDMETHOD(DrawSimpleSector)(double CenterX, double CenterY, double Radius1, double Radius2, double StartAngle, double SweepAngle, VARIANT_BOOL Solid)
	{
		if (!Valid())
			return S_OK;

		REAL R1 = (REAL)min(Radius1, Radius2);
		REAL R2 = (REAL)max(Radius1, Radius2);

		REAL rLeft = (REAL)(CenterX - R2);
		REAL rTop = (REAL)(CenterY - R2);
		REAL rRight = (REAL)(CenterX + R2);
		REAL rBottom = (REAL)(CenterY + R2);
		REAL rStartAngle = (REAL)StartAngle;
		REAL rSweepAngle = (REAL)SweepAngle;
		REAL rRatio = (REAL)(R1/R2);

		GraphicsPath p;

		p.AddEllipse(rLeft + (rRight - rLeft - (rRight - rLeft)*rRatio)/2, rTop + (rBottom - rTop - (rBottom - rTop)*rRatio)/2,
			(rRight - rLeft)*rRatio - 1, (rBottom - rTop)*rRatio - 1);
		
		if (Solid == VARIANT_TRUE)
		{
			RectF rectGradient(rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
			Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);
			m_graphics->SetClip(&p, CombineModeXor);
			m_graphics->FillPie(brush, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1, rStartAngle, rSweepAngle);
			m_graphics->ResetClip();
			
		}
		else
		{
			Pen* pen = m_pen.GetPen();
			m_graphics->SetClip(&p, CombineModeXor);
			m_graphics->DrawPie(pen, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1, rStartAngle, rSweepAngle);
			m_graphics->ResetClip();
			m_graphics->DrawArc(pen, rLeft + (rRight - rLeft - (rRight - rLeft)*rRatio)/2, rTop + (rBottom - rTop - (rBottom - rTop)*rRatio)/2,
			(rRight - rLeft)*rRatio - 1, (rBottom - rTop)*rRatio - 1, rStartAngle, rSweepAngle);
		}
		return S_OK;
	}
	STDMETHOD(DrawSimplePie)(double CenterX, double CenterY, double Radius1, double StartAngle, double SweepAngle, VARIANT_BOOL Solid)
	{	
		if (!Valid())
			return S_OK;

		REAL rLeft = (REAL)(CenterX - Radius1);
		REAL rTop = (REAL)(CenterY - Radius1);
		REAL rRight = (REAL)(CenterX + Radius1);
		REAL rBottom = (REAL)(CenterY + Radius1);
		REAL rStartAngle = (REAL)StartAngle;
		REAL rSweepAngle = (REAL)SweepAngle;

		if (Solid == VARIANT_TRUE)
		{
			RectF rectGradient(rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1);
			Brush* brush = m_brush.GetBrush(&m_textures, &rectGradient);

			m_graphics->FillPie(brush, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1, rStartAngle, rSweepAngle);
		}
		else
		{
			Pen* pen = m_pen.GetPen();
			m_graphics->DrawPie(pen, rLeft, rTop, rRight - rLeft - 1, rBottom - rTop - 1, rStartAngle, rSweepAngle);			
		}
		return S_OK;
	}
	
	STDMETHOD(get_settingsSmoothText)(VARIANT_BOOL* val)
	{
		*val = m_format.AntiAliasText ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(put_settingsSmoothText)(VARIANT_BOOL val)
	{
		m_format.AntiAliasText = (val != VARIANT_FALSE);

		return S_OK;
	}
	STDMETHOD(get_settingsSmoothLines)(VARIANT_BOOL* val)
	{
		*val = m_format.AntiAliasPen ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(put_settingsSmoothLines)(VARIANT_BOOL val)
	{
		m_format.AntiAliasPen = (val != VARIANT_FALSE);

		return S_OK;
	}
	
	STDMETHOD(get_penColor)(OLE_COLOR* val)
	{
		*val = m_pen.Color;
		
		return S_OK;
	}
	STDMETHOD(put_penColor)(OLE_COLOR val)
	{
		OleTranslateColor(val, NULL, (COLORREF*)&m_pen.Color);
		
		m_pen.Update();
		
		return S_OK;
	}
	STDMETHOD(get_penAlpha)(long* val)
	{
		*val = m_pen.Alpha;
		
		return S_OK;
	}
	STDMETHOD(put_penAlpha)(long val)
	{
		m_pen.Alpha = val;
		
		m_pen.Update();
	
		return S_OK;
	}
	STDMETHOD(get_penSize)(double* val)
	{
		*val = m_pen.Size;
		
		return S_OK;
	}
	STDMETHOD(put_penSize)(double val)
	{
		m_pen.Size = val;
		
		m_pen.Update();
		
		return S_OK;
	}
	
	STDMETHOD(get_brushType)(long* val)
	{
		*val = m_brush.Type;
		
		return S_OK;
	}
	STDMETHOD(put_brushType)(long val)
	{
		m_brush.Type = Painter::ConstantCompatible(val);
		
		m_brush.Update();
		
		return S_OK;
	}
	STDMETHOD(get_brushColor1)(OLE_COLOR* val)
	{
		*val = m_brush.Color1;
		
		return S_OK;
	}
	STDMETHOD(put_brushColor1)(OLE_COLOR val)
	{
		OleTranslateColor(val, NULL, (COLORREF*)&m_brush.Color1);
		
		m_brush.Update();
		
		return S_OK;
	}
	STDMETHOD(get_brushAlpha1)(long* val)
	{
		*val = m_brush.Alpha1;
		
		return S_OK;
	}
	STDMETHOD(put_brushAlpha1)(long val)
	{
		m_brush.Alpha1 = val;

		m_brush.Update();
		
		return S_OK;
	}
	STDMETHOD(get_brushColor2)(OLE_COLOR* val)
	{
		*val = m_brush.Color2;

		return S_OK;
	}
	STDMETHOD(put_brushColor2)(OLE_COLOR val)
	{
		OleTranslateColor(val, NULL, (COLORREF*)&m_brush.Color2);
		
		m_brush.Update();
		
		return S_OK;
	}
	STDMETHOD(get_brushAlpha2)(long* val)
	{
		*val = m_brush.Alpha2;
		
		return S_OK;
	}
	STDMETHOD(put_brushAlpha2)(long val)
	{
		m_brush.Alpha2 = val;
		
		m_brush.Update();
		
		return S_OK;
	}
	STDMETHOD(put_brushTexture)(BSTR bstrName, SAFEARRAY** Image)
	{
		CString Name(bstrName);

		Name.MakeLower();

		int index = m_textures.Add(Name, *Image);

		if (index < 0)
			return S_OK;

		m_brush.TexturePath = Name;
		
		m_brush.Update();
 
		return S_OK;
	}
	STDMETHOD(get_brushTexturePath)(BSTR* val)
	{
		*val = m_brush.TexturePath.AllocSysString();

		return S_OK;
	}
	STDMETHOD(put_brushTexturePath)(BSTR val)
	{
		CString path(val);

		path.MakeLower();

		int index = m_textures.Add(path);

		if (index < 0)
			return S_OK;

		m_brush.TexturePath = path;
		
		m_brush.Update();
 
		return S_OK;
	}
	STDMETHOD(get_brushTextureAlpha)(long* val)
	{
		*val = m_brush.TextureAlpha;

		return S_OK;
	}
	STDMETHOD(put_brushTextureAlpha)(long val)
	{
		m_brush.TextureAlpha = val;

		m_brush.Update();

		return S_OK;
	}
	
	STDMETHOD(get_fontName)(BSTR* val)
	{
		*val = m_font.Name.AllocSysString();

		return S_OK;
	}
	STDMETHOD(put_fontName)(BSTR val)
	{
		m_font.Name = CString(val);

		m_font.Update();

		return S_OK;
	}
	STDMETHOD(get_fontSize)(double* val)
	{
		*val = m_font.Size;

		return S_OK;
	}
	STDMETHOD(put_fontSize)(double val)
	{
		m_font.Size = val;

		m_font.Update();

		return S_OK;
	}
	STDMETHOD(get_fontBold)(VARIANT_BOOL* val)
	{
		*val = m_font.Bold ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(put_fontBold)(VARIANT_BOOL val)
	{
		m_font.Bold = (val != VARIANT_FALSE);

		m_font.Update();

		return S_OK;
	}
	STDMETHOD(get_fontItalic)(VARIANT_BOOL* val)
	{
		*val = m_font.Italic ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(put_fontItalic)(VARIANT_BOOL val)
	{
		m_font.Italic = (val != VARIANT_FALSE);

		m_font.Update();

		return S_OK;
	}
	STDMETHOD(get_fontUnderline)(VARIANT_BOOL* val)
	{
		*val = m_font.Underline ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(put_fontUnderline)(VARIANT_BOOL val)
	{
		m_font.Underline = (val != VARIANT_FALSE);

		m_font.Update();

		return S_OK;
	}
	STDMETHOD(get_fontStrikeout)(VARIANT_BOOL* val)
	{
		*val = m_font.Strikeout ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(put_fontStrikeout)(VARIANT_BOOL val)
	{
		m_font.Strikeout = (val != VARIANT_FALSE);

		m_font.Update();

		return S_OK;
	}
	
	STDMETHOD(get_settingsAlignmentVertical)(long* pVal)
	{
		*pVal = m_format.StringAlignmentVertical;

		return S_OK;
	}
	STDMETHOD(put_settingsAlignmentVertical)(long newVal)
	{
		m_format.StringAlignmentVertical = newVal;

		m_format.Update();

		return S_OK;
	}
	STDMETHOD(get_settingsAlignmentHorizontal)(long* pVal)
	{
		*pVal = m_format.StringAlignmentHorizontal;

		return S_OK;
	}
	STDMETHOD(put_settingsAlignmentHorizontal)(long newVal)
	{
		m_format.StringAlignmentHorizontal = newVal;

		m_format.Update();

		return S_OK;
	}
	STDMETHOD(get_brushTextureMode)(long* pVal)
	{
		*pVal = m_brush.TextureMode;

		return S_OK;
	}
	STDMETHOD(put_brushTextureMode)(long newVal)
	{
		m_brush.TextureMode = newVal;

		m_brush.Update();

		return S_OK;
	}
	STDMETHOD(get_settingsAlphMode)(long* pVal)
	{
		*pVal = m_format.ImageAlphMode;

		return S_OK;
	}
	STDMETHOD(put_settingsAlphMode)(long newVal)
	{
		m_format.ImageAlphMode = newVal;

		return S_OK;
	}
	
	STDMETHOD(SetFormat)(SAFEARRAY** Array)
	{
		if (!Array || !*Array)
			return S_OK;

		BYTE* buffer = (BYTE*)((*Array)->pvData);

		if (!buffer)
			return S_OK;

		Painter::CStream stream;

		stream.SetBuffer(buffer);

		m_pen.Read(stream);
		m_brush.Read(stream);
		m_font.Read(stream);
		m_format.Read(stream);

		return S_OK;
	}
	STDMETHOD(GetFormat)(SAFEARRAY** Array)
	{
		long size = m_pen.GetSize() + m_brush.GetSize() + m_font.GetSize() + m_format.GetSize();

		SAFEARRAYBOUND bound;

		bound.lLbound = 0;
		bound.cElements = size;

		*Array = SafeArrayCreate(VT_UI1, 1, &bound);

		if (!*Array)
			return S_OK;

		BYTE* buffer = (BYTE*)((*Array)->pvData);

		if (!buffer)
			return S_OK;

		Painter::CStream stream;

		stream.SetBuffer(buffer);

		m_pen.Write(stream);
		m_brush.Write(stream);
		m_font.Write(stream);
		m_format.Write(stream);

		return S_OK;
	}
	STDMETHOD(MeasureString)(BSTR textString, DOUBLE* height, DOUBLE* width)
	{
		*height = 0.;
		*width = 0.;

		if (!m_graphics)
			return S_FALSE;

		PointF origin(0.0f, 0.0f);
		RectF boundRect;
		Font* font = m_font.GetFont();
		Status s = m_graphics->MeasureString(textString, ::SysStringLen(textString), font, origin, &boundRect);
		if (s != Ok)
			return S_FALSE;
		
		*height = boundRect.Height;
		*width = boundRect.Width;

		return S_OK;
	}
	STDMETHOD(DrawBorder)(long Size, long Type);
	STDMETHOD(DrawMovObject)(double Frame, long Type, long Divisions, long Color, long Alpha);
	STDMETHOD(DrawCredits)(double Frame, BSTR Text, long Type);
};

