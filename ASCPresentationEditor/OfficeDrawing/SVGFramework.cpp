
#include "SVGFramework.h"

//#include "..\..\..\Interfaces\AVSGraphicsRenderer.h"
//#include "..\..\..\Interfaces\ImageSerializeObjects2.h"
//#include "..\..\AVSVideoStudio3\Common\MediaFormatDefine.h"

#define ADD_COLOR( COLOR, R, G, B ) m_Table.insert(std::pair<CString, unsigned long>( _T(COLOR), ( R << 16 ) | ( G << 8 ) | B ) );

namespace SVG
{
	ColorTable ColorParser::m_oTable;
	BOOL ColorTable::InitClrTable()
	{
		if (m_Table.size())
			return TRUE;

		ADD_COLOR("aliceblue", 240, 248, 255);
		ADD_COLOR("antiquewhite", 250, 235, 215); 
		ADD_COLOR("aqua",  0, 255, 255); 
		ADD_COLOR("aquamarine", 127, 255, 212); 
		ADD_COLOR("azure", 240, 255, 255); 
		ADD_COLOR("beige", 245, 245, 220); 
		ADD_COLOR("bisque", 255, 228, 196); 
		ADD_COLOR("black",  0, 0, 0); 
		ADD_COLOR("blanchedalmond", 255, 235, 205); 
		ADD_COLOR("blue",  0, 0, 255); 
		ADD_COLOR("blueviolet", 138, 43, 226); 
		ADD_COLOR("brown", 165, 42, 42); 
		ADD_COLOR("burlywood", 222, 184, 135); 
		ADD_COLOR("cadetblue",  95, 158, 160); 
		ADD_COLOR("chartreuse", 127, 255, 0); 
		ADD_COLOR("chocolate", 210, 105, 30); 
		ADD_COLOR("coral", 255, 127, 80); 
		ADD_COLOR("cornflowerblue", 100, 149, 237); 
		ADD_COLOR("cornsilk", 255, 248, 220); 
		ADD_COLOR("crimson", 220, 20, 60); 
		ADD_COLOR("cyan",  0, 255, 255); 
		ADD_COLOR("darkblue",  0, 0, 139); 
		ADD_COLOR("darkcyan",  0, 139, 139); 
		ADD_COLOR("darkgoldenrod", 184, 134, 11); 
		ADD_COLOR("darkgray", 169, 169, 169); 
		ADD_COLOR("darkgreen",  0, 100, 0); 
		ADD_COLOR("darkgrey", 169, 169, 169); 
		ADD_COLOR("darkkhaki", 189, 183, 107); 
		ADD_COLOR("darkmagenta", 139, 0, 139); 
		ADD_COLOR("darkolivegreen",  85, 107, 47); 
		ADD_COLOR("darkorange", 255, 140, 0); 
		ADD_COLOR("darkorchid", 153, 50, 204); 
		ADD_COLOR("darkred", 139, 0, 0); 
		ADD_COLOR("darksalmon", 233, 150, 122); 
		ADD_COLOR("darkseagreen", 143, 188, 143); 
		ADD_COLOR("darkslateblue",  72, 61, 139); 
		ADD_COLOR("darkslategray",  47, 79, 79); 
		ADD_COLOR("darkslategrey",  47, 79, 79); 
		ADD_COLOR("darkturquoise",  0, 206, 209); 
		ADD_COLOR("darkviolet", 148, 0, 211); 
		ADD_COLOR("deeppink", 255, 20, 147); 
		ADD_COLOR("deepskyblue",  0, 191, 255); 
		ADD_COLOR("dimgray", 105, 105, 105); 
		ADD_COLOR("dimgrey", 105, 105, 105); 
		ADD_COLOR("dodgerblue",  30, 144, 255); 
		ADD_COLOR("firebrick", 178, 34, 34); 
		ADD_COLOR("floralwhite", 255, 250, 240); 
		ADD_COLOR("forestgreen",  34, 139, 34); 
		ADD_COLOR("fuchsia", 255, 0, 255); 
		ADD_COLOR("gainsboro", 220, 220, 220); 
		ADD_COLOR("ghostwhite", 248, 248, 255); 
		ADD_COLOR("gold", 255, 215, 0); 
		ADD_COLOR("goldenrod", 218, 165, 32); 
		ADD_COLOR("gray", 128, 128, 128); 
		ADD_COLOR("grey", 128, 128, 128); 
		ADD_COLOR("green",  0, 128, 0); 
		ADD_COLOR("greenyellow", 173, 255, 47); 
		ADD_COLOR("honeydew", 240, 255, 240); 
		ADD_COLOR("hotpink", 255, 105, 180); 
		ADD_COLOR("indianred", 205, 92, 92); 
		ADD_COLOR("indigo",  75, 0, 130); 
		ADD_COLOR("ivory", 255, 255, 240); 
		ADD_COLOR("khaki", 240, 230, 140); 
		ADD_COLOR("lavender", 230, 230, 250); 
		ADD_COLOR("lavenderblush", 255, 240, 245); 
		ADD_COLOR("lawngreen", 124, 252, 0); 
		ADD_COLOR("lemonchiffon", 255, 250, 205); 
		ADD_COLOR("lightblue", 173, 216, 230); 
		ADD_COLOR("lightcoral", 240, 128, 128); 
		ADD_COLOR("lightcyan", 224, 255, 255); 
		ADD_COLOR("lightgoldenrodyellow", 250, 250, 210); 
		ADD_COLOR("lightgray", 211, 211, 211); 
		ADD_COLOR("lightgreen", 144, 238, 144); 
		ADD_COLOR("lightgrey", 211, 211, 211); 
		ADD_COLOR("lightpink", 255, 182, 193); 
		ADD_COLOR("lightsalmon", 255, 160, 122); 
		ADD_COLOR("lightseagreen",  32, 178, 170); 
		ADD_COLOR("lightskyblue", 135, 206, 250); 
		ADD_COLOR("lightslategray", 119, 136, 153); 
		ADD_COLOR("lightslategrey", 119, 136, 153); 
		ADD_COLOR("lightsteelblue", 176, 196, 222); 
		ADD_COLOR("lightyellow", 255, 255, 224); 
		ADD_COLOR("lime",  0, 255, 0); 
		ADD_COLOR("limegreen",  50, 205, 50); 
		ADD_COLOR("linen", 250, 240, 230); 
		ADD_COLOR("magenta", 255, 0, 255); 
		ADD_COLOR("maroon", 128, 0, 0); 
		ADD_COLOR("mediumaquamarine", 102, 205, 170); 
		ADD_COLOR("mediumblue",  0, 0, 205); 
		ADD_COLOR("mediumorchid", 186, 85, 211); 
		ADD_COLOR("mediumpurple", 147, 112, 219); 
		ADD_COLOR("mediumseagreen",  60, 179, 113); 
		ADD_COLOR("mediumslateblue", 123, 104, 238); 
		ADD_COLOR("mediumspringgreen",  0, 250, 154); 
		ADD_COLOR("mediumturquoise",  72, 209, 204); 
		ADD_COLOR("mediumvioletred", 199, 21, 133); 
		ADD_COLOR("midnightblue",  25, 25, 112); 
		ADD_COLOR("mintcream", 245, 255, 250); 
		ADD_COLOR("mistyrose", 255, 228, 225); 
		ADD_COLOR("moccasin", 255, 228, 181); 
		ADD_COLOR("navajowhite", 255, 222, 173); 
		ADD_COLOR("navy",  0, 0, 128); 
		ADD_COLOR("oldlace", 253, 245, 230); 
		ADD_COLOR("olive", 128, 128, 0); 
		ADD_COLOR("olivedrab", 107, 142, 35); 
		ADD_COLOR("orange", 255, 165, 0); 
		ADD_COLOR("orangered", 255, 69, 0); 
		ADD_COLOR("orchid", 218, 112, 214); 
		ADD_COLOR("palegoldenrod", 238, 232, 170); 
		ADD_COLOR("palegreen", 152, 251, 152); 
		ADD_COLOR("paleturquoise", 175, 238, 238); 
		ADD_COLOR("palevioletred", 219, 112, 147); 
		ADD_COLOR("papayawhip", 255, 239, 213); 
		ADD_COLOR("peachpuff", 255, 218, 185); 
		ADD_COLOR("peru", 205, 133, 63); 
		ADD_COLOR("pink", 255, 192, 203); 
		ADD_COLOR("plum", 221, 160, 221); 
		ADD_COLOR("powderblue", 176, 224, 230); 
		ADD_COLOR("purple", 128, 0, 128); 
		ADD_COLOR("red", 255, 0, 0); 
		ADD_COLOR("rosybrown", 188, 143, 143); 
		ADD_COLOR("royalblue",  65, 105, 225); 
		ADD_COLOR("paddlebrown", 139, 69, 19); 
		ADD_COLOR("palmon", 250, 128, 114); 
		ADD_COLOR("pandybrown", 244, 164, 96); 
		ADD_COLOR("peagreen",  46, 139, 87); 
		ADD_COLOR("peashell", 255, 245, 238); 
		ADD_COLOR("pienna", 160, 82, 45); 
		ADD_COLOR("pilver", 192, 192, 192); 
		ADD_COLOR("pkyblue", 135, 206, 235); 
		ADD_COLOR("plateblue", 106, 90, 205); 
		ADD_COLOR("plategray", 112, 128, 144); 
		ADD_COLOR("plategrey", 112, 128, 144); 
		ADD_COLOR("pnow", 255, 250, 250); 
		ADD_COLOR("ppringgreen",  0, 255, 127); 
		ADD_COLOR("pteelblue",  70, 130, 180); 
		ADD_COLOR("tan", 210, 180, 140); 
		ADD_COLOR("teal",  0, 128, 128); 
		ADD_COLOR("thistle", 216, 191, 216); 
		ADD_COLOR("tomato", 255, 99, 71); 
		ADD_COLOR("turquoise",  64, 224, 208); 
		ADD_COLOR("violet", 238, 130, 238); 
		ADD_COLOR("wheat", 245, 222, 179); 
		ADD_COLOR("white", 255, 255, 255); 
		ADD_COLOR("whitesmoke", 245, 245, 245); 
		ADD_COLOR("yellow", 255, 255, 0); 
		ADD_COLOR("yellowgreen", 154, 205, 50); 

		return TRUE;
	}

	long ColorParser::ColorFromString(const CString& sColor)
	{
		// HEX VALUE

		int index = sColor.Find(_T('#'));
		if (-1 != index)
		{
			CString OnlyColor = _T("");
			for ( int i = index; i < sColor.GetLength (); ++i)
			{
				if (_T(' ') == sColor[i])
				{
					return ColorFromHexString(sColor.Mid(index, i - index));
				}
			}

			return ColorFromHexString(sColor.Mid(index, sColor.GetLength() - index));
		}

		if (CString(_T("none")) == sColor)
			return -2;

		// 'rgb(x,x,x)' 
		if ((sColor[0] == _T('r')) && (sColor[1] == _T('g')) && (sColor[2] == _T('b')))
			return ColorFromRgbString(sColor);

		// COLOR TABLE
		return m_oTable.Find(sColor);
	}
	long ColorParser::ColorFromRgbString(const CString& Rgb)
	{
		BOOL PCT	=	FALSE;
		double Color [ 4 ]	=	{ 0.0, 0.0, 0.0, 255 };

		BOOL Begin	=	FALSE;	
		int	IndS	=	0;
		int IndCol	=	0;

		for ( int i = 0; i < (int)Rgb.GetLength (); ++i )
		{
			if ( isdigit ( Rgb [ i ] ) )
			{
				if ( FALSE == Begin )
				{
					Begin	=	TRUE;
					IndS	=	i;
				}
			}
			else
			{	
				if ( Begin )
				{
					if ( i - IndS >= 1 )
					{
						Color [ IndCol++ ]	=	_tstof ( Rgb.Mid ( IndS, i - IndS ) );
					}
				}

				Begin	=	FALSE;
			}

			if ( _T('%') == Rgb [ i ] )
			{
				PCT	=	TRUE;
			}
		}

		if ( PCT )
		{
			Color [ 0 ] *= 255.0 * 0.01;
			Color [ 1 ] *= 255.0 * 0.01;
			Color [ 2 ] *= 255.0 * 0.01;
		}

		if (0 == Color[3])
			return -2;

		return ( ( (int) (Color [ 2 ]) << 16 ) | ( (int) (Color [ 1 ]) << 8 ) | (int) (Color [ 0 ]) );			
	}
	long ColorParser::ColorFromHexString(const CString& Hex)
	{
		CString value	=	Hex.Mid ( 1, Hex.GetLength () - 1 );
		if ( 3 == value.GetLength () )	//	for ex ~ #fb0 expands to #ffbb00
		{
			value.Insert ( 0, value [ 0 ] );
			value.Insert ( 2, value [ 2 ] );
			value.Insert ( 4, value [ 4 ] );
		}

		for ( int i = 0; i < value.GetLength (); ++i )
		{
			if ( isdigit ( value [ i ] ) || ( value [ i ] >= L'a' ) || ( value [ i ] >= L'f' ) || ( value [ i ] >= L'A' ) || ( value [ i ] >= L'F' ) )
				continue;

			long InvCol = wcstol ( value.Mid ( 0, i ), NULL, 16 );
			return ( ( InvCol & 0xFF ) << 16 ) | ( ( InvCol & 0xFF00 ) ) | ( ( InvCol & 0xFF0000 ) >> 16 );
		}

		long InvCol = wcstol( value, NULL, 16 );
		return ( ( InvCol & 0xFF ) << 16 ) | ( ( InvCol & 0xFF00 ) ) | ( ( InvCol & 0xFF0000 ) >> 16 );
	}
}

namespace SVG
{
	//BOOL ImageBase64::CreateImage (BYTE* pBuffer, unsigned long Length, int ImageEncoder)
	//{
	//	if (NULL == pBuffer || Length < 1)
	//		return FALSE;

	//	m_nBufferSize	=	Base64::Base64DecodeGetRequiredLength (Length);
	//	m_pBuffer		=	new BYTE [m_nBufferSize];

	//	if (m_pBuffer)
	//	{
	//		if (ImageBase64::Base64Decode ( (LPTSTR) pBuffer, Length, m_pBuffer, &m_nBufferSize))
	//		{
	//			/*

	//			CAtlFile oFile;

	//			if ( ImageEncoder == JPG_ENCODER )
	//			{
	//			if ( SUCCEEDED ( oFile.Create ( _T("C:\\test.jpg"), GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_NEW ) ) )
	//			{
	//			DWORD nFileBuff	=	0L;
	//			oFile.Write ( m_pBuffer, m_nBufferSize, &nFileBuff );
	//			oFile.Close ();
	//			}
	//			}

	//			*/

	//			return TRUE;
	//		}

	//		delete [] m_pBuffer;
	//	}

	//	return FALSE;
	//}
	//BOOL ImageBase64::EncodeBufferToImageCOM (const BYTE* pBuffer, const unsigned long& ImageSize, IUnknown*& pImage)
	//{
	//	if ( NULL == pBuffer || ImageSize < 41 )
	//		return FALSE;

	//	HGLOBAL	hMem	=	::GlobalAlloc ( GMEM_FIXED, ImageSize );
	//	LPVOID	pSImage	=	::GlobalLock ( hMem );

	//	CopyMemory ( pSImage, pBuffer, ImageSize );

	//	IStream* pStream = NULL;
	//	if ( SUCCEEDED ( CreateStreamOnHGlobal ( hMem, FALSE, &pStream ) ) )
	//	{
	//		Gdiplus::Bitmap* pBitmap	= Gdiplus::Bitmap::FromStream ( pStream );
	//		if ( Gdiplus::Ok == pBitmap->GetLastStatus () )
	//		{
	//			BOOL Status	=	ImageBase64::GdiImageToImageCOM ( pBitmap, pImage );

	//			delete pBitmap;

	//			pStream->Release();

	//			GlobalUnlock ( hMem );
	//			GlobalFree ( hMem );

	//			return Status;
	//		}

	//		delete pBitmap;

	//		pStream->Release();
	//	}

	//	GlobalUnlock ( hMem );
	//	GlobalFree ( hMem );

	//	return FALSE;
	//}
//	BOOL ImageBase64::GdiImageToImageCOM (Gdiplus::Bitmap* pBitmap, IUnknown*& pImage)
//	{
//		MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
//		if ( SUCCEEDED ( CoCreateInstance ( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame ) ) )
//		{
//			pFrame->put_Height ( pBitmap->GetHeight () );
//			pFrame->put_Width ( pBitmap->GetWidth () );
//
//			pFrame->put_AspectRatioX ( 0 );
//			pFrame->put_AspectRatioY ( 0 );
//
//#define CSP_BGRA     (1<< 6) /* 32-bit bgra packed */
//
//			//pFrame->put_ColorSpace ( CSP_BGRA | CSP_VFLIP );
//			pFrame->put_ColorSpace	( CSP_BGRA );
//
//			pFrame->SetDefaultStrides ();
//			pFrame->AllocateBuffer	( -1 );
//
//			LONG PlaneSize = 0;		pFrame->get_BufferSize ( &PlaneSize );
//			BYTE* pPlane = NULL;	pFrame->get_Plane ( 0, &pPlane );
//
//			if ( NULL != pPlane )
//			{
//				// memset ( pPlane, 0xFFFFFF00, PlaneSize );
//
//				BitmapData data; 
//				if ( Gdiplus::Ok == pBitmap->LockBits ( NULL, ImageLockModeRead, PixelFormat32bppARGB, &data ) )
//				{
//					memcpy ( pPlane, data.Scan0, PlaneSize );
//					pBitmap->UnlockBits ( &data );
//				}
//
//				pFrame->QueryInterface ( __uuidof(IUnknown), (void**)&pImage );
//				pFrame->Release ();
//
//				return TRUE;
//			}
//
//			pFrame->Release ();
//		}
//
//		return FALSE;
//	}
	//BOOL ImageBase64::Base64Decode(LPTSTR szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen) throw()
	//{
	//	// walk the source buffer
	//	// each four character sequence is converted to 3 bytes
	//	// CRLFs and =, and any characters not in the encoding table
	//	// are skiped

	//	if (szSrc == NULL || pnDestLen == NULL)
	//		return FALSE;

	//	LPTSTR szSrcEnd = szSrc + nSrcLen;
	//	int nWritten = 0;

	//	BOOL bOverflow = (pbDest == NULL) ? TRUE : FALSE;

	//	while (szSrc < szSrcEnd &&(*szSrc) != 0)
	//	{
	//		DWORD dwCurr = 0;
	//		int i;
	//		int nBits = 0;
	//		for (i=0; i<4; i++)
	//		{
	//			if (szSrc >= szSrcEnd)
	//				break;
	//			int nCh = DecodeBase64Char(*szSrc);
	//			szSrc++;
	//			if (nCh == -1)
	//			{
	//				// skip this char
	//				i--;
	//				continue;
	//			}
	//			dwCurr <<= 6;
	//			dwCurr |= nCh;
	//			nBits += 6;
	//		}

	//		if(!bOverflow && nWritten + (nBits/8) > (*pnDestLen))
	//			bOverflow = TRUE;

	//		// dwCurr has the 3 bytes to write to the output buffer
	//		// left to right
	//		dwCurr <<= 24-nBits;
	//		for (i=0; i<nBits/8; i++)
	//		{
	//			if(!bOverflow)
	//			{
	//				*pbDest = (BYTE) ((dwCurr & 0x00ff0000) >> 16);
	//				pbDest++;
	//			}
	//			dwCurr <<= 8;
	//			nWritten++;
	//		}

	//	}

	//	*pnDestLen = nWritten;

	//	if(bOverflow)
	//	{
	//		// if (pbDest != NULL) ATLASSERT(FALSE);

	//		return FALSE;
	//	}

	//	return TRUE;
	//}
}

namespace SVG
{
	BOOL Painter::Draw(IRefStorage* model, IRenderer* render, const UnitSystem& oUs)
	{
		if (NULL == model || NULL == render)
			return FALSE;

		m_model		=	model;
		m_render	=	render;
		m_oUs		=	oUs;

		m_render->get_DpiX(&m_dpiX);
		m_render->get_DpiY(&m_dpiY);
		m_render->get_Width(&m_widthMM);
		m_render->get_Height(&m_heightMM);

		if ((fabs(m_dpiX) < 0.000001) || (fabs(m_dpiY) < 0.000001))
		{
			m_dpiX	=	m_oUs.GetWidth() / m_widthMM * 25.4;
			m_dpiY	=	m_oUs.GetHeight() / m_heightMM * 25.4;
		}

		m_dAddMX	=	25.4 / m_dpiX;
		m_dAddMY	=	25.4 / m_dpiY;

		m_model->Normalize(m_dAddMX, m_dAddMY);

//		VARIANT var;
//		if (SUCCEEDED(m_render->GetAdditionalParam(L"FontManager", &var)))
//		{
//			if (var.vt == VT_UNKNOWN && NULL == var.punkVal)
//			{
//				m_bEnableFonts = FALSE;					
//#ifndef _DEBUG
//				var.punkVal = GetFontManager();
//				m_render->SetAdditionalParam(L"FontManager", var);
//#endif
//			}
//		}

		return DrawStorage(m_model);
	}
	BOOL Painter::DrawLine (Line* element, const Style& oStyle, const CString& strClassName)
	{
		LONG type = 0;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)	
		{
			m_render->PathCommandStart();
			m_render->BeginCommand (c_nPathType);

			m_render->PathCommandStart ();

			m_render->PathCommandMoveTo (element->GetFrom().X, element->GetFrom().Y);
			m_render->PathCommandLineTo (element->GetTo().X, element->GetTo().Y);

			m_render->DrawPath (type);
			m_render->EndCommand (c_nPathType);
			m_render->PathCommandEnd ();
		}	

		return TRUE;
	}
	BOOL Painter::DrawRectangle (Rectangle* element, const Style& oStyle, const CString& strClassName)
	{
		DoClip (element->GetClip(), TRUE);	

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)
			DoRectangle (element, type);

		DoClip (element, FALSE);

		return TRUE;
	}
	BOOL Painter::DrawCircle (Circle* element,const Style& oStyle, const CString& strClassName)
	{
		DoClip (element->GetClip(), TRUE);			

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)	
			DoCircle (element, type);

		DoClip (element->GetClip(), FALSE);

		return TRUE;
	}
	BOOL Painter::DrawEllipse (Ellipse* element, const Style& oStyle, const CString& strClassName)
	{
		DoClip (element->GetClip(), TRUE);			

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)	
			DoEllipse (element, type);

		DoClip (element->GetClip(), FALSE);

		return TRUE;
	}
	BOOL Painter::DrawPolyline (Polyline* element, const Style& oStyle, const CString& strClassName)
	{
		DoClip (element->GetClip(), TRUE);			

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)	
		{
			DoPolyline (element, type);
		}

		DoClip (element->GetClip(), FALSE);

		return TRUE;
	}
	BOOL Painter::DrawPolygon (Polygon* element, const Style& oStyle, const CString& strClassName)
	{
		DoClip (element->GetClip(), TRUE);			

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)	
		{
			DoPolygon (element, type);
		}

		DoClip (element->GetClip(), FALSE);

		return TRUE;
	}		
	BOOL Painter::DrawPath (Path* element, const Style& oStyle, const CString& strClassName)
	{
		DoClip (element->GetClip(), TRUE);			

		LONG lType = 0;
		if (SetBrushStyle(oStyle, strClassName))
			lType += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			lType += c_nStroke;

		if (0 != lType)
			PushPathCommands (element, lType);

		DoClip (element->GetClip(), FALSE);

		return TRUE;
	}
	BOOL Painter::DrawText (Text* element, const Style& oStyle, const CString& strClassName)
	{
		if (!m_bEnableFonts)
			return FALSE;

		//IAVSFontManager* fontManager = GetFontManager();
		//if (NULL == fontManager)
		//	return FALSE;

		BOOL clipOn = DoClip (element->GetClip(), TRUE);			

		SetBrushStyle (oStyle, strClassName);

		double dpiX = 0.0f;	m_render->get_DpiX ( &dpiX );
		double dpiY = 0.0f;	m_render->get_DpiY ( &dpiY );

		if ( 0.0 == dpiX || 0.0 == dpiY )
		{
			double dWidthMM = 0; m_render->get_Width(&dWidthMM);
			double dHeightMM = 0; m_render->get_Height(&dHeightMM);

			double dWidthPix = m_oUs.GetWidth();
			double dHeightPix = m_oUs.GetHeight();

			dpiX = dWidthPix / dWidthMM * 25.4;
			dpiY = dHeightPix / dHeightMM * 25.4;
		}

		double dFontSize	=	element->GetFontStyle ().DoubleAttribute ( FontSize ) * 72.0f / dpiX * element->m_oUs.GetFactorFontSize ();
		if (dFontSize <= 0.0)	//	for geicha_6000px.svg 
		{
			if (clipOn)
				DoClip (element->GetClip(), FALSE);

			return FALSE;
		}

        std::wstring bsFontFamily	=	string2std_string( element->GetFontStyle ().GetAttribute ( FontFamily ) );
        std::wstring bsText			=	string2std_string( element->GetText () );

		float X			=	0.0;
		float Y			=	0.0;
		float BoundX	=	0.0;
		float BoundY	=	0.0;

		m_render->put_FontSize ( dFontSize );
		m_render->put_FontName ( bsFontFamily );

		LONG Style	= 0L;
		if ( FontWeightBold == element->GetFontStyle ().LongAttribute ( FontWeight ) )
		{
			Style |= 0x01;
		}

		if ( FontStyleItalic == element->GetFontStyle ().LongAttribute ( FontStyleAttr ) )
		{
			Style |= 0x02;
		}

		m_render->put_FontStyle (Style);

		//fontManager->LoadFontByName ( bsFontFamily, (float)dFontSize, 0, 72.0, 72.0 );
		//fontManager->LoadString2 ( bsText, 0, 0 );
		//fontManager->MeasureString ( &X, &Y, &BoundX, &BoundY );

		BoundX *= (25.4f / 72.0f);
		BoundY *= (25.4f / 72.0f);

		float m_fLineSpacing=0;
		float m_fEmHeight=0;
		float m_fAscent=0;
		float m_fDescent=0;

		unsigned short iTemp = 0;
//todooo проверить правильность выставления
		m_fAscent		= GetFontManager()->m_lAscender;		
		m_fDescent		= GetFontManager()->m_lDescender;
		m_fLineSpacing	= GetFontManager()->m_lLineHeight;
		m_fEmHeight		= GetFontManager()->m_lUnits_Per_Em;

		double OffSetY = 3 * (m_fLineSpacing - m_fDescent) - m_fAscent;
		OffSetY /= 2.0;
		OffSetY *= (dFontSize / m_fEmHeight);
		//OffSetY *= dpiY / 72.0;
		OffSetY *= (25.4 / 72.0);

		if ( FontTextAnchorStart == element->GetFontStyle().LongAttribute(FontTextAnchor))
		{
			m_render->CommandDrawText ( bsText, element->m_Pos.X, element->m_Pos.Y, BoundX, 0.0, OffSetY ); 
		}

		if ( FontTextAnchorMiddle == element->GetFontStyle().LongAttribute(FontTextAnchor))
		{
			m_render->CommandDrawText ( bsText, element->m_Pos.X - BoundX * 0.5, element->m_Pos.Y, BoundX, 0.0, OffSetY );
		}

		if (FontTextAnchorEnd == element->GetFontStyle ().LongAttribute(FontTextAnchor))
		{
			m_render->CommandDrawText ( bsText, element->m_Pos.X - BoundX, element->m_Pos.Y, BoundX, 0.0, OffSetY ); 
		}

		DoClip (element->GetClip(), FALSE);

		return TRUE;
	}
	//BOOL Painter::DrawImage (Image* element, const Style& oStyle, const CString& strClassName)
	//{
	//	if (element->GetXLink().GetLength())
	//	{	
	//		DrawImageFromFile (element, element->LivePath(m_sWorkingDirectory));					
	//	}
	//	else if (element->m_ImagePath.GetLength())
	//	{
	//		DrawImageFromFile (element);			
	//	}
	//	else
	//	{
	//		// TODO : save images

	//		IUnknown* pImage = NULL;
	//		if (element->m_bin64Image.GetImage(pImage))
	//		{
	//			m_render->DrawImage(pImage, element->m_Or.X, element->m_Or.Y, element->m_Size.X, element->m_Size.Y);
	//			RELEASEINTERFACE(pImage);
	//		}
	//	}

	//	return TRUE;
	//}
	BOOL Painter::DrawUse (Use* element, const Style& oStyle, const CString& strClassName)
	{
		BOOL retVal = FALSE;

		element->RefreshXLinkSource (m_model);

		if (element->GetRefLink())
		{
			ENodeType code = element->GetRefLink ()->nodeType();
			ISvgRef* refLink = element->GetRefLink();

			if (EGraphicsContainer == code)
			{
				IRefStorage* pStorage = dynamic_cast<IRefStorage*>(refLink);
				if (pStorage) 
				{
					retVal = DrawStorage (pStorage, element->GetTransform(), element->GetFrom());
				}
			}
			else if (ESymbol == code)
			{
				Symbol* pSymbol = dynamic_cast<Symbol*>(refLink);
				if (pSymbol)
				{
					BOOL bStatus = FALSE;
					for (long i = 0; i < pSymbol->GetCount(); ++i)
					{
						DrawElement* pContent = pSymbol->GetAt(i);
						if (pContent)
						{
							retVal = DrawInternal (pContent, element->GetTransform(), Point(), element->GetStyle());
						}
					}
				}
			}
			else
			{
				DrawElement* drawElement = dynamic_cast<DrawElement*>(refLink);
				if (drawElement)
				{					
					m_transforms.Push(element->GetTransform(), FALSE);

					Matrix local = m_transforms.GetFinal();
					retVal = DrawInternal (drawElement, local, element->GetFrom(), element->GetStyle());

					m_transforms.Pop();
				}
			}
		}

		return retVal;
	}

	//
	BOOL Painter::DrawStorage (IRefStorage* pStorage, const Matrix& parentTransform, const Point& off)
	{
		if (pStorage)
		{
			int count = pStorage->GetSize();
			if (0 == count)
				return FALSE;

			m_transforms.Push(parentTransform.Copy().Move(off.X, off.Y), FALSE);
			for (int i = 0; i < count; ++i)
			{
				DrawElement* pE = static_cast<DrawElement*>(pStorage->Get(i));
				if (pE)
				{	
					const ENodeType code	=	pE->nodeType();
					if (EClipPath == code)
						continue;

					const CString& css		=	pE->ClassName();
					Style oStyle			=	pE->GetStyle();						
					oStyle					=	ComposeStyles(pE, oStyle);

					Matrix mat = parentTransform;						
					mat *= Matrix::TranslateTransform(off.X, off.Y);
					mat *= pE->GetTransform();

					m_render->SetTransform(mat[0], mat[3], mat[1], mat[4], (mat[2] + m_oUs.OffSetX()) * m_dAddMX, (mat[5] + m_oUs.OffSetY()) * m_dAddMY);

					UpdateClass(pE);

					switch (code)
					{
					case ERectangle:			DrawRectangle(static_cast<Rectangle*>(pE),oStyle,css);								continue;	break;
					case ELine:					DrawLine(static_cast<Line*>(pE),oStyle,css);										continue;	break;
					case EEllipse:				DrawEllipse(static_cast<Ellipse*>(pE),oStyle,css);									continue;	break;
					case ECircle:				DrawCircle(static_cast<Circle*>(pE),oStyle,css);									continue;	break;
					case EPath:					DrawPath(static_cast<Path*>(pE),oStyle,css);										continue;	break;
					case EPolyline:				DrawPolyline(static_cast<Polyline*>(pE),oStyle,css);								continue;	break;
					case EPolygon:				DrawPolygon(static_cast<Polygon*>(pE),oStyle,css);									continue;	break;
					case EText:					DrawText(static_cast<Text*>(pE),oStyle,css);										continue;	break;
					//case EImage:				DrawImage(static_cast<Image*>(pE),oStyle,css);										continue;	break;
					case EUse:					DrawUse(static_cast<Use*>(pE),oStyle,css);											continue;	break;
					case EGraphicsContainer:	DrawGraphicsContainer(static_cast<GraphicsContainer*>(pE), parentTransform, off);	continue;	break;
					}
				}
			}

			m_transforms.Pop();
		}

		return TRUE;
	}
	BOOL Painter::DrawGraphicsContainer (GraphicsContainer* element, const Matrix& parentTransform, const Point& off)
	{
		DoClip (element->GetClip(), TRUE);			

		BOOL ret = DrawStorage (element, parentTransform, off);

		DoClip (element->GetClip(), FALSE);

		return ret;
	}
	BOOL Painter::DrawInternal (DrawElement* pE, const Matrix& parentTransform, const Point& off, const Style& oMainStyle)
	{
		Matrix mat = parentTransform;						
		mat *=	Matrix::TranslateTransform(off.X, off.Y);
		mat *=	pE->GetTransform();

		m_render->SetTransform (mat[0],	mat[3], mat[1], mat[4], (mat[2] + m_oUs.OffSetX()) * m_dAddMX,	(mat[5] + m_oUs.OffSetY()) * m_dAddMY);

		UpdateClass(pE);

		Style oStyle	=	pE->GetStyle();
		Style::UpdateValidateAttributes (oMainStyle, oStyle);
		oStyle			=	ComposeStyles(pE, oStyle);

		const CString& strClass = pE->ClassName();
		ENodeType code	=	pE->nodeType();

		if (ERectangle == code)
		{
			DrawRectangle(static_cast<Rectangle*>(pE),oStyle,strClass);			return TRUE;
		}
		else if (ELine == code)
		{
			DrawLine(static_cast<Line*>(pE),oStyle,strClass);					return TRUE;
		}
		else if (EEllipse == code)
		{
			DrawEllipse(static_cast<Ellipse*>(pE),oStyle,strClass);				return TRUE;
		}
		else if (ECircle == code)
		{
			DrawCircle(static_cast<Circle*>(pE),oStyle,strClass);				return TRUE;
		}
		else if (EPath == code)
		{
			DrawPath(static_cast<Path*>(pE),oStyle,strClass);					return TRUE;
		}
		else if (EPolyline == code)
		{
			DrawPolyline(static_cast<Polyline*>(pE),oStyle,strClass);			return TRUE;
		}
		else if (EPolygon == code)
		{
			DrawPolygon(static_cast<Polygon*>(pE),oStyle,strClass);				return TRUE;
		}
		else if (EText == code)
		{
			DrawText(static_cast<Text*>(pE),oStyle,strClass);					return TRUE;
		}
		else if (EImage == code)
		{
			DrawUse(static_cast<Use*>(pE),oStyle,strClass);						return TRUE;
		}
		else if (EUse == code)
		{
			DrawUse(static_cast<Use*>(pE),oStyle,strClass);						return TRUE;
		}

		return FALSE;
	}

	BOOL Painter::PushPathCommands (Path* element, long PathType)
	{
		m_render->PathCommandStart();
		m_render->BeginCommand ( c_nPathType );

		BOOL SegmentEnd	=	TRUE;
		Point lastMove;

		Point LastPoint;
		PathParser& oPath	=	element->GetPath ();
		for (long i = 0; i < (long)element->GetSize (); ++i)
		{
#ifdef _DEBUG
			WCHAR Code = element->GetCode(i);
#endif
			if (_T('Z') == element->GetCode(i) || _T('z') == element->GetCode(i))
			{
				m_render->PathCommandClose();

				SegmentEnd	=	TRUE;
			}
			else if (_T('m') == element->GetCode(i))
			{
				if (0 == i)
				{					
					m_render->PathCommandMoveTo(element->GetX(i), element->GetY(i));
					LastPoint	=	element->Get(i);

					lastMove	=	LastPoint;
				}
				else
				{
					LastPoint.X	=	lastMove.X;
					LastPoint.Y	=	lastMove.Y;

					m_render->PathCommandMoveTo(element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y);
					LastPoint	+=	element->Get(i);

					lastMove	=	LastPoint;
				}
			}
			else if (_T('M') == element->GetCode(i))
			{
				m_render->PathCommandMoveTo(element->GetX(i), element->GetY(i));
				LastPoint	=	element->Get(i);

				lastMove	=	LastPoint;
			}
			// lines
			else if (_T('l') == element->GetCode(i))
			{
				m_render->PathCommandLineTo(element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y);
				LastPoint	+=	element->Get(i);
			}
			else if (_T('L') == element->GetCode(i))
			{
				m_render->PathCommandLineTo(element->GetX(i), element->GetY(i));
				LastPoint	=	element->Get(i);
			}
			// Horizontal
			else if (_T('h') == element->GetCode(i))
			{
				m_render->PathCommandLineTo(element->GetX(i) + LastPoint.X, LastPoint.Y);
				LastPoint.X	+=	element->Get(i).X;
			}
			else if (_T('H') == element->GetCode(i))
			{
				m_render->PathCommandLineTo(element->GetX(i), LastPoint.Y);
				LastPoint.X	=	element->Get(i).X;
			}
			// Vertical
			else if (_T('v') == element->GetCode(i))
			{
				m_render->PathCommandLineTo(LastPoint.X, element->GetY(i) + LastPoint.Y);
				LastPoint.Y	+=	element->Get(i).Y;
			}
			else if (_T('V') == element->GetCode(i))
			{
				m_render->PathCommandLineTo(LastPoint.X, element->GetY(i));
				LastPoint.Y	=	element->Get(i).Y;
			}
			// elliptical arc curve
			else if (_T('a') == element->GetCode(i))
			{
				int	LargeFlag		=	static_cast<int> ( element->GetY ( i + 1 ) );
				int	SweepFlag		=	static_cast<int> ( element->GetX ( i + 2 ) );					

				double RX			=	element->GetX ( i );
				double RY			=	element->GetY ( i );

				Point End			=	LastPoint + element->Get ( i + 3 ); 

				Point Center		=	GetCenter ( LargeFlag, SweepFlag, Point ( element->GetX ( i ), element->GetY ( i ) ), LastPoint, End );

				double dStartAngle	=	GetAngle ( Center.X, Center.Y, LastPoint.X, LastPoint.Y );
				double dEndAngle	=	GetAngle ( Center.X, Center.Y, End.X, End.Y ); 

				double dSweep		=	0.0;

				if ( GetArcAngles ( LargeFlag, SweepFlag, dStartAngle, dEndAngle, dSweep ) )
				{
					RenderArcAppx (element->GetX ( i + 1 ), Center, Point ( element->GetX ( i ), element->GetY ( i ) ), dStartAngle, dSweep);

					LastPoint	+=	element->Get ( i + 3 );

					i += 3;
				}
			}
			else if ( _T('A') == element->GetCode (i) )
			{
				int	LargeFlag		=	static_cast<int> ( element->GetY ( i + 1 ) );
				int	SweepFlag		=	static_cast<int> ( element->GetX ( i + 2 ) );					

				double RX			=	element->GetX ( i );
				double RY			=	element->GetY ( i );

				Point End			=	element->Get ( i + 3 ); 

				Point Center		=	GetCenter ( LargeFlag, SweepFlag, Point ( element->GetX ( i ), element->GetY ( i ) ), LastPoint, End );

				double dStartAngle	=	GetAngle ( Center.X, Center.Y, LastPoint.X, LastPoint.Y );
				double dEndAngle	=	GetAngle ( Center.X, Center.Y, End.X, End.Y ); 

				double dSweep	=	0.0;

				if ( GetArcAngles ( LargeFlag, SweepFlag, dStartAngle, dEndAngle, dSweep ) )
				{					
					RenderArcAppx (element->GetX ( i + 1 ), Center, Point ( element->GetX ( i ), element->GetY ( i ) ), dStartAngle, dSweep);
				}

				LastPoint	=	element->Get ( i + 3 );

				i += 3;
			}
			// cubic bezier
			else  if ( _T('C') == element->GetCode (i) )
			{
				m_render->PathCommandCurveTo ( element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1), 
					element->GetX(i+2), element->GetY(i+2) );

				LastPoint	=	element->Get ( i + 2 );

				++i;
				++i;
			}
			else if ( _T('c') == element->GetCode (i) )
			{
				m_render->PathCommandCurveTo ( element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
					element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y,
					element->GetX(i+2) + LastPoint.X, element->GetY(i+2) + LastPoint.Y );

				LastPoint	+=	element->Get(i+2);

				++i;
				++i;
			}
			else if ( _T('S') == element->GetCode (i) )
			{
				if ( _T('C') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( _T('c') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( _T('S') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( _T('s') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else
				{
					m_render->PathCommandLineTo ( element->GetX(i+1), element->GetY(i+1) );
				}

				LastPoint	=	element->Get(i+1);							
				++i;
			}
			else if ( _T('s') == element->GetCode (i) )
			{
				if ( _T('C') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( _T('c') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( _T('S') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( _T('s') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if (_T('m') == element->GetCode(i - 1) 
					|| _T('l') == element->GetCode(i - 1)
					|| _T('v') == element->GetCode(i - 1)
					||_T('h') == element->GetCode(i - 1))
				{
					m_render->PathCommandCurveTo(LastPoint.X, LastPoint.Y,
						element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
						element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y);
				}
				else
				{
					m_render->PathCommandLineTo(element->GetX(i+1), element->GetY(i+1));
				}

				LastPoint	+=	element->Get(i+1);							
				++i;
			}
			// quadratic bezier
			else if ( _T('q') == element->GetCode(i))
			{
				m_render->PathCommandCurveTo ( element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
					element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y,
					element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );

				LastPoint	+=	element->Get(i+1);

				++i;
			}
			else if ( _T('Q') == element->GetCode (i) )
			{
				m_render->PathCommandCurveTo ( element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1), element->GetX(i+1), element->GetY(i+1) );

				LastPoint	=	element->Get(i+1);

				++i;
			}
			else if ( _T('t') == element->GetCode (i) )
			{
				Point p1 = oPath.GetPoints()[ i - 1 ].oPoint;
				Point p2 = oPath.GetPoints()[ i - 2 ].oPoint;
				Point p3 = oPath.GetPoints()[ i	+ 0	].oPoint;

				if ( _T('Q') == element->GetCode ( i - 1 ) )
				{
					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					m_render->PathCommandCurveTo ( dX, dY, p3.X + LastPoint.X, p3.Y + LastPoint.Y, p3.X + LastPoint.X, p3.Y + LastPoint.Y );
				}
				else if ( _T('q') == element->GetCode ( i - 1 ) )
				{
					p1 = Point ( LastPoint.X, LastPoint.Y );
					p2 = Point ( p1.X - element->GetX(i-1) + element->GetX(i-2), p1.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					m_render->PathCommandCurveTo ( dX, dY, p3.X + LastPoint.X, p3.Y + LastPoint.Y, p3.X + LastPoint.X, p3.Y + LastPoint.Y );
				}
				else
				{
					m_render->PathCommandLineTo ( element->GetX(i), element->GetY(i) );
				}

				LastPoint	+=	element->Get(i);							
			}
			else if ( _T('T') == element->GetCode (i) )
			{
				Point p1 = oPath.GetPoints()[ i - 1 ].oPoint;
				Point p2 = oPath.GetPoints()[ i - 2 ].oPoint;
				Point p3 = oPath.GetPoints()[ i + 0 ].oPoint;

				if ( _T('Q') == element->GetCode ( i - 1 ) )
				{
					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					m_render->PathCommandCurveTo ( dX, dY, p3.X, p3.Y, p3.X, p3.Y );
				}
				else if ( _T('q') == element->GetCode ( i - 1 ) )
				{
					p1 = Point ( LastPoint.X, LastPoint.Y );
					p2 = Point ( p1.X - element->GetX(i-1) + element->GetX(i-2), p1.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					m_render->PathCommandCurveTo ( dX, dY, p3.X, p3.Y, p3.X, p3.Y );
				}
				else
				{
					m_render->PathCommandLineTo ( element->GetX(i), element->GetY(i) );
				}

				LastPoint	+=	element->Get(i);							
			}
		}

		m_render->DrawPath(PathType);

		m_render->EndCommand(c_nPathType);
		m_render->PathCommandEnd();

		return TRUE;
	}
	BOOL Painter::ClipPathCommands (Path* element, long PathType)
	{
		Aggplus::CGraphicsPathSimpleConverter simplifier;
		simplifier.SetRenderer(m_render);

		BOOL SegmentEnd	=	TRUE;
		Point lastMove;

		Point LastPoint;
		PathParser& oPath = element->GetPath ();

		for (long i = 0; i < (long)element->GetSize (); ++i)
		{
#ifdef _DEBUG
			WCHAR Code = element->GetCode (i);
#endif
			if ( _T('Z') == element->GetCode (i) || _T('z') == element->GetCode (i) )
			{
				simplifier.PathCommandClose ();

				SegmentEnd	=	TRUE;
			}
			else if ( _T('m') == element->GetCode (i) )
			{
				if (0 == i)
				{					
					simplifier.PathCommandMoveTo(element->GetX(i), element->GetY(i));
					LastPoint	=	element->Get(i);

					lastMove	=	LastPoint;
				}
				else
				{
					LastPoint.X	=	lastMove.X;
					LastPoint.Y	=	lastMove.Y;

					simplifier.PathCommandMoveTo(element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y);
					LastPoint	+=	element->Get(i);

					lastMove	=	LastPoint;
				}
			}
			else if ( _T('M') == element->GetCode (i) )
			{
				simplifier.PathCommandMoveTo ( element->GetX(i), element->GetY(i) );

				LastPoint	=	element->Get(i);

				lastMove	=	LastPoint;
			}
			// lines
			else if ( _T('l') == element->GetCode (i) )
			{
				double X = element->GetX(i);
				double Y = element->GetY(i);

				simplifier.PathCommandLineTo ( element->GetX(i) +  LastPoint.X, element->GetY(i) + LastPoint.Y );

				LastPoint	+=	element->Get(i);
			}
			else if ( _T('L') == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( element->GetX(i), element->GetY(i) );

				LastPoint	=	element->Get(i);
			}
			// Horizontal
			else if ( _T('h') == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( element->GetX(i) + LastPoint.X, LastPoint.Y );

				LastPoint.X	+=	element->Get(i).X;
			}
			else if ( _T('H') == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( element->GetX(i), LastPoint.Y );

				LastPoint.X	=	element->Get(i).X;
			}
			// Vertical
			else if ( _T('v') == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( LastPoint.X, element->GetY(i) + LastPoint.Y );

				LastPoint.Y	+=	element->Get(i).Y;
			}
			else if ( _T('V') == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( LastPoint.X, element->GetY(i) );

				LastPoint.Y	=	element->Get(i).Y;
			}
			// elliptical arc curve
			else if ( _T('a') == element->GetCode (i) )
			{
				int	LargeFlag		=	static_cast<int> ( element->GetY ( i + 1 ) );
				int	SweepFlag		=	static_cast<int> ( element->GetX ( i + 2 ) );					

				double RX			=	element->GetX ( i );
				double RY			=	element->GetY ( i );

				Point End			=	LastPoint + element->Get ( i + 3 ); 

				Point Center		=	GetCenter ( LargeFlag, SweepFlag, Point ( element->GetX ( i ), element->GetY ( i ) ), LastPoint, End );

				double dStartAngle	=	GetAngle ( Center.X, Center.Y, LastPoint.X, LastPoint.Y );
				double dEndAngle	=	GetAngle ( Center.X, Center.Y, End.X, End.Y ); 

				double dSweep		=	0.0;

				if ( GetArcAngles ( LargeFlag, SweepFlag, dStartAngle, dEndAngle, dSweep ) )
				{
					ClipArcAppx (simplifier, element->GetX ( i + 1 ), Center, Point ( element->GetX ( i ), element->GetY ( i ) ), dStartAngle, dSweep);

					LastPoint	+=	element->Get ( i + 3 );

					i += 3;
				}
			}
			else if ( _T('A') == element->GetCode (i) )
			{
				int	LargeFlag		=	static_cast<int> ( element->GetY ( i + 1 ) );
				int	SweepFlag		=	static_cast<int> ( element->GetX ( i + 2 ) );					

				double RX			=	element->GetX ( i );
				double RY			=	element->GetY ( i );

				Point End			=	element->Get ( i + 3 ); 

				Point Center		=	GetCenter ( LargeFlag, SweepFlag, Point ( element->GetX ( i ), element->GetY ( i ) ), LastPoint, End );

				double dStartAngle	=	GetAngle ( Center.X, Center.Y, LastPoint.X, LastPoint.Y );
				double dEndAngle	=	GetAngle ( Center.X, Center.Y, End.X, End.Y ); 

				double dSweep	=	0.0;

				if ( GetArcAngles ( LargeFlag, SweepFlag, dStartAngle, dEndAngle, dSweep ) )
				{					
					ClipArcAppx (simplifier, element->GetX ( i + 1 ), Center, Point ( element->GetX ( i ), element->GetY ( i ) ), dStartAngle, dSweep);
				}

				LastPoint	=	element->Get ( i + 3 );

				i += 3;
			}
			// cubic bezier
			else if ( _T('C') == element->GetCode (i) )
			{
				simplifier.PathCommandCurveTo ( element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1), 
					element->GetX(i+2), element->GetY(i+2) );

				LastPoint	=	element->Get ( i + 2 );

				++i;
				++i;
			}
			else if ( _T('c') == element->GetCode (i) )
			{
				simplifier.PathCommandCurveTo ( element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
					element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y,
					element->GetX(i+2) + LastPoint.X, element->GetY(i+2) + LastPoint.Y );

				LastPoint	+=	element->Get(i+2);

				++i;
				++i;
			}
			else if ( _T('S') == element->GetCode (i) )
			{
				if ( _T('C') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( _T('c') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( _T('S') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( _T('s') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else
				{
					simplifier.PathCommandLineTo ( element->GetX(i+1), element->GetY(i+1) );
				}

				LastPoint	=	element->Get(i+1);							
				++i;
			}
			else if ( _T('s') == element->GetCode (i) )
			{
				if ( _T('C') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( _T('c') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( _T('S') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( _T('s') == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if (_T('m') == element->GetCode(i - 1) 
					|| _T('l') == element->GetCode(i - 1)
					|| _T('v') == element->GetCode(i - 1)
					||_T('h') == element->GetCode(i - 1))
				{
					simplifier.PathCommandCurveTo(LastPoint.X, LastPoint.Y,
						element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
						element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y);
				}
				else
				{
					simplifier.PathCommandLineTo(element->GetX(i+1), element->GetY(i+1));
				}

				LastPoint	+=	element->Get(i+1);							
				++i;
			}
			// quadratic bezier
			else if ( _T('q') == element->GetCode (i) )
			{
				simplifier.PathCommandCurveTo ( element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
					element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y,
					element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );

				LastPoint	+=	element->Get(i+1);

				++i;
			}
			else if ( _T('Q') == element->GetCode (i) )
			{
				simplifier.PathCommandCurveTo ( element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1), element->GetX(i+1), element->GetY(i+1) );

				LastPoint	=	element->Get(i+1);

				++i;
			}
			else if ( _T('t') == element->GetCode(i))
			{
				Point p1 = oPath.GetPoints()[ i - 1 ].oPoint;
				Point p2 = oPath.GetPoints()[ i - 2 ].oPoint;
				Point p3 = oPath.GetPoints()[ i	+ 0	].oPoint;

				if ( _T('Q') == element->GetCode ( i - 1 ) )
				{
					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					simplifier.PathCommandCurveTo ( dX, dY, p3.X + LastPoint.X, p3.Y + LastPoint.Y, p3.X + LastPoint.X, p3.Y + LastPoint.Y );
				}
				else if ( _T('q') == element->GetCode ( i - 1 ) )
				{
					p1 = Point ( LastPoint.X, LastPoint.Y );
					p2 = Point ( p1.X - element->GetX(i-1) + element->GetX(i-2), p1.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					simplifier.PathCommandCurveTo ( dX, dY, p3.X + LastPoint.X, p3.Y + LastPoint.Y, p3.X + LastPoint.X, p3.Y + LastPoint.Y );
				}
				else
				{
					simplifier.PathCommandLineTo ( element->GetX(i), element->GetY(i) );
				}

				LastPoint	+=	element->Get(i);							
			}
			else if (_T('T') == element->GetCode(i))
			{
				Point p1 = oPath.GetPoints()[ i - 1 ].oPoint;
				Point p2 = oPath.GetPoints()[ i - 2 ].oPoint;
				Point p3 = oPath.GetPoints()[ i + 0 ].oPoint;

				if ( _T('Q') == element->GetCode ( i - 1 ) )
				{
					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					simplifier.PathCommandCurveTo ( dX, dY, p3.X, p3.Y, p3.X, p3.Y );
				}
				else if ( _T('q') == element->GetCode ( i - 1 ) )
				{
					p1 = Point ( LastPoint.X, LastPoint.Y );
					p2 = Point ( p1.X - element->GetX(i-1) + element->GetX(i-2), p1.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					simplifier.PathCommandCurveTo ( dX, dY, p3.X, p3.Y, p3.X, p3.Y );
				}
				else
				{
					simplifier.PathCommandLineTo ( element->GetX(i), element->GetY(i) );
				}

				LastPoint	+=	element->Get(i);							
			}
		}

		return TRUE;
	}

	BOOL Painter::SetBrushStyle (const Style& style, const CString& strClassName)
	{
		double alpha	=	style.DoubleAttribute(Opacity);

		m_render->put_BrushType(c_BrushTypeSolid);

		ISvgRef* pFill	=	style.GetFill();
		if (pFill)
		{
			if (ELinearGradient == pFill->nodeType())
			{
				LinearGradient* gradient = static_cast<LinearGradient*>(pFill);
				if (gradient)
				{
					if (gradient->GetColors().Count())
					{
						m_render->put_BrushColor1(gradient->GetColors().Get(0).m_nColor);
						m_render->put_BrushAlpha1((long)(gradient->GetColors().Get(0).m_fOpacity * 255.0 * alpha));

//						VARIANT oVar;
//						oVar.vt			=	VT_BSTR;
//						oVar.bstrVal	=	CComBSTR(gradient->GetXml());
						//m_render->SetAdditionalParam(L"Fill-LinearGradient", oVar);
					}
				}
			}
			else if (ERadialGradient == pFill->nodeType())
			{
				RadialGradient* gradient = static_cast<RadialGradient*>(pFill);
				if (gradient)
				{
					if (gradient->GetColors().Count())
					{
						m_render->put_BrushColor1(gradient->GetColors().Get(0).m_nColor);
						m_render->put_BrushAlpha1((long)(gradient->GetColors().Get(0).m_fOpacity * 255.0 * alpha));

//						VARIANT oVar;
//						oVar.vt			=	VT_BSTR;
//						oVar.bstrVal	=	CComBSTR(gradient->GetXml());
//						//m_render->SetAdditionalParam(L"Fill-RadialGradient", oVar);
					}
				}
			}
			//else if (EPattern == pFill->nodeType())
			//{
			//	DoPattern (static_cast<Pattern*>(pFill));
			//}

			return TRUE;
		}

		long lLongAttribute =	style.LongAttribute(FillColor);
		double dAlplaFill	=	style.DoubleAttribute(FillOpacity);

		if (-3 == lLongAttribute)
		{
			int nFound = strClassName.Find(_T("fill"));

			LONG lRendererType = 0;
			m_render->get_Type(&lRendererType);
			if ((c_nSVGConverter != lRendererType) || (nFound == -1))
			{
				m_render->put_BrushColor1 ( 0 );
			}
			m_render->put_BrushAlpha1 ( (LONG)( dAlplaFill * 255.0 * alpha) );
			return TRUE;
		}

		if (-2 == lLongAttribute)
			return FALSE;

		if (dAlplaFill > 0.0 && (-1 != lLongAttribute) && (alpha > 0.0))
		{
			if (!style.ValidAttribute(FillColor))
			{
				LONG lRendererType = 0;
				m_render->get_Type(&lRendererType);
				if (c_nSVGConverter == lRendererType)
				{
					int nFound = strClassName.Find(_T("fill"));
					
					if (-1 == nFound)
						m_render->put_BrushColor1 ( 0 );					

					m_render->put_BrushAlpha1 ( (LONG)( dAlplaFill * 255.0 * alpha) );
					return TRUE;
				}
			}

			m_render->put_BrushColor1 ( lLongAttribute );
			m_render->put_BrushAlpha1 ( (LONG)( dAlplaFill * 255.0 * alpha) );

			return TRUE;
		}

		if ((strClassName.GetLength () > 1) && (-1 == lLongAttribute ) && (alpha > 0.0))
		{
			m_render->put_BrushColor1 (0);
			m_render->put_BrushAlpha1 ((LONG)( dAlplaFill * 255.0 * alpha));

			return TRUE;
		}

		return FALSE;
	}
	BOOL Painter::SetStrokeStyle (const Style& style, const CString& strClassName)
	{
		double alpha			=	style.DoubleAttribute(Opacity);
		double dAlplaStroke		=	style.DoubleAttribute (StrokeOpacity);
		long lColor				=	style.LongAttribute (StrokeColor);
		double dWidth			=	style.DoubleAttribute(StrokeWidth);

		ISvgRef* pStroke		=	style.GetStroke();
		if ((NULL != pStroke) && (dWidth > 0.0))
		{
			m_render->put_PenSize(dWidth * m_dAddMX);

			long nStrokeLineCap		=	style.LongAttribute(StrokeLineCap);
			long nStrokeLineJoin	=	style.LongAttribute(StrokeLineJoin);

			m_render->put_PenLineJoin((unsigned char)(style.LongAttribute(StrokeLineJoin)));

			if (-1 == nStrokeLineCap)
			{
				if (c_ag_LineJoinRound == nStrokeLineJoin)
					m_render->put_PenLineStartCap(c_ag_LineCapRoundAnchor);

				if (c_ag_LineJoinBevel == nStrokeLineJoin)
					m_render->put_PenLineStartCap(c_ag_LineCapTriangle);	//	видимо на строне рендера есть проблема с таким стилем

				if (c_ag_LineJoinMiter == nStrokeLineJoin)
					m_render->put_PenLineStartCap(c_ag_LineCapSquare);
			}
			else
			{
				m_render->put_PenLineStartCap((unsigned char)nStrokeLineCap);
			}

			if (ELinearGradient == pStroke->nodeType())
			{
				LinearGradient* gradient = static_cast<LinearGradient*>(pStroke);
				if (gradient)
				{
					if (gradient->GetColors().Count())
					{
						m_render->put_BrushColor1(gradient->GetColors().Get(0).m_nColor);
						m_render->put_BrushAlpha1((long)(gradient->GetColors().Get(0).m_fOpacity * 255.0 * alpha));

//						VARIANT oVar;
//						oVar.vt			=	VT_BSTR;
//						oVar.bstrVal	=	CComBSTR(gradient->GetXml());
//						//m_render->SetAdditionalParam(L"Stroke-LinearGradient", oVar);
					}
				}
			}
			else if (ERadialGradient == pStroke->nodeType())
			{
				RadialGradient* gradient = static_cast<RadialGradient*>(pStroke);
				if (gradient)
				{
					if (gradient->GetColors().Count())
					{
						m_render->put_BrushColor1(gradient->GetColors().Get(0).m_nColor);
						m_render->put_BrushAlpha1((long)(gradient->GetColors().Get(0).m_fOpacity * 255.0 * alpha));

//						VARIANT oVar;
//						oVar.vt			=	VT_BSTR;
//						oVar.bstrVal	=	CComBSTR(gradient->GetXml());
//						//m_render->SetAdditionalParam(L"Stroke-RadialGradient", oVar);
					}
				}
			}

			return TRUE;
		}

		if (dAlplaStroke > 0.0 && dWidth > 0.0 && ( 0 <= lColor ) && alpha > 0.0)
		{
			m_render->put_PenColor(lColor);
			m_render->put_PenSize(dWidth * m_dAddMX);
			m_render->put_PenAlpha((LONG)(dAlplaStroke * 255.0 * alpha));

			long nStrokeLineCap		=	style.LongAttribute(StrokeLineCap);
			long nStrokeLineJoin	=	style.LongAttribute(StrokeLineJoin);

			m_render->put_PenLineJoin((unsigned char)(style.LongAttribute(StrokeLineJoin)));

			if (-1 == nStrokeLineCap)
			{
				if (c_ag_LineJoinRound == nStrokeLineJoin)
					m_render->put_PenLineStartCap(c_ag_LineCapRoundAnchor);

				if (c_ag_LineJoinBevel == nStrokeLineJoin)
					m_render->put_PenLineStartCap(c_ag_LineCapTriangle);	//	видимо на строне рендера есть проблема с таким стилем

				if (c_ag_LineJoinMiter == nStrokeLineJoin)
					m_render->put_PenLineStartCap(c_ag_LineCapSquare);
			}
			else
			{
				m_render->put_PenLineStartCap((unsigned char)nStrokeLineCap);
			}

			return TRUE;
		}

		long lLongAttribute = lColor;
		if (-2 == lLongAttribute)
			return FALSE;

		if (strClassName.GetLength () > 1 && (-1 == lLongAttribute))
		{
			m_render->put_PenColor	(0);
			m_render->put_PenSize	(dWidth);
			m_render->put_PenAlpha	((LONG)(dAlplaStroke * 255.0 * alpha));

			return TRUE;
		}

		return FALSE;
	}

	void Painter::DoRectangle(Rectangle* element, long type, BOOL clipMode)
	{
		if (!clipMode)
		{
			if ((fabs(element->GetArc().X) < 0.000001) && (fabs(element->GetArc().Y) < 0.000001))
			{
				m_render->PathCommandStart();
				m_render->BeginCommand(c_nPathType);

				m_render->PathCommandStart();

				m_render->PathCommandMoveTo(element->GetFrom().X, element->GetFrom().Y);
				m_render->PathCommandLineTo(element->GetTo().X, element->GetFrom().Y);
				m_render->PathCommandLineTo(element->GetTo().X, element->GetTo().Y);
				m_render->PathCommandLineTo(element->GetFrom().X, element->GetTo().Y);
				m_render->PathCommandLineTo(element->GetFrom().X, element->GetFrom().Y);

				m_render->DrawPath(type);
				m_render->EndCommand(c_nPathType);
				m_render->PathCommandEnd();
			}
			else
			{
				m_render->PathCommandStart();
				m_render->BeginCommand(c_nPathType);

				m_render->PathCommandStart();

				m_render->PathCommandMoveTo(element->GetFrom().X + element->GetArc().X, element->GetFrom().Y);

				m_render->PathCommandLineTo(element->GetTo().X - element->GetArc().X, element->GetFrom().Y);
				m_render->PathCommandArcTo(element->GetTo().X - element->GetArc().X * 2.0, element->GetFrom().Y, element->GetArc().X * 2.0, element->GetArc().Y * 2.0, 270.0, 90.0);

				m_render->PathCommandLineTo(element->GetTo().X, element->GetTo().Y - element->GetArc().Y);
				m_render->PathCommandArcTo(element->GetTo().X - element->GetArc().X * 2.0, element->GetTo().Y - element->GetArc().Y * 2.0, element->GetArc().X * 2.0, element->GetArc().Y * 2.0, 0.0, 90.0);

				m_render->PathCommandLineTo(element->GetFrom().X + element->GetArc().X, element->GetTo().Y);
				m_render->PathCommandArcTo(element->GetFrom().X, element->GetTo().Y - element->GetArc().Y * 2.0, element->GetArc().X * 2.0, element->GetArc().Y * 2.0, 90.0, 90.0);

				m_render->PathCommandLineTo(element->GetFrom().X, element->GetFrom().Y + element->GetArc().Y);
				m_render->PathCommandArcTo(element->GetFrom().X, element->GetFrom().Y, element->GetArc().X * 2.0, element->GetArc().Y * 2.0, 180.0, 90.0);

				m_render->DrawPath(type);
				m_render->EndCommand(c_nPathType);
				m_render->PathCommandEnd ();
			}
		}
		else
		{
			if ((fabs(element->GetArc().X) < 0.000001) && (fabs(element->GetArc().Y) < 0.000001))
			{
				m_render->PathCommandMoveTo(element->GetFrom().X, element->GetFrom().Y);
				m_render->PathCommandLineTo(element->GetTo().X, element->GetFrom().Y);
				m_render->PathCommandLineTo(element->GetTo().X, element->GetTo().Y);
				m_render->PathCommandLineTo(element->GetFrom().X, element->GetTo().Y);
				m_render->PathCommandLineTo(element->GetFrom().X, element->GetFrom().Y);
			}
			else
			{
				Aggplus::CGraphicsPathSimpleConverter simplifier;
				simplifier.SetRenderer(m_render);

				m_render->PathCommandMoveTo(element->GetFrom().X + element->GetArc().X, element->GetFrom().Y);

				m_render->PathCommandLineTo(element->GetTo().X - element->GetArc().X, element->GetFrom().Y);
				simplifier.PathCommandArcTo(element->GetTo().X - element->GetArc().X * 2.0, element->GetFrom().Y, element->GetArc().X * 2.0, element->GetArc().Y * 2.0, 270.0, 90.0);

				m_render->PathCommandLineTo(element->GetTo().X, element->GetTo().Y - element->GetArc().Y);
				simplifier.PathCommandArcTo(element->GetTo().X - element->GetArc().X * 2.0, element->GetTo().Y - element->GetArc().Y * 2.0, element->GetArc().X * 2.0, element->GetArc().Y * 2.0, 0.0, 90.0);

				m_render->PathCommandLineTo(element->GetFrom().X + element->GetArc().X, element->GetTo().Y);
				simplifier.PathCommandArcTo(element->GetFrom().X, element->GetTo().Y - element->GetArc().Y * 2.0, element->GetArc().X * 2.0, element->GetArc().Y * 2.0, 90.0, 90.0);

				m_render->PathCommandLineTo(element->GetFrom().X, element->GetFrom().Y + element->GetArc().Y);
				simplifier.PathCommandArcTo(element->GetFrom().X, element->GetFrom().Y, element->GetArc().X * 2.0, element->GetArc().Y * 2.0, 180.0, 90.0);
			}
		}
	}
	void Painter::DoCircle(Circle* element, long type, BOOL clipMode)
	{
		if (!clipMode)
		{
			m_render->PathCommandStart();
			m_render->BeginCommand (c_nPathType);

			m_render->PathCommandStart ();

			m_render->PathCommandMoveTo(element->m_C.X + element->m_R.X, element->m_C.Y);
			m_render->PathCommandArcTo(element->m_C.X - element->m_R.X, element->m_C.Y - element->m_R.Y, element->m_R.X * 2.0, element->m_R.Y * 2.0, 0, 359.9999);

			m_render->DrawPath (type);
			m_render->EndCommand (c_nPathType);

			m_render->PathCommandEnd ();
		}
		else
		{
			Aggplus::CGraphicsPathSimpleConverter simplifier;
			simplifier.SetRenderer(m_render);

			simplifier.PathCommandMoveTo(element->m_C.X + element->m_R.X, element->m_C.Y);
			simplifier.PathCommandArcTo(element->m_C.X - element->m_R.X, element->m_C.Y - element->m_R.Y, element->m_R.X * 2.0, element->m_R.Y * 2.0, 0, 359.9999);
		}
	}
	void Painter::DoEllipse(Ellipse* element, long type, BOOL clipMode)
	{
		if (!clipMode)
		{
			m_render->PathCommandStart();
			m_render->BeginCommand (c_nPathType);

			m_render->PathCommandStart();

			m_render->PathCommandMoveTo(element->m_C.X + element->m_R.X, element->m_C.Y);
			m_render->PathCommandArcTo(element->m_C.X - element->m_R.X, element->m_C.Y - element->m_R.Y, element->m_R.X * 2.0, element->m_R.Y * 2.0, 0, 359.9999);

			m_render->DrawPath (type);
			m_render->EndCommand (c_nPathType);
			m_render->PathCommandEnd ();
		}
		else
		{
			Aggplus::CGraphicsPathSimpleConverter simplifier;
			simplifier.SetRenderer(m_render);

			simplifier.PathCommandMoveTo(element->m_C.X + element->m_R.X, element->m_C.Y);
			simplifier.PathCommandArcTo(element->m_C.X - element->m_R.X, element->m_C.Y - element->m_R.Y, element->m_R.X * 2.0, element->m_R.Y * 2.0, 0, 359.9999);
		}
	}
	void Painter::DoPolyline(Polyline* element, long type, BOOL clipMode)
	{
		if (!clipMode)
		{
			if (element->GetSize())
			{
				m_render->PathCommandStart();
				m_render->BeginCommand (c_nPathType);

				m_render->PathCommandStart();

				m_render->PathCommandMoveTo (element->GetX(0), element->GetY(0));

				for (long i = 1; i < element->GetSize (); ++i)
					m_render->PathCommandLineTo (element->GetX(i), element->GetY(i));

				m_render->PathCommandLineTo (element->GetX(0), element->GetY(0));

				m_render->DrawPath (type);
				m_render->EndCommand (c_nPathType);
				m_render->PathCommandEnd ();
			}
		}
		else
		{
			m_render->PathCommandMoveTo (element->GetX(0), element->GetY(0));

			for (long i = 1; i < element->GetSize (); ++i)
				m_render->PathCommandLineTo (element->GetX(i), element->GetY(i));

			m_render->PathCommandLineTo (element->GetX(0), element->GetY(0));
		}
	}
	void Painter::DoPolygon(Polygon* element, long type, BOOL clipMode)
	{
		if (!clipMode)
		{
			if (element->GetSize())
			{
				m_render->PathCommandStart();
				m_render->BeginCommand (c_nPathType);

				m_render->PathCommandStart ();

				m_render->PathCommandMoveTo (element->GetX(0), element->GetY(0));

				for (long i = 1; i < element->GetSize (); ++i)
					m_render->PathCommandLineTo (element->GetX(i), element->GetY(i));

				m_render->PathCommandLineTo (element->GetX(0), element->GetY(0));

				m_render->DrawPath (type);
				m_render->EndCommand (c_nPathType);
				m_render->PathCommandEnd ();
			}
		}
		else
		{
			m_render->PathCommandMoveTo (element->GetX(0), element->GetY(0));

			for (long i = 1; i < element->GetSize (); ++i)
				m_render->PathCommandLineTo (element->GetX(i), element->GetY(i));

			m_render->PathCommandLineTo (element->GetX(0), element->GetY(0));
		}
	}
	void Painter::DoPath(Path* element, long type, BOOL clipMode)
	{
		if (!clipMode)
		{
			PushPathCommands (element, type);
		}
		else
		{
			ClipPathCommands (element, type);
		}
	}

	// clip 

	BOOL Painter::DoClip(ISvgRef* pRef, BOOL enable)
	{
		if (NULL == pRef)
			return FALSE;

		if (EClipPath == pRef->nodeType())
		{
			ClipPath* pClip = static_cast<ClipPath*>(pRef);
			if (pClip)
			{
				if (enable)
				{
					if (pClip->GetCount())
					{
						m_render->BeginCommand(c_nClipType);
						m_render->BeginCommand(c_nPathType);

						for (long i = 0; i < pClip->GetCount(); ++i)
						{
							ISvgRef* drawElement = pClip->GetAt(i);
							ENodeType code = drawElement->nodeType();
							if (EUse == code)
							{
								drawElement = FindEntry(static_cast<Use*>(drawElement));
								if (NULL == drawElement)
									continue;

								code = drawElement->nodeType();
							}

							switch (code)
							{
							case ECircle:		DoCircle(static_cast<Circle*>(drawElement), 0, TRUE);		break;
							case ERectangle:	DoRectangle(static_cast<Rectangle*>(drawElement), 0, TRUE);	break;
							case EEllipse:		DoEllipse(static_cast<Ellipse*>(drawElement), 0, TRUE);		break;
							case EPolyline:		DoPolyline(static_cast<Polyline*>(drawElement), 0, TRUE);	break;
							case EPolygon:		DoPolygon(static_cast<Polygon*>(drawElement), 0, TRUE);		break;
							case EPath:			DoPath(static_cast<Path*>(drawElement), 0, TRUE);			break;
							}

							m_render->PathCommandClose ();
						}

						m_render->EndCommand(c_nPathType);
						m_render->EndCommand(c_nClipType);
						m_render->PathCommandEnd();

						return TRUE;
					}

					return FALSE;
				}				
				else 
				{
					m_render->BeginCommand(c_nResetClipType);
					m_render->EndCommand(c_nResetClipType);
				}
			}
		}

		return FALSE;
	}

	// image

	//BOOL Painter::DrawImageFromFile(Image* element, CString texturePath)
	//{
	//	if (0 == texturePath.GetLength())
	//		texturePath = element->m_ImagePath;

	//	if (0 == texturePath.GetLength())
	//		return FALSE;

	//	DoClip (element->GetClip(), TRUE);

	//	BSTR filePath = texturePath.AllocSysString();

	//	LONG type = 0;
	//	type += c_nWindingFillMode;

	//	m_render->put_BrushType(c_BrushTypeTexture);
	//	m_render->put_BrushTexturePath(filePath);
	//	m_render->put_BrushTextureMode(0);

	//	m_render->PathCommandStart();
	//	m_render->BeginCommand(c_nPathType);

	//	m_render->PathCommandStart();

	//	m_render->PathCommandMoveTo(element->GetFrom().X, element->GetFrom().Y);
	//	m_render->PathCommandLineTo(element->GetFrom().X + element->GetSize().X, element->GetFrom().Y);
	//	m_render->PathCommandLineTo(element->GetFrom().X + element->GetSize().X, element->GetFrom().Y + element->GetSize().Y);
	//	m_render->PathCommandLineTo(element->GetFrom().X, element->GetFrom().Y + element->GetSize().Y);
	//	m_render->PathCommandLineTo(element->GetFrom().X, element->GetFrom().Y);

	//	m_render->DrawPath(type);
	//	m_render->EndCommand(c_nPathType);
	//	m_render->PathCommandEnd();

	//	SysFreeString (filePath);

	//	DoClip (element->GetClip(), FALSE);

	//	return TRUE;
	//}
	//BOOL Painter::DoPattern(Pattern* element)
	//{
	//	if (NULL == element)
	//		return FALSE;

	//	if (0 == element->GetSize() || 0 == element->nodeId().GetLength())
	//		return FALSE;

	//	PatternImage* patternImage = NULL;
	//	CAtlMap<CString, PatternImage*>::CPair* cp = m_patterns.Lookup(element->nodeId());
	//	if (NULL == cp)
	//	{
	//		patternImage = new PatternImage(element, m_render, this);
	//		if (patternImage)
	//		{
	//			m_patterns.SetAt(element->nodeId(), patternImage);
	//		}
	//	}
	//	else
	//	{
	//		patternImage = cp->m_value;
	//	}

	//	if (NULL == patternImage)
	//		return FALSE;

	//	m_render->put_BrushType(c_BrushTypeTexture);
	//	m_render->put_BrushTexturePath(CComBSTR(patternImage->LivePath()));
	//	m_render->put_BrushTextureMode(c_BrushTextureModeTile);

	//	return TRUE;

	//	for (long i = 0; i < element->GetSize(); ++i)
	//	{
	//		ISvgRef* drawElement = element->Get(i);
	//		ENodeType code = drawElement->nodeType();

	//		if (EImage == code)
	//		{
	//			Image* imageElement = static_cast<Image*>(drawElement);
	//			if (imageElement)
	//			{
	//				CString path = imageElement->LivePath(m_sWorkingDirectory);
	//				if (path.GetLength())
	//				{
	//					m_render->put_BrushType(c_BrushTypeTexture);
	//					m_render->put_BrushTexturePath(CComBSTR(path));
	//					m_render->put_BrushTextureMode(c_BrushTextureModeTile);
	//				}
	//			}
	//		}	
	//	}

	//	return TRUE;
	//}

	// arc

	double Painter::GetAngle(const double& CX, const double& CY, const double& X, const double& Y)
	{
		double dAngle = 0.0;

		if ( (X - CX) == 0.0 && 0.0 == (Y - CY) )	//	
			return 0.0;

		if ( (X - CX) > 0.0 && 0.0 == (Y - CY) )
			return 0.0;
		if ( (X - CX) < 0.0 && 0.0 == (Y - CY) )
			return 180.0;
		if ( (X - CX) == 0.0 && 0.0 > (Y - CY) )
			return 270.0;
		if ( (X - CX) == 0.0 && 0.0 < (Y - CY) )
			return 90.0;

		dAngle	=	atan ( abs ( X - CX ) / abs ( Y - CY ) ) * 180.0 / M_PI;

		if ( (X - CX) > 0.0 && (Y - CY) > 0.0 )			//	1
		{
			dAngle	=	90.0 - dAngle;
		}

		if ( (X - CX) < 0.0 && (Y - CY) > 0.0 )			//	2
		{
			dAngle	+=	90.0;
		}

		if ( ( (X - CX) < 0.0) && ((Y - CY) < 0.0) )	//	3
		{
			dAngle	=	( 90.0 - dAngle ) + 180.0;
		}

		if ( (X - CX) > 0.0 && (Y - CY) < 0.0 )			//	4
		{
			dAngle	+=	270.0;
		}

		return dAngle;
	}
	Point Painter::GetCenter(int LargeFlag, int SweepFlag, Point Radi, Point P1, Point P2)
	{
		double RadF	=	Radi.Y / Radi.X;

		Point Begin	=	Point ( P1.X * RadF, P1.Y );
		Point End	=	Point ( P2.X * RadF, P2.Y );

		Point Mid	=	Point ( ( Begin.X + End.X ) * 0.5, ( Begin.Y + End.Y ) * 0.5 );
		Point Vec	=	Point ( Begin.X - End.X, Begin.Y - End.Y );

		double HChord	=  sqrt ( Vec.X * Vec.X + Vec.Y * Vec.Y ) * 0.5;

		Point Rot;

		if ( LargeFlag == SweepFlag )
			Rot		=	Point ( -Vec.Y, Vec.X );
		else
			Rot		=	Point ( Vec.Y, -Vec.X );

		Rot.X		/=	( HChord * 2.0 );
		Rot.Y		/=	( HChord * 2.0 );

		double Ch	=	sqrt ( abs ( Radi.Y * Radi.Y - HChord * HChord ) );

		return Point ( ( Mid.X + Ch * Rot.X ) / RadF, Mid.Y + Ch * Rot.Y );
	}
	BOOL Painter::GetArcAngles(int LargeFlag, int SweepFlag, const double& dStartAngle, const double& dEndAngle, double& dSweep)	
	{
		dSweep		=	0.0;

		if ( 1 == LargeFlag && 0 == SweepFlag )
		{
			if ( abs ( dEndAngle - dStartAngle ) > 180 )
				dSweep	=	dEndAngle - dStartAngle;
			else
				dSweep	=	- ( 360 - abs ( dEndAngle - dStartAngle ) );

			if ( SweepFlag ) 
				dSweep	=	abs ( dSweep );
			else
				dSweep	=	-abs ( dSweep );
		}
		else if ( 0 == LargeFlag && 1 == SweepFlag )	
		{
			if ( abs ( dEndAngle - dStartAngle ) < 180 )
				dSweep	=	dEndAngle - dStartAngle;
			else
				dSweep	=	360 - abs ( dEndAngle - dStartAngle );

			if ( SweepFlag ) 
				dSweep	=	abs ( dSweep );
			else
				dSweep	=	-abs ( dSweep );
		}
		else if ( 0 == LargeFlag && 0 == SweepFlag )
		{
			if ( abs ( dEndAngle - dStartAngle ) > 180 )
				dSweep	=	dEndAngle - dStartAngle;
			else
				dSweep	=	- ( 360 - abs ( dEndAngle - dStartAngle ) );

			if ( SweepFlag ) 
				dSweep	=	abs ( dEndAngle - dStartAngle );
			else
				dSweep	=	-abs ( dEndAngle - dStartAngle );
		}
		else if ( 1 == LargeFlag && 1 == SweepFlag )
		{
			if ( abs ( dEndAngle - dStartAngle ) > 180 )
				dSweep	=	dEndAngle - dStartAngle;
			else
				dSweep	=	- ( 360 - abs ( dEndAngle - dStartAngle ) );

			if ( SweepFlag ) 
				dSweep	=	abs ( dSweep );
			else
				dSweep	=	-abs ( dSweep );
		}

		return TRUE;
	}
}

namespace SVG
{
	//BOOL PatternImage::Build()
	//{
	//	if (NULL == m_pattern)
	//		return FALSE;

	//	if (NULL == m_pFrame && NULL == m_render)
	//	{
	//		if (InitFrame() && InitPatternFrame())
	//		{
	//			if (InitRender(m_pFrame, m_pattern->GetBox().GetWidth(), m_pattern->GetBox().GetHeight()))
	//			{
	//				return Render();
	//			}
	//		}
	//	}

	//	return FALSE;
	//}
	//BOOL PatternImage::InitFrame()
	//{
	//	if (m_pFrame)
	//		return TRUE;

	//	if (SUCCEEDED(CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pFrame)))
	//	{
	//		int Width		=	m_pattern->GetBox().GetWidth();
	//		int Height		=	m_pattern->GetBox().GetHeight();

	//		m_pFrame->put_Width(Width);
	//		m_pFrame->put_Height(Height);
	//		m_pFrame->put_Stride(0, 4 * Width);

	//		m_pFrame->put_AspectRatioX(Width);
	//		m_pFrame->put_AspectRatioY(Height);

	//		m_pFrame->put_ColorSpace(CSP_BGRA);
	//		m_pFrame->AllocateBuffer(-1);

	//		BYTE* pBuffer = NULL;
	//		m_pFrame->get_Buffer(&pBuffer);

	//		if (pBuffer)
	//		{
	//			memset (pBuffer, 0, 4 * Width * Height);
	//		}
	//	}

	//	return (NULL != m_pFrame);
	//}
	//BOOL PatternImage::InitPatternFrame()
	//{
	//	if (m_pPatternFrame)
	//		return TRUE;

	//	if (SUCCEEDED(CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pPatternFrame)))
	//	{
	//		m_pPatternFrame->put_Width(m_pattern->GetBound().GetWidth());
	//		m_pPatternFrame->put_Height(m_pattern->GetBound().GetHeight());
	//		m_pPatternFrame->put_Stride(0, 4 * m_pattern->GetBound().GetWidth());

	//		m_pPatternFrame->put_AspectRatioX(m_pattern->GetBound().GetWidth());
	//		m_pPatternFrame->put_AspectRatioY(m_pattern->GetBound().GetHeight());

	//		m_pPatternFrame->put_ColorSpace(CSP_BGRA);
	//		m_pPatternFrame->AllocateBuffer(-1);

	//		BYTE* pBuffer = NULL;
	//		m_pPatternFrame->get_Buffer(&pBuffer);

	//		if (pBuffer)
	//		{
	//			memset (pBuffer, 0, 4 * m_pattern->GetBound().GetWidth() * m_pattern->GetBound().GetHeight());
	//		}
	//	}

	//	return (NULL != m_pFrame);
	//}
	//BOOL PatternImage::InitRender(IUnknown* frame, int frameWidth, int frameHeight)
	//{
	//	if (m_render)
	//		return TRUE;

	//	//if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSGraphicsRenderer), NULL, CLSCTX_ALL, __uuidof(IAVSGraphicsRenderer), (void**)&m_render)))
	//	{
	//		m_render->CreateFromMediaData(frame, 0, 0, frameWidth, frameHeight);

	//		if (m_baseRender)
	//		{
	//			VARIANT vt;
	//			if (SUCCEEDED(m_baseRender->GetAdditionalParam(L"FontManager", &vt)))
	//			{
	//				if (vt.vt == VT_UNKNOWN && NULL != vt.punkVal)
	//				{
	//					m_render->SetAdditionalParam(L"FontManager", vt);
	//				}
	//			}
	//		}
	//	}

	//	return (NULL != m_render);
	//}
	//BOOL PatternImage::Render()
	//{
	//	Point factor = GetNormalizeFactor();
	//	m_pattern->NormalizeEx(factor.X, factor.Y);

	//	Painter painter;
	//	painter.SetWorkingDirectory(m_basePainter->GetWorkingDirectory());
	//	painter.Draw(m_pattern, m_render, m_basePainter->GetUs());

	//	RELEASEINTERFACE(m_render);
	//	if (InitRender(m_pPatternFrame, m_pattern->GetBound().GetWidth(), m_pattern->GetBound().GetHeight()))
	//	{
	//		Point dpi;
	//		
	//		m_render->get_DpiX(&dpi.X);
	//		m_render->get_DpiY(&dpi.Y);

	//		Point sizes;

	//		m_render->get_Width(&sizes.X);
	//		m_render->get_Height(&sizes.Y);

	//		if ((fabs(dpi.X) < 0.000001) || (fabs(dpi.Y) < 0.000001))
	//		{
	//			dpi.X = m_basePainter->GetUs().GetWidth() / sizes.X * 25.4;
	//			dpi.Y = m_basePainter->GetUs().GetHeight() / sizes.Y * 25.4;

	//			if ((fabs(dpi.X) < 0.000001) || (fabs(dpi.Y) < 0.000001))
	//			{
	//				return FALSE;
	//			}
	//		}

	//		Point mul			=	Point(25.4 / dpi.X, 25.4 / dpi.Y);

	//		int Width			=	m_pattern->GetBound().GetWidth();
	//		int Height			=	m_pattern->GetBound().GetHeight();

	//		if (_T("none") != m_pattern->PreserveAspectRatio())			//	TODO: http://www.w3.org/TR/SVG/coords.html#PreserveAspectRatioAttribute
	//		{
	//			if (0 == m_pattern->PreserveAspectRatio().GetLength())	 
	//			{
	//				double dfX	=	(double)m_pattern->GetBox().GetWidth() / (double)m_pattern->GetBound().GetWidth();
	//				double dfY	=	(double)m_pattern->GetBox().GetHeight() / (double)m_pattern->GetBound().GetHeight();

	//				double dF	=	dfX / dfY - 1.0;
	//				if (!(fabs(dF) < 0.000001))
	//				{
	//					dfY		/=	__min(dfY, dfX);
	//					dfX		/=	__min(dfY, dfX);

	//					Width	/=	dfY;
	//					Height	/=	dfX;
	//				}
	//			}
	//		}

	//		if (0 == (int)m_pattern->GetBound().GetX() && 0 == (int)m_pattern->GetBound().GetY())
	//		{
	//			m_render->DrawImage(m_pFrame, 0, 0, Width * mul.X, Height * mul.Y);
	//		}
	//		else
	//		{
	//			// сдвинутый тайл

	//			Point move	=	Point (m_pattern->GetBound().GetX(), m_pattern->GetBound().GetY());

	//			if (m_pattern->GetBound().GetX() > m_pattern->GetBound().GetWidth())
	//				move.X	=	(double) ( (int)m_pattern->GetBound().GetX() % (int)m_pattern->GetBound().GetWidth());

	//			if (m_pattern->GetBound().GetY() > m_pattern->GetBound().GetHeight())
	//				move.Y	=	(double) ( (int)m_pattern->GetBound().GetY() % (int)m_pattern->GetBound().GetHeight());

	//			Point t		=	Point (move.X * mul.X, move.Y * mul.Y);

	//			m_render->DrawImage(m_pFrame, t.X,					t.Y,					Width * mul.X, Height * mul.Y);
	//			m_render->DrawImage(m_pFrame, t.X - Width * mul.X,	t.Y - Height * mul.Y,	Width * mul.X, Height * mul.Y);
	//			m_render->DrawImage(m_pFrame, t.X,					t.Y - Height * mul.Y,	Width * mul.X, Height * mul.Y);
	//			m_render->DrawImage(m_pFrame, t.X - Width * mul.X,	t.Y,					Width * mul.X, Height * mul.Y);
	//		}

	//		m_sLivePath.Format (_T("%s\\%s.png"), FileUtils::GetTempFolder(), m_pattern->nodeId());
	//		SaveImage(m_pPatternFrame, m_sLivePath);
	//	}

	//	// освободим память

	//	RELEASEINTERFACE(m_render);
	//	RELEASEINTERFACE(m_pFrame);
	//	RELEASEINTERFACE(m_pPatternFrame);

	//	return TRUE;
	//}
	//BOOL PatternImage::SaveImage(IUnknown* punkFrame, CString file)
	//{
	//	ImageStudio::IImageTransforms* pTransform = NULL;
	//	CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

	//	VARIANT var;
	//	var.vt = VT_UNKNOWN;
	//	var.punkVal = punkFrame;
	//	pTransform->SetSource(0, var);

	//	CString strXml = _T("<transforms><ImageFile-SaveAsPng destinationpath=\"") + file + _T("\" format=\"888\"/></transforms>");

	//	VARIANT_BOOL vbSuccess = VARIANT_FALSE;
	//	BSTR bsXml = strXml.AllocSysString();
	//	pTransform->SetXml(bsXml, &vbSuccess);
	//	SysFreeString(bsXml);

	//	pTransform->Transform(&vbSuccess);

	//	RELEASEINTERFACE(pTransform);

	//	return TRUE;
	//}
	//Point PatternImage::GetNormalizeFactor()
	//{
	//	if (m_render)
	//	{
	//		Point dpi;
	//		
	//		m_render->get_DpiX(&dpi.X);
	//		m_render->get_DpiY(&dpi.Y);

	//		Point sizes;

	//		m_render->get_Width(&sizes.X);
	//		m_render->get_Height(&sizes.Y);

	//		if ((fabs(dpi.X) < 0.000001) || (fabs(dpi.Y) < 0.000001))
	//		{
	//			dpi.X		=	m_basePainter->GetUs().GetWidth() / sizes.X * 25.4;
	//			dpi.Y		=	m_basePainter->GetUs().GetHeight() / sizes.Y * 25.4;
	//		}

	//		return Point(25.4 / dpi.X, 25.4 / dpi.Y);
	//	}

	//	return Point(1.0, 1.0);
	//}
}
