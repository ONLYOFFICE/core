#pragma once
	
#include "resource.h"
#include <registration.h>
#include <PaintStruct.h>
#include "TextFormat.h"
#include "TextToPath.h"
#include <ImageColors.h>
#include <ImageCore.h>
	
[object, uuid("D7DC8BC9-3F5A-4de4-BFB1-F92930AF14EE"), dual, helpstring("IImageTextStatic3 Interface"), pointer_default(unique)]
__interface IImageTextStatic3 : IDispatch
{
	// general
	[id(1)] HRESULT Draw([in,out, satype(BYTE)] SAFEARRAY ** Image, [in] BSTR Text, [in] long Left, [in] long Top, [in] long Right, [in] long Bottom);
	[id(2)] HRESULT DrawMedia([in, out] IUnknown** Image, [in] BSTR Text, [in] long Left, [in] long Top, [in] long Right, [in] long Bottom);
	[id(3)] HRESULT DrawEllipse([in,out, satype(BYTE)] SAFEARRAY ** Image, [in] BSTR Text, [in] long Left, [in] long Top, [in] long Right, [in] long Bottom, [in] double StartAngle, [in] double SweepAngle);
	[id(4)] HRESULT DrawMediaEllipse([in, out] IUnknown** Image, [in] BSTR Text, [in] long Left, [in] long Top, [in] long Right, [in] long Bottom, [in] double StartAngle, [in] double SweepAngle);
	[id(5)] HRESULT DrawPolyline([in,out, satype(BYTE)] SAFEARRAY ** Image, [in, satype("double")] SAFEARRAY** Array, [in] BSTR Text);
	[id(6)] HRESULT DrawMediaPolyline([in, out] IUnknown** Image, [in, satype("double")] SAFEARRAY** Array, [in] BSTR Text);

	// font
	[id(101), propget] HRESULT fontName([out, retval] BSTR* pVal);
	[id(101), propput] HRESULT fontName([in] BSTR newVal);
	[id(102), propget] HRESULT fontSize([out, retval] double* pVal);
	[id(102), propput] HRESULT fontSize([in] double newVal);
	[id(103), propget] HRESULT fontBold([out, retval] VARIANT_BOOL* pVal);
	[id(103), propput] HRESULT fontBold([in] VARIANT_BOOL newVal);
	[id(104), propget] HRESULT fontItalic([out, retval] VARIANT_BOOL* pVal);
	[id(104), propput] HRESULT fontItalic([in] VARIANT_BOOL newVal);
	[id(105), propget] HRESULT fontUnderline([out, retval] VARIANT_BOOL* pVal);
	[id(105), propput] HRESULT fontUnderline([in] VARIANT_BOOL newVal);
	[id(106), propget] HRESULT fontStrikeout([out, retval] VARIANT_BOOL* pVal);
	[id(106), propput] HRESULT fontStrikeout([in] VARIANT_BOOL newVal);

	// brush
	[id(203), propget] HRESULT brushType([out, retval] long* pVal);
	[id(203), propput] HRESULT brushType([in] long newVal);
	[id(204), propget] HRESULT brushColor1([out, retval] long* pVal);
	[id(204), propput] HRESULT brushColor1([in] long newVal);
	[id(205), propget] HRESULT brushColor2([out, retval] long* pVal);
	[id(205), propput] HRESULT brushColor2([in] long newVal);
	[id(206), propget] HRESULT brushAlpha1([out, retval] long* pVal);
	[id(206), propput] HRESULT brushAlpha1([in] long newVal);
	[id(207), propget] HRESULT brushAlpha2([out, retval] long* pVal);
	[id(207), propput] HRESULT brushAlpha2([in] long newVal);
	[id(208), propget] HRESULT brushTexturePath([out, retval] BSTR* val);
	[id(208), propput] HRESULT brushTexturePath([in] BSTR val);
	[id(209), propget] HRESULT brushTextureAlpha([out, retval] long* val);
	[id(209), propput] HRESULT brushTextureAlpha([in] long val);
	[id(210), propget] HRESULT brushTextureMode([out, retval] long* val);
	[id(210), propput] HRESULT brushTextureMode([in] long val);
	[id(211), propput] HRESULT brushTexture([in] BSTR Name, [in, satype("BYTE")] SAFEARRAY** Image);

	// formatting
	[id(301), propget] HRESULT formatAlignVertical([out, retval] long* pVal);
	[id(301), propput] HRESULT formatAlignVertical([in] long newVal);
	[id(302), propget] HRESULT formatAlignHorizontal([out, retval] long* pVal);
	[id(302), propput] HRESULT formatAlignHorizontal([in] long newVal);
	[id(303), propget] HRESULT formatRotate([out, retval] long* pVal);
	[id(303), propput] HRESULT formatRotate([in] long newVal);

	// shadow
	[id(401), propget] HRESULT shadowVisible([out, retval] VARIANT_BOOL* pVal);
	[id(401), propput] HRESULT shadowVisible([in] VARIANT_BOOL newVal);
	[id(402), propget] HRESULT shadowDistanceX([out, retval] long* pVal);
	[id(402), propput] HRESULT shadowDistanceX([in] long newVal);
	[id(403), propget] HRESULT shadowDistanceY([out, retval] long* pVal);
	[id(403), propput] HRESULT shadowDistanceY([in] long newVal);
	[id(404), propget] HRESULT shadowColor([out, retval] long* pVal);
	[id(404), propput] HRESULT shadowColor([in] long newVal);
	[id(405), propget] HRESULT shadowAlpha([out, retval] long* pVal);
	[id(405), propput] HRESULT shadowAlpha([in] long newVal);
	[id(406), propget] HRESULT shadowBlur([out, retval] long* pVal);
	[id(406), propput] HRESULT shadowBlur([in] long newVal);

	// edge
	[id(501), propget] HRESULT edgeVisible([out, retval] long* pVal);
	[id(501), propput] HRESULT edgeVisible([in] long newVal);
	[id(502), propget] HRESULT edgeDistance([out, retval] long* pVal);
	[id(502), propput] HRESULT edgeDistance([in] long newVal);
	[id(503), propget] HRESULT edgeColor([out, retval] long* pVal);
	[id(503), propput] HRESULT edgeColor([in] long newVal);
	[id(504), propget] HRESULT edgeAlpha([out, retval] long* pVal);
	[id(504), propput] HRESULT edgeAlpha([in] long newVal);

	// presets
	[id(601), propget] HRESULT currentPresetName([out, retval] BSTR* pVal);
	[id(601), propput] HRESULT currentPresetName([in] BSTR newVal);
	[id(602), propget] HRESULT currentPresetIndex([out, retval] long* pVal);
	[id(602), propput] HRESULT currentPresetIndex([in] long newVal);

	[id(651)] HRESULT SavePresets([in] BSTR path);
	[id(652)] HRESULT LoadPresets([in] BSTR path, VARIANT_BOOL append);
	[id(653)] HRESULT GetPresetCount([out,retval] long* count);
	[id(654)] HRESULT GetPresetName([in] long index, [out] BSTR* name);
	[id(655)] HRESULT DeletePreset([in] BSTR path, [in] long index);
	[id(656)] HRESULT SetFormat([in, satype("BYTE")] SAFEARRAY** Array);
	[id(657)] HRESULT GetFormat([out, retval, satype("BYTE")] SAFEARRAY** Array);
};
	
[ coclass, uuid("188CF510-A263-45e2-B786-76D4C1427533"), threading(apartment), vi_progid("AVSImagePaint3.ImageTextStatic3"), progid("AVSImagePaint3.ImageTextStatic3.1"), version(1.0), helpstring("ImageTextStatic3 Class")]
class ATL_NO_VTABLE ImageTextStatic3 : public IImageTextStatic3
{
protected:
	
	// general
	CGdiPlusInit initGdiPlus;
	
	// painting formats
	Painter::CBrush m_brush;
	Painter::CFont m_font;
	Painter::CFormat m_format;
	Painter::CShadow m_shadow;
	Painter::CEdgeText m_edge;
	Painter::CTextureManager m_textures;
	ImageStudio::Core::MediaData m_oMediaData;
	
	// primary settings
	long m_nRotateAngle;
	
	// presets
	CString m_sCurrentPresetName;
	long m_nCurentPresetIndex;
	CAtlArray<TextFormat> m_arrFormats;
	
protected:
	
	ImageTextStatic3()
	{
		initGdiPlus.Init();
		m_nRotateAngle = 0;
		m_nCurentPresetIndex = 0;
		m_sCurrentPresetName = "Preset";
	}

	~ImageTextStatic3()
	{
		Clear();

		m_arrFormats.RemoveAll();
	}
	
	void Clear()
	{
		m_brush.Clear();
		m_font.Clear();
		m_format.Clear();
		m_shadow.Clear();
		m_edge.Clear();
		m_textures.Clear();
	}
	Gdiplus::Brush* CreateBrush(GraphicsPath& path, Gdiplus::Color& color1, Gdiplus::Color& color2, int index, RectF& rectCharacter)
	{
		Brush* brush = NULL;
		int k = 4;
		if (Painter::c_BrushTypePathGradient1 == m_brush.Type)
		{
			Color colors[] = {color1, color1, color1, color1};
			PathGradientBrush* pBrush = new PathGradientBrush(&path);
			pBrush->SetSurroundColors(colors, &k);
			pBrush->SetCenterColor(color2);
//			pBrush->SetCenterPoint(PointF(rectCharacter.X + rectCharacter.Width - 1, rectCharacter.Y + rectCharacter.Height/2.));

			brush = pBrush;
		}
		if (Painter::c_BrushTypePathGradient2 == m_brush.Type)
		{
/*
// beg variant 1
			PathGradientBrush* pBrush = new PathGradientBrush(&path);
			Color colors1[] = {color1, color2, color2, color1};
			Color colors2[] = {color2, color1, color1, color2};

			if (index%2 == 0)
			{
				pBrush->SetSurroundColors(colors1, &k);
				pBrush->SetCenterColor(color1);
			}
			if (index%2 == 1)
			{
				pBrush->SetSurroundColors(colors2, &k);
				pBrush->SetCenterColor(color2);
			}
// end variant 1
// beg variant 2
//			LinearGradientBrush* pBrush = new Gdiplus::LinearGradientBrush(rectCharacter, color1, color2, LinearGradientModeHorizontal);
//			Status s = pBrush->SetWrapMode(WrapModeClamp);
// end variant 2
// beg variant 3
//			LinearGradientBrush* pBrush = new Gdiplus::LinearGradientBrush(rectCharacter, brushColor1, brushColor2, -180.F);
//			pBrush->SetBlendBellShape(0.50f);
//			Status s = pBrush->SetWrapMode(WrapModeClamp);
// end variant 3
// beg variant 4 gradient horizontal
			Color colors[] = {color1, color2, color2, color1};
			PathGradientBrush* pBrush = new PathGradientBrush(&path);
			pBrush->SetSurroundColors(colors, &k);
			pBrush->SetCenterColor(color1);
			pBrush->SetCenterPoint(PointF(rectCharacter.X, rectCharacter.Y + rectCharacter.Height/2.));
// end variant 4
*/
// beg variant 5
			LinearGradientBrush* pBrush = new Gdiplus::LinearGradientBrush(rectCharacter, color1, color2, LinearGradientModeHorizontal);
			Status s = pBrush->SetWrapMode(WrapModeTileFlipX);
// end variant 5
/*
			Color colors[] = {color2, color2, color2, color2};
			PathGradientBrush* pBrush = new PathGradientBrush(&path);
			pBrush->SetSurroundColors(colors, &k);
			pBrush->SetCenterColor(color1);
*/
			brush = pBrush;
		}

		return brush;
	}
    void DrawPathGradientText(Graphics* graph, BSTR Text, int length, RectF& rectDraw, RectF& StringBox)
	{
		Font* myFont = m_font.GetFont();
		StringFormat* myStringFormat = m_format.GetStringFormat();
		Gdiplus::Color color1((BYTE)m_brush.Alpha1, GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
		Gdiplus::Color color2((BYTE)m_brush.Alpha2, GetRValue(m_brush.Color2), GetGValue(m_brush.Color2), GetBValue(m_brush.Color2));
		GraphicsPath path;
		for(int index = 0; index < length; index++)
		{
			// get rect of current(index) character
			RectF rectCharacter;
			Painter::GetRectCurrentCharacter(graph, myStringFormat, myFont, Text, length, index, StringBox, rectCharacter);

			// create path for rect and brush
			path.AddRectangle(rectCharacter);
			Brush* pBrush = CreateBrush(path, color1, color2, index, rectCharacter);

			// draw text
//			rectCharacter.Height = rectDraw.Height;
//			rectCharacter.Y = rectDraw.Y;
//			graph->DrawString(Text+index, 1, myFont, rectCharacter, myStringFormat, pBrush);
			graph->DrawString(Text, length, myFont, rectDraw, myStringFormat, pBrush);

//			graph->FillRectangle(pBrush, rectDraw);
//			Pen blackPen(Color(255, 0, 0, 0), 3);
//			graph->DrawRectangle(&blackPen, rectCharacter);
//			graph->FillRectangle(myBrush, rectCharacter);

			// delete brush and reset path
			if (pBrush)
				delete pBrush;
			path.Reset();
		}
	}
	Bitmap* CreateBitmap(int nWidth, int nHeight, BYTE* pPixels)
	{
		if (!pPixels || nWidth < 2 || nHeight < 2)
			return NULL;

		// variables
		BITMAPINFO info;

		// prepare header for the image
		info.bmiHeader.biSize =				sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biWidth =			nWidth;
		info.bmiHeader.biHeight =			nHeight;
		info.bmiHeader.biBitCount =			32;
		info.bmiHeader.biPlanes =			1;
		info.bmiHeader.biClrImportant =		0;
		info.bmiHeader.biClrUsed =			0;
		info.bmiHeader.biCompression =		0;
		info.bmiHeader.biXPelsPerMeter =	0;
		info.bmiHeader.biYPelsPerMeter =	0;
		info.bmiHeader.biSizeImage =		0;

		return new Bitmap(&info, pPixels);
	}
	Bitmap* CreateBitmap(void** Image, BOOL bArray, ImageStudio::Core::MediaData& oMediaData)
	{
		if (!Image || !*Image)
			return NULL;

		// create bitmap
		int nWidth = 0;
		int nHeight = 0;
		BYTE* pPixels = 0;
	
		if (bArray)
		{
			int nChannels = 0;
			
			pPixels = ImageStudioUtils::ExtractImage((SAFEARRAY**)Image, nWidth, nHeight, nChannels);

			if (nChannels != 4)
				return NULL;
		}
		else
		{
			if (!oMediaData.Create(*(IUnknown**)Image, FALSE) || !oMediaData.IsValidBGRA())
				return NULL;

			nWidth = oMediaData.GetWidth();
			nHeight = oMediaData.GetHeight();
			pPixels = oMediaData.GetBuffer();

			*Image = oMediaData.GetMediaData(TRUE);
		}

		if (!pPixels)
			return NULL;

		return CreateBitmap(nWidth, nHeight, pPixels);
	}
	void Draw(void** Image, BSTR Text, long Left, long Top, long Right, long Bottom, BOOL bArray = TRUE)
	{	
		if (!CRegistratorClient::IsRegistered())
			return;

		// create bitmap
		ImageStudio::Core::MediaData oMediaData;
		Bitmap* pBitmap = CreateBitmap(Image, bArray, oMediaData);
		if (!pBitmap)
			return;

		// create graphics
		Graphics* Gr = new Graphics(pBitmap);
		Gr->SetTextRenderingHint(TextRenderingHintAntiAlias);
		Gr->SetSmoothingMode(SmoothingModeAntiAlias);
		
		RectF rectDraw((REAL)Left, (REAL)Top, (REAL)(Right - Left), (REAL)(Bottom - Top));

		// get lenght of string
		CString sText; sText = Text;
		int len = sText.GetLength();

		// get bound rect for string
		RectF StringBox;
		Font* myFont = m_font.GetFont();
		StringFormat* myStringFormat = m_format.GetStringFormat();
	    Gr->MeasureString(Text, len, myFont, rectDraw, myStringFormat, &StringBox);

		// apply rotation of the image around center ponit
		Matrix oTransform; oTransform.Reset();
		PointF oTransformCenterPoint(StringBox.X + StringBox.Width/2, StringBox.Y + StringBox.Height/2);
		oTransform.RotateAt(m_nRotateAngle, oTransformCenterPoint);
		Gr->SetTransform(&oTransform);

		// draw shadow
		m_shadow.Draw(Gr, myFont, myStringFormat, Text, len, rectDraw);
	
		// draw edge
		m_edge.Draw(Gr, myFont, myStringFormat, Text, len, rectDraw);
	
		// draw the text
		if ((Painter::c_BrushTypePathGradient1 != m_brush.Type) && (Painter::c_BrushTypePathGradient2 != m_brush.Type))
		{
			Brush* myBrush = m_brush.GetBrush(&m_textures, &StringBox);
			Gr->DrawString(Text, len, myFont, rectDraw, myStringFormat, myBrush);  
		}
		else
			DrawPathGradientText(Gr, Text, len, rectDraw, StringBox);

		// delete bitmap
		if (Gr)
			delete Gr;  
		if (pBitmap)
			delete pBitmap;
	}
	void DrawEllipse(void** Image, BSTR Text, long Left, long Top, long Right, long Bottom, double StartAngle, double SweepAngle, BOOL bArray = TRUE)
	{	
		if (!CRegistratorClient::IsRegistered())
			return;

		// create bitmap
		ImageStudio::Core::MediaData oMediaData;
		Bitmap* pBitmap = CreateBitmap(Image, bArray, oMediaData);
		if (!pBitmap)
			return;

		// create graphics
		Graphics* Gr = new Graphics(pBitmap);
		Gr->SetTextRenderingHint(TextRenderingHintAntiAlias);
		Gr->SetSmoothingMode(SmoothingModeAntiAlias);

		// get lenght of string
		CString sText; sText = Text;
		int len = sText.GetLength();

		// create ellipse path
		GraphicsPath path; path.AddArc((REAL)Left, (REAL)Top, (REAL)(Right - Left), (REAL)(Bottom - Top), (REAL)StartAngle, (REAL)SweepAngle);

		// draw ellipse text
		Font* font = m_font.GetFont();
		CTextToPath tp(path);
		tp.SetAlignmentVertical(m_format.StringAlignmentVertical);
		tp.SetAlignmentHorizontal(m_format.StringAlignmentHorizontal);
//		tp.SetKerning(0.5);
		tp.DrawText(Gr, font, &m_brush, &m_textures, &m_edge, &m_shadow, Text, len);
//		tp.DrawLine(Gr);

		// delete bitmap
		if (Gr)
			delete Gr;  
		if (pBitmap)
			delete pBitmap;
	}
	void DrawPolyline(void** Image, SAFEARRAY** Array, BSTR Text, BOOL bArray = TRUE)
	{
		if (!CRegistratorClient::IsRegistered())
			return;

		// check Polyline
		if (!Array || !*Array)
			return;
		int nCount = (*Array)->rgsabound[0].cElements;
		if (nCount % 2 || nCount < 4)
			return;
		nCount /= 2;

		// create bitmap
		ImageStudio::Core::MediaData oMediaData;
		Bitmap* pBitmap = CreateBitmap(Image, bArray, oMediaData);
		if (!pBitmap)
			return;

		// create graphics
		Graphics* Gr = new Graphics(pBitmap);
		Gr->SetTextRenderingHint(TextRenderingHintAntiAlias);
		Gr->SetSmoothingMode(SmoothingModeAntiAlias);

		// get lenght of string
		CString sText; sText = Text;
		int len = sText.GetLength();

		// variables
		PointF* pPoints = new PointF[nCount];
		double* pDoubles = (double*)((*Array)->pvData);
		
		// compose array of points to draw
		for (int nIndex = 0; nIndex < nCount; ++nIndex, pDoubles += 2)
		{
			pPoints[nIndex].X = (REAL)pDoubles[0];
			pPoints[nIndex].Y = (REAL)pDoubles[1];
		}

		// create polyline path
		GraphicsPath path; 
		path.AddCurve(pPoints, nCount);

		// draw polyline text
		Font* font = m_font.GetFont();
		CTextToPath tp(path);
		tp.SetAlignmentVertical(m_format.StringAlignmentVertical);
		tp.SetAlignmentHorizontal(m_format.StringAlignmentHorizontal);
//		tp.SetKerning(0.5);
		tp.DrawText(Gr, font, &m_brush, &m_textures, &m_edge, &m_shadow, Text, len);
//		tp.DrawLine(Gr);

		// delete bitmap and points
		if (Gr)
			delete Gr;  
		if (pBitmap)
			delete pBitmap;
		if (pPoints)
			delete pPoints;
	}
	
	// get font style
	void GetFontStyle(long fontStyle)
	{
		m_font.Bold = (fontStyle&1)? TRUE:FALSE;
		m_font.Italic = (fontStyle&2)? TRUE:FALSE;
		m_font.Underline = (fontStyle&4)? TRUE:FALSE;
		m_font.Strikeout = (fontStyle&8)? TRUE:FALSE;
	}
	
public:
	// general
	STDMETHOD(Draw)(SAFEARRAY** Image, BSTR Text, long Left, long Top, long Right, long Bottom)
	{
		Draw((void**)Image, Text, Left, Top, Right, Bottom, TRUE);

		return S_OK;
	}
	STDMETHOD(DrawMedia)(IUnknown** Image, BSTR Text, long Left, long Top, long Right, long Bottom)
	{	
		Draw((void**)Image, Text, Left, Top, Right, Bottom, FALSE);

		return S_OK;
	}
	STDMETHOD(DrawEllipse)(SAFEARRAY** Image, BSTR Text, long Left, long Top, long Right, long Bottom, double StartAngle, double SweepAngle)
	{
		DrawEllipse((void**)Image, Text, Left, Top, Right, Bottom, StartAngle, SweepAngle, TRUE);

		return S_OK;
	}
	STDMETHOD(DrawMediaEllipse)(IUnknown** Image, BSTR Text, long Left, long Top, long Right, long Bottom, double StartAngle, double SweepAngle)
	{
		DrawEllipse((void**)Image, Text, Left, Top, Right, Bottom, StartAngle, SweepAngle, FALSE);

		return S_OK;
	}
	STDMETHOD(DrawPolyline)(SAFEARRAY** Image, SAFEARRAY** Array, BSTR Text)
	{
		DrawPolyline((void**)Image, Array, Text, TRUE);

		return S_OK;
	}
	STDMETHOD(DrawMediaPolyline)(IUnknown** Image, SAFEARRAY** Array, BSTR Text)
	{
		DrawPolyline((void**)Image, Array, Text, FALSE);

		return S_OK;
	}

	STDMETHOD(SavePresets)(BSTR path) 
	{
		try
		{
			// store current preset
			int nLastIndex = m_arrFormats.GetCount();
			m_arrFormats.Add();
			Gdiplus::Color brushColor1((BYTE)m_brush.Alpha1, GetRValue(m_brush.Color1), GetGValue(m_brush.Color1), GetBValue(m_brush.Color1));
			Gdiplus::Color brushColor2((BYTE)m_brush.Alpha2, GetRValue(m_brush.Color2), GetGValue(m_brush.Color2), GetBValue(m_brush.Color2));

			m_arrFormats[nLastIndex].m_nTextureAlpha = (BYTE)m_brush.TextureAlpha;
			m_arrFormats[nLastIndex].m_cShadowColor = Gdiplus::Color((BYTE)m_shadow.Alpha, GetRValue(m_shadow.Color), GetGValue(m_shadow.Color), GetBValue(m_shadow.Color));
			m_arrFormats[nLastIndex].m_nShadowBlur = m_shadow.BlurSize;
			m_arrFormats[nLastIndex].m_nRotateAngle = m_nRotateAngle;
			m_arrFormats[nLastIndex].m_sTextureFilePath = m_brush.TexturePath;
			m_arrFormats[nLastIndex].m_bShadowVisible = m_shadow.Visible;
			m_arrFormats[nLastIndex].m_nEdgeVisible = m_edge.Visible;
			m_arrFormats[nLastIndex].m_nHatchType = m_brush.Type - Painter::c_BrushTypeHatch1;
			m_arrFormats[nLastIndex].m_cFontHatchColor2 = brushColor2;
			m_arrFormats[nLastIndex].m_cFontHatchColor1 = brushColor1;
			m_arrFormats[nLastIndex].m_cFontGradColor2 = brushColor2;
			m_arrFormats[nLastIndex].m_cFontGradColor1 = brushColor1;
			m_arrFormats[nLastIndex].m_cFontColor = brushColor1;
			m_arrFormats[nLastIndex].m_nEdgeDist = m_edge.Dist;
			m_arrFormats[nLastIndex].m_cEdgeColor = Gdiplus::Color((BYTE)m_edge.Alpha, GetRValue(m_edge.Color), GetGValue(m_edge.Color), GetBValue(m_edge.Color));
			m_arrFormats[nLastIndex].m_nShadowDistanceY = m_shadow.DistanceY;
			m_arrFormats[nLastIndex].m_nShadowDistanceX = m_shadow.DistanceX;
			m_arrFormats[nLastIndex].m_nBrushFaceColorType = m_brush.Type;
			m_arrFormats[nLastIndex].m_nBrushFaceColorMode = m_brush.Type - Painter::c_BrushTypeHorizontal;
			m_arrFormats[nLastIndex].m_nAlignVertical = m_format.StringAlignmentVertical;
			m_arrFormats[nLastIndex].m_nAlignHorizontal = m_format.StringAlignmentHorizontal;
			m_arrFormats[nLastIndex].m_sCurrentPresetName = m_sCurrentPresetName;

			// add font
			Font* myFont = m_font.GetFont();
			m_arrFormats[nLastIndex].m_sFontName = m_font.Name;
			m_arrFormats[nLastIndex].m_nFontSize = myFont->GetSize();
			m_arrFormats[nLastIndex].m_nFontStyle = myFont->GetStyle();

			// save all presets to file
			XmlUtils::CXmlWriter oXmlWriter;

			// open presets section
			oXmlWriter.WriteNodeBegin("presets");

			// write presets
			for (int nIndex = 0; nIndex < m_arrFormats.GetCount(); nIndex++)
				oXmlWriter.WriteString(m_arrFormats[nIndex].ToXmlString());

			// close presets section
			oXmlWriter.WriteNodeEnd("presets");

			// write composed xml string to the file
			CString sFileXml(path);
			oXmlWriter.SaveToFile(sFileXml, TRUE);
		}
		catch (...)
		{
			return S_FALSE;
		}

		return S_OK;
	}
	STDMETHOD(LoadPresets)(BSTR path, VARIANT_BOOL append) 
	{
		try
		{
			if (append == VARIANT_FALSE)
				m_arrFormats.RemoveAll();

			XmlUtils::CXmlReader oXmlReader;
			if (!oXmlReader.OpenFromFile(CString(path)))
				return S_FALSE;

			oXmlReader.ReadRootNode("presets");

			// all presets
			oXmlReader.ReadNodeList("preset");
			for (int nIndex = 0; nIndex < oXmlReader.GetLengthList(); nIndex++)
			{
				m_arrFormats.Add();
				int r = m_arrFormats.GetCount();
				m_arrFormats[r - 1].FromXmlString(oXmlReader.ReadNodeXml(nIndex));
			}
		}
		catch (...)
		{
			return S_FALSE;
		}

		return S_OK;
	}
	STDMETHOD(GetPresetCount)(LONG* count) 
	{
		*count = m_arrFormats.GetCount();

		return S_OK;
	}
	STDMETHOD(GetPresetName)(long index, BSTR* name)
	{
		if (index >= 0 && index < m_arrFormats.GetCount())
			*name = m_arrFormats[index].m_sCurrentPresetName.AllocSysString();
		else
			*name = L"error";

		return S_OK;
	}

	STDMETHOD(DeletePreset)(BSTR path, long index)
	{
		try
		{
			XML::IXMLDOMDocumentPtr pDocument; pDocument.CreateInstance(CLSID_DOMDocument);
			if (NULL == pDocument)
				return S_FALSE;

			if (VARIANT_FALSE == pDocument->load(path))
				return S_FALSE;

			XML::IXMLDOMElementPtr pRootNode = pDocument->selectSingleNode("presets");
			if (NULL == pRootNode)
				return S_FALSE;

			XML::IXMLDOMNodeListPtr pNodeList = pRootNode->selectNodes("preset");
			if (NULL == pNodeList || index < 0 || index >= pNodeList->Getlength())
				return S_FALSE;

			XML::IXMLDOMNodePtr pNode = pNodeList->item[index];
			if (NULL == pNode)
				return S_FALSE;

			pRootNode->removeChild(pNode);
			pDocument->save(path);
		}
		catch(...)
		{
			return S_FALSE;
		}

		return S_OK;
	}
			
	STDMETHOD(get_currentPresetName)(BSTR* pVal)	
	{ 
		*pVal = m_sCurrentPresetName.AllocSysString();

		return S_OK; 
	}
	STDMETHOD(put_currentPresetName)(BSTR newVal)	
	{ 
		m_sCurrentPresetName = newVal; 

		return S_OK; 
	}
	STDMETHOD(get_currentPresetIndex)(long* pVal)	
	{ 
		*pVal = m_nCurentPresetIndex;

		return S_OK; 
	}
	STDMETHOD(put_currentPresetIndex)(long newVal)
	{
		if (newVal < 0 || newVal >= m_arrFormats.GetCount())
			return S_FALSE;

		m_nCurentPresetIndex = newVal;

		m_brush.TextureAlpha = m_arrFormats[m_nCurentPresetIndex].m_nTextureAlpha;
		Color shadowColor = m_arrFormats[m_nCurentPresetIndex].m_cShadowColor;
			m_shadow.Alpha = shadowColor.GetA(); 
			m_shadow.Color = RGB(shadowColor.GetR(), shadowColor.GetG(), shadowColor.GetB());
		m_shadow.BlurSize = m_arrFormats[m_nCurentPresetIndex].m_nShadowBlur;
		m_nRotateAngle = m_arrFormats[m_nCurentPresetIndex].m_nRotateAngle;
		m_brush.TexturePath = m_arrFormats[m_nCurentPresetIndex].m_sTextureFilePath;
		m_shadow.Visible = m_arrFormats[m_nCurentPresetIndex].m_bShadowVisible;
		m_edge.Visible = m_arrFormats[m_nCurentPresetIndex].m_nEdgeVisible;
		m_edge.Dist = m_arrFormats[m_nCurentPresetIndex].m_nEdgeDist;
		Color edgeColor = m_arrFormats[m_nCurentPresetIndex].m_cEdgeColor;
			m_edge.Alpha = edgeColor.GetA(); 
			m_edge.Color = RGB(edgeColor.GetR(), edgeColor.GetG(), edgeColor.GetB());
		m_shadow.DistanceY = m_arrFormats[m_nCurentPresetIndex].m_nShadowDistanceY;
		m_shadow.DistanceX = m_arrFormats[m_nCurentPresetIndex].m_nShadowDistanceX;
		m_brush.Type = Painter::ConstantCompatible(m_arrFormats[m_nCurentPresetIndex].m_nBrushFaceColorType);
//		int hatchType = m_arrFormats[m_nCurentPresetIndex].hatchType;
//		int brushFaceColorMode = m_arrFormats[m_nCurentPresetIndex].brushFaceColorMode;
		m_format.StringAlignmentVertical = m_arrFormats[m_nCurentPresetIndex].m_nAlignVertical;
		m_format.StringAlignmentHorizontal = m_arrFormats[m_nCurentPresetIndex].m_nAlignHorizontal;
		m_sCurrentPresetName = m_arrFormats[m_nCurentPresetIndex].m_sCurrentPresetName;

		// add font
		m_font.Name = m_arrFormats[m_nCurentPresetIndex].m_sFontName;
		m_font.Size = m_arrFormats[m_nCurentPresetIndex].m_nFontSize;
		GetFontStyle(m_arrFormats[m_nCurentPresetIndex].m_nFontStyle);

		// retrieve brush color
		if ((m_brush.Type >= Painter::c_BrushTypeHorizontal && m_brush.Type <= Painter::c_BrushTypeDiagonal2) || 
			(m_brush.Type == Painter::c_BrushTypeCenter))
		{
			m_brush.Color1 = RGB(m_arrFormats[m_nCurentPresetIndex].m_cFontGradColor1.GetR(), m_arrFormats[m_nCurentPresetIndex].m_cFontGradColor1.GetG(), m_arrFormats[m_nCurentPresetIndex].m_cFontGradColor1.GetB());
			m_brush.Alpha1 = m_arrFormats[m_nCurentPresetIndex].m_cFontGradColor1.GetA();
			m_brush.Color2 = RGB(m_arrFormats[m_nCurentPresetIndex].m_cFontGradColor2.GetR(), m_arrFormats[m_nCurentPresetIndex].m_cFontGradColor2.GetG(), m_arrFormats[m_nCurentPresetIndex].m_cFontGradColor2.GetB());
			m_brush.Alpha2 = m_arrFormats[m_nCurentPresetIndex].m_cFontGradColor2.GetA();
		}
		else if (m_brush.Type >= Painter::c_BrushTypeHatch1 && m_brush.Type <= Painter::c_BrushTypeHatch53)
		{
			m_brush.Color1 = RGB(m_arrFormats[m_nCurentPresetIndex].m_cFontHatchColor1.GetR(), m_arrFormats[m_nCurentPresetIndex].m_cFontHatchColor1.GetG(), m_arrFormats[m_nCurentPresetIndex].m_cFontHatchColor1.GetB());
			m_brush.Alpha1 = m_arrFormats[m_nCurentPresetIndex].m_cFontHatchColor1.GetA();
			m_brush.Color2 = RGB(m_arrFormats[m_nCurentPresetIndex].m_cFontHatchColor2.GetR(), m_arrFormats[m_nCurentPresetIndex].m_cFontHatchColor2.GetG(), m_arrFormats[m_nCurentPresetIndex].m_cFontHatchColor2.GetB());
			m_brush.Alpha2 = m_arrFormats[m_nCurentPresetIndex].m_cFontHatchColor2.GetA();
		}
		else  // if (m_brush.Type == Painter::c_BrushTypeSolid)
		{
			m_brush.Color1 = RGB(m_arrFormats[m_nCurentPresetIndex].m_cFontColor.GetR(), m_arrFormats[m_nCurentPresetIndex].m_cFontColor.GetG(), m_arrFormats[m_nCurentPresetIndex].m_cFontColor.GetB());
			m_brush.Alpha1 = m_arrFormats[m_nCurentPresetIndex].m_cFontColor.GetA();
			m_brush.Color2 = RGB(m_arrFormats[m_nCurentPresetIndex].m_cFontColor.GetR(), m_arrFormats[m_nCurentPresetIndex].m_cFontColor.GetG(), m_arrFormats[m_nCurentPresetIndex].m_cFontColor.GetB());
			m_brush.Alpha2 = m_arrFormats[m_nCurentPresetIndex].m_cFontColor.GetA();
		}

		Clear();

		return S_OK;
	}
	
	// brush
	STDMETHOD(get_brushType)(long* pVal)
	{
		*pVal = m_brush.Type;
		return S_OK;
	}
	STDMETHOD(put_brushType)(long newVal)
	{
		m_brush.Type = Painter::ConstantCompatible(newVal);
		m_brush.Update();
		return S_OK;
	}
	STDMETHOD(get_brushColor1)(long* pVal)
	{ 
		*pVal = m_brush.Color1; 
		return S_OK; 
	}
	STDMETHOD(put_brushColor1)(long newVal)
	{ 
		m_brush.Color1 = newVal;
		m_brush.Update();
		return S_OK; 
	}
	STDMETHOD(get_brushColor2)(long* pVal)
	{ 
		*pVal = m_brush.Color2; 
		return S_OK; 
	}
	STDMETHOD(put_brushColor2)(long newVal)
	{ 
		m_brush.Color2 = newVal;
		m_brush.Update();
		return S_OK; 
	}
	STDMETHOD(get_brushAlpha1)(long* pVal)
	{ 
		*pVal = m_brush.Alpha1; 
		return S_OK; 
	}
	STDMETHOD(put_brushAlpha1)(long newVal)
	{ 
		m_brush.Alpha1 = newVal;
		m_brush.Update();
		return S_OK; 
	}
	STDMETHOD(get_brushAlpha2)(long* pVal)
	{ 
		*pVal = m_brush.Alpha2; 
		return S_OK; 
	}
	STDMETHOD(put_brushAlpha2)(long newVal)
	{ 
		m_brush.Alpha2 = newVal;
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
	
	// font
	STDMETHOD(get_fontName)(BSTR* pVal)
	{
		*pVal = m_font.Name.AllocSysString();

		return S_OK;
	}
	STDMETHOD(put_fontName)(BSTR newVal)
	{
		m_font.Name = CString(newVal);
		m_font.Update();
		return S_OK;
	}
	STDMETHOD(get_fontSize)(DOUBLE* pVal)
	{
		*pVal = m_font.Size;
		return S_OK;
	}
	STDMETHOD(put_fontSize)(DOUBLE newVal)
	{
		m_font.Size = newVal;
		m_font.Update();
		return S_OK;
	}
	STDMETHOD(get_fontBold)(VARIANT_BOOL* pVal)
	{
		*pVal = m_font.Bold ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_fontBold)(VARIANT_BOOL newVal)
	{
		m_font.Bold = (newVal != VARIANT_FALSE);
		m_font.Update();
		return S_OK;
	}
	STDMETHOD(get_fontItalic)(VARIANT_BOOL* pVal)
	{
		*pVal = m_font.Italic ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_fontItalic)(VARIANT_BOOL newVal)
	{
		m_font.Italic = (newVal != VARIANT_FALSE);
		m_font.Update();
		return S_OK;
	}
	STDMETHOD(get_fontUnderline)(VARIANT_BOOL* pVal)
	{
		*pVal = m_font.Underline ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_fontUnderline)(VARIANT_BOOL newVal)
	{
		m_font.Underline = (newVal != VARIANT_FALSE);
		m_font.Update();
		return S_OK;
	}
	STDMETHOD(get_fontStrikeout)(VARIANT_BOOL* pVal)
	{
		*pVal = m_font.Strikeout ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_fontStrikeout)(VARIANT_BOOL newVal)
	{
		m_font.Strikeout = (newVal != VARIANT_FALSE);
		m_font.Update();
		return S_OK;
	}
	
	// formats
	STDMETHOD(get_formatAlignVertical)(long* pVal)
	{
		*pVal = m_format.StringAlignmentVertical;
		return S_OK;
	}
	STDMETHOD(put_formatAlignVertical)(long newVal)
	{
		m_format.StringAlignmentVertical = newVal;
		m_format.Update();
		return S_OK;
	}
	STDMETHOD(get_formatAlignHorizontal)(long* pVal)
	{
		*pVal = m_format.StringAlignmentHorizontal;
		return S_OK;
	}
	STDMETHOD(put_formatAlignHorizontal)(long newVal)
	{
		m_format.StringAlignmentHorizontal = newVal;
		m_format.Update();
		return S_OK;
	}
	STDMETHOD(get_formatRotate)(long* pVal)
	{
		*pVal = m_nRotateAngle;

		return S_OK;
	}
	STDMETHOD(put_formatRotate)(long newVal)
	{
		m_nRotateAngle = newVal;

		return S_OK;
	}
	
	// shadows
	STDMETHOD(get_shadowVisible)(VARIANT_BOOL* pVal)
	{ 
		*pVal = m_shadow.Visible ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK; 
	}
	STDMETHOD(put_shadowVisible)(VARIANT_BOOL newVal)
	{ 
		m_shadow.Visible = (newVal == VARIANT_TRUE); 
		return S_OK; 
	}
	STDMETHOD(get_shadowDistanceX)(long* pVal)
	{ 
		*pVal = m_shadow.DistanceX; 
		return S_OK; 
	}
	STDMETHOD(put_shadowDistanceX)(long newVal)
	{ 
		m_shadow.DistanceX = newVal; 
		return S_OK; 
	}
	STDMETHOD(get_shadowDistanceY)(long* pVal)
	{ 
		*pVal = m_shadow.DistanceY; 
		return S_OK; 
	}
	STDMETHOD(put_shadowDistanceY)(long newVal)
	{ 
		m_shadow.DistanceY = newVal; 
		return S_OK; 
	}
	STDMETHOD(get_shadowColor)(long* pVal)
	{ 
		*pVal = m_shadow.Color; 
		return S_OK; 
	}
	STDMETHOD(put_shadowColor)(long newVal)
	{
		m_shadow.Color = newVal;
		m_shadow.Update();
		return S_OK; 
	}
	STDMETHOD(get_shadowAlpha)(long* pVal)
	{ 
		*pVal = m_shadow.Alpha;
		return S_OK;
	}
	STDMETHOD(put_shadowAlpha)(long newVal)
	{
		m_shadow.Alpha = newVal;
		m_shadow.Update();
		return S_OK;
	}
	STDMETHOD(get_shadowBlur)(long* pVal)
	{ 
		*pVal = m_shadow.BlurSize; 
		return S_OK; 
	}
	STDMETHOD(put_shadowBlur)(long newVal)
	{ 
		m_shadow.BlurSize = newVal; 
		return S_OK; 
	}
	
	// edges
	STDMETHOD(get_edgeVisible)(long* pVal)
	{
		*pVal = m_edge.Visible;
		return S_OK; 
	}
	STDMETHOD(put_edgeVisible)(long newVal)
	{
		m_edge.Visible = newVal; 
		return S_OK; 
	}
	STDMETHOD(get_edgeDistance)(long* pVal)
	{ 
		*pVal = m_edge.Dist; 
		return S_OK; 
	}
	STDMETHOD(put_edgeDistance)(long newVal)
	{ 
		m_edge.Dist = newVal; 
		return S_OK; 
	}
	STDMETHOD(get_edgeColor)(long* pVal)
	{ 
		*pVal = m_edge.Color; 
		return S_OK; 
	}
	STDMETHOD(put_edgeColor)(long newVal)
	{
		m_edge.Color = newVal;
		m_edge.Update();
		return S_OK; 
	}
	STDMETHOD(get_edgeAlpha)(long* pVal)
	{ 
		*pVal = m_edge.Alpha;
		return S_OK;
	}
	STDMETHOD(put_edgeAlpha)(long newVal)
	{
		m_edge.Alpha = newVal;
		m_edge.Update();
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

		m_brush.Read(stream);
		m_font.Read(stream);
		m_format.Read(stream);
		m_shadow.Read(stream);
		m_edge.Read(stream);

		return S_OK;
	}
	STDMETHOD(GetFormat)(SAFEARRAY** Array)
	{
		long size = m_brush.GetSize() + m_font.GetSize() + m_format.GetSize() + m_shadow.GetSize() + m_edge.GetSize();

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

		m_brush.Write(stream);
		m_font.Write(stream);
		m_format.Write(stream);
		m_shadow.Write(stream);
		m_edge.Write(stream);

		return S_OK;
	}
};

