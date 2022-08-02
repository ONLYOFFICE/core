#include "SVGFramework.h"

#define ADD_COLOR( COLOR, R, G, B ) m_Table.insert(std::pair<std::wstring, unsigned int>( L##COLOR, ( G << 16 ) | ( G << 8 ) | R ))

namespace SVG
{
	bool isDoubleNull(const double& value)
	{
		double dEps = 0.000001;
		if (fabs(value) < dEps)
			return true;
		return false;
	}

	ColorTable ColorParser::m_oTable;
	void ColorTable::InitClrTable()
	{
		if (m_Table.size())
			return;

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
	}

	long ColorParser::ColorFromString(const std::wstring& sColor)
	{
		// HEX VALUE
		const wchar_t* buf = sColor.c_str();
		size_t len = sColor.length();

		std::wstring::size_type index = sColor.find(L"#");
		if (std::wstring::npos != index)
		{
			for (std::wstring::size_type i = index; i < len; ++i)
			{
				if (' ' == buf[i])
				{
					return ColorFromHexString(sColor.substr(index, i - index));
				}
			}

			return ColorFromHexString(sColor.substr(index, len - index));
		}

		if (L"none" == sColor)
			return -2;

		// 'rgb(x,x,x)'
		if (3 <= len && (buf[0] == 'r') && (buf[1] == 'g') && (buf[2] == 'b'))
			return ColorFromRgbString(sColor);

		// COLOR TABLE
		return m_oTable.Find(sColor);
	}
	long ColorParser::ColorFromRgbString(const std::wstring& Rgb)
	{
		bool PCT = false;
		double Color [ 4 ]	=	{ 0.0, 0.0, 0.0, 255 };

		bool Begin = false;
		size_t IndS = 0;
		size_t IndCol =	0;

		const wchar_t* buf = Rgb.c_str();
		size_t len = Rgb.length();
		for ( size_t i = 0; i < len; ++i )
		{
			if ( isdigit ( buf [ i ] ) )
			{
				if ( false == Begin )
				{
					Begin	=	true;
					IndS	=	i;
				}
			}
			else
			{
				if ( Begin )
				{
					if ( i - IndS >= 1 )
					{
						Color [ IndCol++ ]	=	std::stod ( Rgb.substr ( IndS, i - IndS ) );
					}
				}

				Begin	=	false;
			}

			if ( '%' == buf[i] )
			{
				PCT	=	true;
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

		return ( ( (int) (Color [ 2 ]) << 16 ) | ( (int) (Color [ 1 ]) << 8 ) | (int) (Color [ 1 ]) );
	}
	long ColorParser::ColorFromHexString(const std::wstring& Hex)
	{
		std::wstring value	=	Hex.substr (1);
		const wchar_t* buf = value.c_str();
		if ( 3 == value.length () )	//	for ex ~ #fb0 expands to #ffbb00
		{
			std::wstring tmp;
			tmp += buf[0];
			tmp += buf[0];
			tmp += buf[1];
			tmp += buf[1];
			tmp += buf[2];
			tmp += buf[2];
			value = tmp;
		}

		buf = value.c_str();
		size_t len = value.length();
		for ( size_t i = 0; i < len; ++i )
		{
			if ( isdigit ( buf [ i ] ) || (( buf [ i ] >= L'a' ) && ( buf [ i ] <= L'f' )) || (( buf [ i ] >= L'A' ) && ( buf [ i ] <= L'F' )) )
				continue;

			if (0 == i)
				return 0;
			long InvCol = std::stoi ( value.substr ( 0, i ), NULL, 16 );
			return ( ( InvCol & 0xFF ) << 16 ) | ( ( InvCol & 0xFF00 ) ) | ( ( InvCol & 0xFF0000 ) >> 16 );
		}

		long InvCol = std::stoi( value, NULL, 16 );
		return ( ( InvCol & 0xFF ) << 16 ) | ( ( InvCol & 0xFF00 ) ) | ( ( InvCol & 0xFF0000 ) >> 16 );
	}
}

namespace SVG
{
	void Painter::SetTransform(const double& sx, const double& shy, const double& shx, const double& sy, const double& tx, const double& ty)
	{
		Aggplus::CMatrix tmp(sx, shy, shx, sy, tx, ty);
		tmp.Multiply(&m_baseTransform, Aggplus::MatrixOrderAppend);
		m_render->SetTransform(tmp.sx(), tmp.shy(), tmp.shx(), tmp.sy(), tmp.tx(), tmp.ty());
	}

	bool Painter::Draw(IRefStorage* model, IRenderer* render, const UnitSystem& oUs, double dX, double dY, double dW, double dH)
	{
		if (NULL == model || NULL == render)
			return false;

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

#if 0
		m_dAddMX	=	dW / m_oUs.GetWidth();
		m_dAddMY	=	dH / m_oUs.GetHeight();

		m_model->Normalize(m_dAddMX, m_dAddMY);
#endif

		double oldTransform[6];
		oldTransform[0] = oldTransform[3] = 1;
		oldTransform[1] = oldTransform[2] = oldTransform[4] = oldTransform[5] = 0;
		m_render->GetTransform(&oldTransform[0], &oldTransform[1], &oldTransform[2], &oldTransform[3], &oldTransform[4], &oldTransform[5]);
		m_baseTransform.SetElements(oldTransform[0], oldTransform[1], oldTransform[2], oldTransform[3], oldTransform[4], oldTransform[5]);

		Aggplus::CMatrix rect(dW / m_oUs.GetWidth(), 0, 0, dH / m_oUs.GetHeight(), dX, dY);
		m_baseTransform.Multiply(&rect, Aggplus::MatrixOrderAppend);

		m_dAddMX = 1;
		m_dAddMY = 1;

		this->SetTransform(1, 0, 0, 1, 0, 0);

		bool bRes = DrawStorage(m_model);

		m_render->SetTransform(oldTransform[0], oldTransform[1], oldTransform[2], oldTransform[3], oldTransform[4], oldTransform[5]);

		return bRes;
	}
	bool Painter::DrawLine (Line* element, const Style& oStyle, const std::wstring& strClassName)
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

		return true;
	}
	bool Painter::DrawRectangle (Rectangle* element, const Style& oStyle, const std::wstring& strClassName)
	{
		DoClip (element->GetClip(), true);

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)
			DoRectangle (element, type);

		DoClip (element, false);

		return true;
	}
	bool Painter::DrawCircle (Circle* element,const Style& oStyle, const std::wstring& strClassName)
	{
		DoClip (element->GetClip(), true);

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)
			DoCircle (element, type);

		DoClip (element->GetClip(), false);

		return true;
	}
	bool Painter::DrawEllipse (Ellipse* element, const Style& oStyle, const std::wstring& strClassName)
	{
		DoClip (element->GetClip(), true);

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)
			DoEllipse (element, type);

		DoClip (element->GetClip(), false);

		return true;
	}
	bool Painter::DrawPolyline (Polyline* element, const Style& oStyle, const std::wstring& strClassName)
	{
		DoClip (element->GetClip(), true);

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)
		{
			DoPolyline (element, type);
		}

		DoClip (element->GetClip(), false);

		return true;
	}
	bool Painter::DrawPolygon (Polygon* element, const Style& oStyle, const std::wstring& strClassName)
	{
		DoClip (element->GetClip(), true);

		LONG type = 0;
		if (SetBrushStyle(oStyle, strClassName))
			type += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			type += c_nStroke;

		if (0 != type)
		{
			DoPolygon (element, type);
		}

		DoClip (element->GetClip(), false);

		return true;
	}
	bool Painter::DrawPath (Path* element, const Style& oStyle, const std::wstring& strClassName)
	{
		DoClip (element->GetClip(), true);

		LONG lType = 0;
		if (SetBrushStyle(oStyle, strClassName))
			lType += c_nWindingFillMode;
		if (SetStrokeStyle(oStyle, strClassName))
			lType += c_nStroke;

		if (0 != lType)
			PushPathCommands (element, lType);

		DoClip (element->GetClip(), false);

		return true;
	}
	bool Painter::DrawText (Text* element, const Style& oStyle, const std::wstring& strClassName)
	{
		if (!m_bEnableFonts)
			return false;

		NSFonts::IFontManager* fontManager = GetFontManager();
		if (NULL == fontManager)
			return false;

		bool clipOn = DoClip (element->GetClip(), true);

		SetBrushStyle (oStyle, strClassName);

		double dpiX = 0.0;	m_render->get_DpiX ( &dpiX );
		double dpiY = 0.0;	m_render->get_DpiY ( &dpiY );

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
				DoClip (element->GetClip(), false);

			return false;
		}

		std::wstring bsFontFamily	=	element->GetFontStyle().GetAttribute(FontFamily);
		std::wstring bsText			=	element->GetText();

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

		fontManager->LoadFontByName ( bsFontFamily, dFontSize, 0, 72.0, 72.0 );
		fontManager->LoadString2 ( bsText, 0, 0 );
		TBBox bBox = fontManager->MeasureString();

		bBox.fMinX *= (25.4f / 72.0f);
		bBox.fMinY *= (25.4f / 72.0f);

		float m_fLineSpacing = fontManager->GetLineHeight();
		float m_fEmHeight = fontManager->GetUnitsPerEm();
		float m_fAscent = fontManager->GetAscender();
		float m_fDescent = fontManager->GetDescender();

		double OffSetY = 3 * (m_fLineSpacing - m_fDescent) - m_fAscent;
		OffSetY /= 2.0;
		OffSetY *= (dFontSize / m_fEmHeight);
		//OffSetY *= dpiY / 72.0;
		OffSetY *= (25.4 / 72.0);

		if ( FontTextAnchorStart == element->GetFontStyle().LongAttribute(FontTextAnchor))
		{
			m_render->CommandDrawText ( bsText, element->m_Pos.X, element->m_Pos.Y, bBox.fMinX, OffSetY );
		}

		if ( FontTextAnchorMiddle == element->GetFontStyle().LongAttribute(FontTextAnchor))
		{
			m_render->CommandDrawText ( bsText, element->m_Pos.X - bBox.fMinX * 0.5, element->m_Pos.Y, bBox.fMinX, OffSetY );
		}

		if (FontTextAnchorEnd == element->GetFontStyle ().LongAttribute(FontTextAnchor))
		{
			m_render->CommandDrawText ( bsText, element->m_Pos.X - bBox.fMinX, element->m_Pos.Y, bBox.fMinX, OffSetY );
		}

		DoClip (element->GetClip(), false);
		return true;
	}
	bool Painter::DrawImage (Image* element, const Style& oStyle, const std::wstring& strClassName)
	{
		if (!element->GetXLink().empty())
		{
			DrawImageFromFile (element, element->LivePath(m_sWorkingDirectory));
		}
		else if (!element->m_ImagePath.empty())
		{
			DrawImageFromFile (element);
		}
		else
		{
			// TODO : save images

			// TODO:
			/*
			IUnknown* pImage = NULL;
			if (element->m_bin64Image.GetImage(pImage))
			{
				m_render->DrawImage(pImage, element->m_Or.X, element->m_Or.Y, element->m_Size.X, element->m_Size.Y);
				RELEASEINTERFACE(pImage);
			}
			*/
		}

		return true;
	}
	bool Painter::DrawUse (Use* element, const Style& oStyle, const std::wstring& strClassName)
	{
		bool retVal = false;

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
					bool bStatus = false;
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
					m_transforms.Push(element->GetTransform(), false);

					Matrix local = m_transforms.GetFinal();

					Style oCompleteStyle;
					Style::UpdateValidateAttributes(drawElement->GetStyle(), oCompleteStyle);
					Style::UpdateValidateAttributes(element->GetStyle(), oCompleteStyle);

					element->GetStyle();
					retVal = DrawInternal (drawElement, local, element->GetFrom(), oCompleteStyle);

					m_transforms.Pop();
				}
			}
		}

		return retVal;
	}

	//
	bool Painter::DrawStorage (IRefStorage* pStorage, const Matrix& parentTransform, const Point& off)
	{
		if (pStorage)
		{
			int count = pStorage->GetSize();
			if (0 == count)
				return false;

			m_transforms.Push(parentTransform.Copy().Move(off.X, off.Y), false);
			for (int i = 0; i < count; ++i)
			{
				DrawElement* pE = static_cast<DrawElement*>(pStorage->Get(i));
				if (pE)
				{
					const ENodeType code	=	pE->nodeType();
					if (EClipPath == code)
						continue;

					const std::wstring& css	=	pE->ClassName();
					Style oStyle			=	pE->GetStyle();
					oStyle					=	ComposeStyles(pE, oStyle);

					Matrix mat = parentTransform;
					mat *= Matrix::TranslateTransform(off.X, off.Y);
					mat *= pE->GetTransform();

					this->SetTransform(mat[0], mat[3], mat[1], mat[4], (mat[2] + m_oUs.OffSetX()) * m_dAddMX, (mat[5] + m_oUs.OffSetY()) * m_dAddMY);

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
					case EImage:				DrawImage(static_cast<Image*>(pE),oStyle,css);										continue;	break;
					case EUse:					DrawUse(static_cast<Use*>(pE),oStyle,css);											continue;	break;
					case EGraphicsContainer:	DrawGraphicsContainer(static_cast<GraphicsContainer*>(pE), parentTransform, off);	continue;	break;
					}
				}
			}

			m_transforms.Pop();
		}

		return true;
	}
	bool Painter::DrawGraphicsContainer (GraphicsContainer* element, const Matrix& parentTransform, const Point& off)
	{
		DoClip (element->GetClip(), true);

		bool ret = DrawStorage (element, parentTransform, off);

		DoClip (element->GetClip(), false);

		return ret;
	}
	bool Painter::DrawInternal (DrawElement* pE, const Matrix& parentTransform, const Point& off, const Style& oMainStyle)
	{
		Matrix mat = parentTransform;
		mat *=	Matrix::TranslateTransform(off.X, off.Y);
		mat *=	pE->GetTransform();

		this->SetTransform (mat[0],	mat[3], mat[1], mat[4], (mat[2] + m_oUs.OffSetX()) * m_dAddMX,	(mat[5] + m_oUs.OffSetY()) * m_dAddMY);

		UpdateClass(pE);

		Style oStyle	=	pE->GetStyle();
		Style::UpdateValidateAttributes (oMainStyle, oStyle);
		oStyle			=	ComposeStyles(pE, oStyle);

		const std::wstring& strClass = pE->ClassName();
		ENodeType code	=	pE->nodeType();

		if (ERectangle == code)
		{
			DrawRectangle(static_cast<Rectangle*>(pE),oStyle,strClass);			return true;
		}
		else if (ELine == code)
		{
			DrawLine(static_cast<Line*>(pE),oStyle,strClass);					return true;
		}
		else if (EEllipse == code)
		{
			DrawEllipse(static_cast<Ellipse*>(pE),oStyle,strClass);				return true;
		}
		else if (ECircle == code)
		{
			DrawCircle(static_cast<Circle*>(pE),oStyle,strClass);				return true;
		}
		else if (EPath == code)
		{
			DrawPath(static_cast<Path*>(pE),oStyle,strClass);					return true;
		}
		else if (EPolyline == code)
		{
			DrawPolyline(static_cast<Polyline*>(pE),oStyle,strClass);			return true;
		}
		else if (EPolygon == code)
		{
			DrawPolygon(static_cast<Polygon*>(pE),oStyle,strClass);				return true;
		}
		else if (EText == code)
		{
			DrawText(static_cast<Text*>(pE),oStyle,strClass);					return true;
		}
		else if (EImage == code)
		{
			DrawUse(static_cast<Use*>(pE),oStyle,strClass);						return true;
		}
		else if (EUse == code)
		{
			DrawUse(static_cast<Use*>(pE),oStyle,strClass);						return true;
		}

		return false;
	}

	bool Painter::PushPathCommands (Path* element, long PathType)
	{
		m_render->PathCommandStart();
		m_render->BeginCommand ( c_nPathType );

		bool SegmentEnd	=	true;
		Point lastMove;

		Point LastPoint;
		PathParser& oPath	=	element->GetPath ();
		for (long i = 0; i < (long)element->GetSize (); ++i)
		{
#ifdef _DEBUG
			WCHAR Code = element->GetCode(i);
#endif
			if ('Z' == element->GetCode(i) || 'z' == element->GetCode(i))
			{
				m_render->PathCommandClose();

				SegmentEnd	=	true;
			}
			else if ('m' == element->GetCode(i))
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
			else if ('M' == element->GetCode(i))
			{
				m_render->PathCommandMoveTo(element->GetX(i), element->GetY(i));
				LastPoint	=	element->Get(i);

				lastMove	=	LastPoint;
			}
			// lines
			else if ('l' == element->GetCode(i))
			{
				m_render->PathCommandLineTo(element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y);
				LastPoint	+=	element->Get(i);
			}
			else if ('L' == element->GetCode(i))
			{
				m_render->PathCommandLineTo(element->GetX(i), element->GetY(i));
				LastPoint	=	element->Get(i);
			}
			// Horizontal
			else if ('h' == element->GetCode(i))
			{
				m_render->PathCommandLineTo(element->GetX(i) + LastPoint.X, LastPoint.Y);
				LastPoint.X	+=	element->Get(i).X;
			}
			else if ('H' == element->GetCode(i))
			{
				m_render->PathCommandLineTo(element->GetX(i), LastPoint.Y);
				LastPoint.X	=	element->Get(i).X;
			}
			// Vertical
			else if ('v' == element->GetCode(i))
			{
				m_render->PathCommandLineTo(LastPoint.X, element->GetY(i) + LastPoint.Y);
				LastPoint.Y	+=	element->Get(i).Y;
			}
			else if ('V' == element->GetCode(i))
			{
				m_render->PathCommandLineTo(LastPoint.X, element->GetY(i));
				LastPoint.Y	=	element->Get(i).Y;
			}
			// elliptical arc curve
			else if ('a' == element->GetCode(i))
			{
				int	LargeFlag		=	static_cast<int> ( element->GetY ( i + 1 ) );
				int	SweepFlag		=	static_cast<int> ( element->GetX ( i + 2 ) );

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
			else if ( 'A' == element->GetCode (i) )
			{
				int	LargeFlag		=	static_cast<int> ( element->GetY ( i + 1 ) );
				int	SweepFlag		=	static_cast<int> ( element->GetX ( i + 2 ) );

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
			else  if ( 'C' == element->GetCode (i) )
			{
				m_render->PathCommandCurveTo ( element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1),
											   element->GetX(i+2), element->GetY(i+2) );

				LastPoint	=	element->Get ( i + 2 );

				++i;
				++i;
			}
			else if ( 'c' == element->GetCode (i) )
			{
				m_render->PathCommandCurveTo ( element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
											   element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y,
											   element->GetX(i+2) + LastPoint.X, element->GetY(i+2) + LastPoint.Y );

				LastPoint	+=	element->Get(i+2);

				++i;
				++i;
			}
			else if ( 'S' == element->GetCode (i) )
			{
				if ( 'C' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( 'c' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( 'S' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( 's' == element->GetCode ( i - 1 ) )
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
			else if ( 's' == element->GetCode (i) )
			{
				if ( 'C' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( 'c' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( 'S' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( 's' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					m_render->PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ('m' == element->GetCode(i - 1)
						 || 'l' == element->GetCode(i - 1)
						 || 'v' == element->GetCode(i - 1)
						 || 'h' == element->GetCode(i - 1))
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
			else if ( 'q' == element->GetCode(i))
			{
				m_render->PathCommandCurveTo ( element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
											   element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y,
											   element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );

				LastPoint	+=	element->Get(i+1);

				++i;
			}
			else if ( 'Q' == element->GetCode (i) )
			{
				m_render->PathCommandCurveTo ( element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1), element->GetX(i+1), element->GetY(i+1) );

				LastPoint	=	element->Get(i+1);

				++i;
			}
			else if ( 't' == element->GetCode (i) )
			{
				Point p1 = oPath.GetPoints()[ i - 1 ].oPoint;
				Point p2 = oPath.GetPoints()[ i - 2 ].oPoint;
				Point p3 = oPath.GetPoints()[ i	+ 0	].oPoint;

				if ( 'Q' == element->GetCode ( i - 1 ) )
				{
					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					m_render->PathCommandCurveTo ( dX, dY, p3.X + LastPoint.X, p3.Y + LastPoint.Y, p3.X + LastPoint.X, p3.Y + LastPoint.Y );
				}
				else if ( 'q' == element->GetCode ( i - 1 ) )
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
			else if ( 'T' == element->GetCode (i) )
			{
				Point p1 = oPath.GetPoints()[ i - 1 ].oPoint;
				Point p2 = oPath.GetPoints()[ i - 2 ].oPoint;
				Point p3 = oPath.GetPoints()[ i + 0 ].oPoint;

				if ( 'Q' == element->GetCode ( i - 1 ) )
				{
					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					m_render->PathCommandCurveTo ( dX, dY, p3.X, p3.Y, p3.X, p3.Y );
				}
				else if ( 'q' == element->GetCode ( i - 1 ) )
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

		return true;
	}
	bool Painter::ClipPathCommands (Path* element, long PathType)
	{
		Aggplus::CGraphicsPathSimpleConverter simplifier;
		simplifier.SetRenderer(m_render);

		bool SegmentEnd	=	true;
		Point lastMove;

		Point LastPoint;
		PathParser& oPath = element->GetPath ();

		for (long i = 0; i < (long)element->GetSize (); ++i)
		{
#ifdef _DEBUG
			WCHAR Code = element->GetCode (i);
#endif
			if ( 'Z' == element->GetCode (i) || 'z' == element->GetCode (i) )
			{
				simplifier.PathCommandClose ();

				SegmentEnd	=	true;
			}
			else if ( 'm' == element->GetCode (i) )
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
			else if ( 'M' == element->GetCode (i) )
			{
				simplifier.PathCommandMoveTo ( element->GetX(i), element->GetY(i) );

				LastPoint	=	element->Get(i);

				lastMove	=	LastPoint;
			}
			// lines
			else if ( 'l' == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( element->GetX(i) +  LastPoint.X, element->GetY(i) + LastPoint.Y );

				LastPoint	+=	element->Get(i);
			}
			else if ( 'L' == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( element->GetX(i), element->GetY(i) );

				LastPoint	=	element->Get(i);
			}
			// Horizontal
			else if ( 'h' == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( element->GetX(i) + LastPoint.X, LastPoint.Y );

				LastPoint.X	+=	element->Get(i).X;
			}
			else if ( 'H' == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( element->GetX(i), LastPoint.Y );

				LastPoint.X	=	element->Get(i).X;
			}
			// Vertical
			else if ( 'v' == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( LastPoint.X, element->GetY(i) + LastPoint.Y );

				LastPoint.Y	+=	element->Get(i).Y;
			}
			else if ( 'V' == element->GetCode (i) )
			{
				simplifier.PathCommandLineTo ( LastPoint.X, element->GetY(i) );

				LastPoint.Y	=	element->Get(i).Y;
			}
			// elliptical arc curve
			else if ( 'a' == element->GetCode (i) )
			{
				int	LargeFlag		=	static_cast<int> ( element->GetY ( i + 1 ) );
				int	SweepFlag		=	static_cast<int> ( element->GetX ( i + 2 ) );

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
			else if ( 'A' == element->GetCode (i) )
			{
				int	LargeFlag		=	static_cast<int> ( element->GetY ( i + 1 ) );
				int	SweepFlag		=	static_cast<int> ( element->GetX ( i + 2 ) );

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
			else if ( 'C' == element->GetCode (i) )
			{
				simplifier.PathCommandCurveTo ( element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1),
												element->GetX(i+2), element->GetY(i+2) );

				LastPoint	=	element->Get ( i + 2 );

				++i;
				++i;
			}
			else if ( 'c' == element->GetCode (i) )
			{
				simplifier.PathCommandCurveTo ( element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
												element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y,
												element->GetX(i+2) + LastPoint.X, element->GetY(i+2) + LastPoint.Y );

				LastPoint	+=	element->Get(i+2);

				++i;
				++i;
			}
			else if ( 'S' == element->GetCode (i) )
			{
				if ( 'C' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( 'c' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( 'S' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1) );
				}
				else if ( 's' == element->GetCode ( i - 1 ) )
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
			else if ( 's' == element->GetCode (i) )
			{
				if ( 'C' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( 'c' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( 'S' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	element->Get ( i - 1 );
					Point pReflect	=	element->Get ( i - 2 );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ( 's' == element->GetCode ( i - 1 ) )
				{
					Point pAt		=	Point ( LastPoint.X, LastPoint.Y );
					Point pReflect	=	Point ( pAt.X - element->GetX(i-1) + element->GetX(i-2), pAt.Y - element->GetY(i-1) + element->GetY(i-2) );

					double dX		=	pAt.X - ( pReflect.X - pAt.X );
					double dY		=	pAt.Y - ( pReflect.Y - pAt.Y );

					simplifier.PathCommandCurveTo ( dX, dY, element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y, element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );
				}
				else if ('m' == element->GetCode(i - 1)
						 || 'l' == element->GetCode(i - 1)
						 || 'v' == element->GetCode(i - 1)
						 || 'h' == element->GetCode(i - 1))
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
			else if ( 'q' == element->GetCode (i) )
			{
				simplifier.PathCommandCurveTo ( element->GetX(i) + LastPoint.X, element->GetY(i) + LastPoint.Y,
												element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y,
												element->GetX(i+1) + LastPoint.X, element->GetY(i+1) + LastPoint.Y );

				LastPoint	+=	element->Get(i+1);

				++i;
			}
			else if ( 'Q' == element->GetCode (i) )
			{
				simplifier.PathCommandCurveTo ( element->GetX(i), element->GetY(i), element->GetX(i+1), element->GetY(i+1), element->GetX(i+1), element->GetY(i+1) );

				LastPoint	=	element->Get(i+1);

				++i;
			}
			else if ( 't' == element->GetCode(i))
			{
				Point p1 = oPath.GetPoints()[ i - 1 ].oPoint;
				Point p2 = oPath.GetPoints()[ i - 2 ].oPoint;
				Point p3 = oPath.GetPoints()[ i	+ 0	].oPoint;

				if ( 'Q' == element->GetCode ( i - 1 ) )
				{
					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					simplifier.PathCommandCurveTo ( dX, dY, p3.X + LastPoint.X, p3.Y + LastPoint.Y, p3.X + LastPoint.X, p3.Y + LastPoint.Y );
				}
				else if ( 'q' == element->GetCode ( i - 1 ) )
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
			else if ('T' == element->GetCode(i))
			{
				Point p1 = oPath.GetPoints()[ i - 1 ].oPoint;
				Point p2 = oPath.GetPoints()[ i - 2 ].oPoint;
				Point p3 = oPath.GetPoints()[ i + 0 ].oPoint;

				if ( 'Q' == element->GetCode ( i - 1 ) )
				{
					double dX	=	p1.X - ( p2.X - p1.X );
					double dY	=	p1.Y - ( p2.Y - p1.Y );

					simplifier.PathCommandCurveTo ( dX, dY, p3.X, p3.Y, p3.X, p3.Y );
				}
				else if ( 'q' == element->GetCode ( i - 1 ) )
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

		return true;
	}

	bool Painter::SetBrushStyle (const Style& style, const std::wstring& strClassName)
	{
		double alpha	=	style.DoubleAttribute(Opacity);

		m_render->put_BrushType(c_BrushTypeSolid);

		ISvgRef* pFill	=	style.GetFill();
		if (pFill)
		{
			if (ELinearGradient == pFill->nodeType())
			{
				LinearGradient* gradient = static_cast<LinearGradient*>(pFill);
				if (gradient && gradient->GetColors().Count())
				{
					gradient->ToRenderer(m_render);
				}
			}
			else if (ERadialGradient == pFill->nodeType())
			{
				RadialGradient* gradient = static_cast<RadialGradient*>(pFill);
				if (gradient)
				{
					if (gradient && gradient->GetColors().Count())
					{
						gradient->ToRenderer(m_render);
					}
				}
			}
			else if (EPattern == pFill->nodeType())
			{
				DoPattern (static_cast<Pattern*>(pFill));
			}

			return true;
		}

		long lLongAttribute =	style.LongAttribute(FillColor);
		double dAlplaFill	=	style.DoubleAttribute(FillOpacity);

		if (-3 == lLongAttribute)
		{
			std::wstring::size_type nFound = strClassName.find(L"fill");

			LONG lRendererType = 0;
			m_render->get_Type(&lRendererType);
			if ((c_nSVGConverter != lRendererType) || (nFound == std::wstring::npos))
			{
				m_render->put_BrushColor1 ( 0 );
			}
			m_render->put_BrushAlpha1 ( (LONG)( dAlplaFill * 255.0 * alpha) );
			return true;
		}

		if (-2 == lLongAttribute)
			return false;

		if (dAlplaFill > 0.0 && (-1 != lLongAttribute) && (alpha > 0.0))
		{
			if (!style.ValidAttribute(FillColor))
			{
				LONG lRendererType = 0;
				m_render->get_Type(&lRendererType);
				if (c_nSVGConverter == lRendererType)
				{
					std::wstring::size_type nFound = strClassName.find(L"fill");
					
					if (nFound == std::wstring::npos)
						m_render->put_BrushColor1 ( 0 );

					m_render->put_BrushAlpha1 ( (LONG)( dAlplaFill * 255.0 * alpha) );
					return true;
				}
			}

			m_render->put_BrushColor1 ( lLongAttribute );
			m_render->put_BrushAlpha1 ( (LONG)( dAlplaFill * 255.0 * alpha) );

			return true;
		}

		if ((strClassName.length() > 1) && (-1 == lLongAttribute ) && (alpha > 0.0))
		{
			m_render->put_BrushColor1 (0);
			m_render->put_BrushAlpha1 ((LONG)( dAlplaFill * 255.0 * alpha));

			return true;
		}

		return false;
	}
	bool Painter::SetStrokeStyle (const Style& style, const std::wstring& strClassName)
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

						/*
						VARIANT oVar;
						oVar.vt			=	VT_BSTR;
						oVar.bstrVal	=	CComBSTR(gradient->GetXml());
						m_render->SetAdditionalParam(L"Stroke-LinearGradient", oVar);
						*/
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

						/*
						VARIANT oVar;
						oVar.vt			=	VT_BSTR;
						oVar.bstrVal	=	CComBSTR(gradient->GetXml());
						m_render->SetAdditionalParam(L"Stroke-RadialGradient", oVar);
						*/
					}
				}
			}

			return true;
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

			return true;
		}

		long lLongAttribute = lColor;
		if (-2 == lLongAttribute)
			return false;

		if (strClassName.length() > 1 && (-1 == lLongAttribute))
		{
			m_render->put_PenColor	(0);
			m_render->put_PenSize	(dWidth);
			m_render->put_PenAlpha	((LONG)(dAlplaStroke * 255.0 * alpha));

			return true;
		}

		return false;
	}

	void Painter::DoRectangle(Rectangle* element, long type, bool clipMode)
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
	void Painter::DoCircle(Circle* element, long type, bool clipMode)
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
	void Painter::DoEllipse(Ellipse* element, long type, bool clipMode)
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
	void Painter::DoPolyline(Polyline* element, long type, bool clipMode)
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
	void Painter::DoPolygon(Polygon* element, long type, bool clipMode)
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
	void Painter::DoPath(Path* element, long type, bool clipMode)
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

	bool Painter::DoClip(ISvgRef* pRef, bool enable)
	{
		if (NULL == pRef)
			return false;

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
							case ECircle:		DoCircle(static_cast<Circle*>(drawElement), 0, true);		break;
							case ERectangle:	DoRectangle(static_cast<Rectangle*>(drawElement), 0, true);	break;
							case EEllipse:		DoEllipse(static_cast<Ellipse*>(drawElement), 0, true);		break;
							case EPolyline:		DoPolyline(static_cast<Polyline*>(drawElement), 0, true);	break;
							case EPolygon:		DoPolygon(static_cast<Polygon*>(drawElement), 0, true);		break;
							case EPath:			DoPath(static_cast<Path*>(drawElement), 0, true);			break;
							}

							m_render->PathCommandClose ();
						}

						m_render->EndCommand(c_nPathType);
						m_render->EndCommand(c_nClipType);
						m_render->PathCommandEnd();

						return true;
					}

					return false;
				}
				else
				{
					m_render->BeginCommand(c_nResetClipType);
					m_render->EndCommand(c_nResetClipType);
				}
			}
		}

		return false;
	}

	// image

	bool Painter::DrawImageFromFile(Image* element, std::wstring texturePath)
	{
		if (texturePath.empty())
			texturePath = element->m_ImagePath;

		if (texturePath.empty())
			return false;

		DoClip (element->GetClip(), true);

		LONG type = 0;
		type += c_nWindingFillMode;

		m_render->put_BrushType(c_BrushTypeTexture);
		m_render->put_BrushTexturePath(texturePath);
		m_render->put_BrushTextureMode(0);

		m_render->PathCommandStart();
		m_render->BeginCommand(c_nPathType);

		m_render->PathCommandStart();

		m_render->PathCommandMoveTo(element->GetFrom().X, element->GetFrom().Y);
		m_render->PathCommandLineTo(element->GetFrom().X + element->GetSize().X, element->GetFrom().Y);
		m_render->PathCommandLineTo(element->GetFrom().X + element->GetSize().X, element->GetFrom().Y + element->GetSize().Y);
		m_render->PathCommandLineTo(element->GetFrom().X, element->GetFrom().Y + element->GetSize().Y);
		m_render->PathCommandLineTo(element->GetFrom().X, element->GetFrom().Y);

		m_render->DrawPath(type);
		m_render->EndCommand(c_nPathType);
		m_render->PathCommandEnd();

		DoClip (element->GetClip(), false);

		return true;
	}
	bool Painter::DoPattern(Pattern* element)
	{
		if (NULL == element)
			return false;

		if (0 == element->GetSize() || element->nodeId().empty())
			return false;

		PatternImage* patternImage = NULL;
		std::map<std::wstring, PatternImage*>::iterator cp = m_patterns.find(element->nodeId());
		if (m_patterns.end() == cp)
		{
			patternImage = new PatternImage(element, m_render, this);
			if (patternImage)
			{
				m_patterns.insert(std::pair<std::wstring, PatternImage*>(element->nodeId(), patternImage));
			}
		}
		else
		{
			patternImage = cp->second;
		}

		if (NULL == patternImage)
			return false;

		m_render->put_BrushType(c_BrushTypeTexture);
		m_render->put_BrushTexturePath(patternImage->LivePath());
		m_render->put_BrushTextureMode(c_BrushTextureModeTile);

		return true;

		for (long i = 0; i < element->GetSize(); ++i)
		{
			ISvgRef* drawElement = element->Get(i);
			ENodeType code = drawElement->nodeType();

			if (EImage == code)
			{
				Image* imageElement = static_cast<Image*>(drawElement);
				if (imageElement)
				{
					std::wstring path = imageElement->LivePath(m_sWorkingDirectory);
					if (!path.empty())
					{
						m_render->put_BrushType(c_BrushTypeTexture);
						m_render->put_BrushTexturePath(path);
						m_render->put_BrushTextureMode(c_BrushTextureModeTile);
					}
				}
			}
		}

		return true;
	}

	// arc

	double Painter::GetAngle(const double& CX, const double& CY, const double& X, const double& Y)
	{
		double dAngle = 0.0;

		if ( isDoubleNull(X - CX) && isDoubleNull(Y - CY) )	//
			return 0.0;

		if ( (X - CX) > 0.0 && isDoubleNull(Y - CY) )
			return 0.0;
		if ( (X - CX) < 0.0 && isDoubleNull(Y - CY) )
			return 180.0;
		if ( isDoubleNull(X - CX) && 0.0 > (Y - CY) )
			return 270.0;
		if ( isDoubleNull(X - CX) && 0.0 < (Y - CY) )
			return 90.0;

		dAngle	=	atan ( fabs ( X - CX ) / fabs ( Y - CY ) ) * 180.0 / M_PI;

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
	bool Painter::GetArcAngles(int LargeFlag, int SweepFlag, const double& dStartAngle, const double& dEndAngle, double& dSweep)
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

		return true;
	}
}

namespace SVG
{
	bool PatternImage::Build()
	{
		if (NULL == m_pattern)
			return false;

		if (NULL == m_pFrame && NULL == m_render)
		{
			if (InitFrame() && InitPatternFrame())
			{
				if (InitRender(m_pFrame, m_pattern->GetBox().GetWidth(), m_pattern->GetBox().GetHeight()))
				{
					return Render();
				}
			}
		}

		return false;
	}
	bool PatternImage::InitFrame()
	{
		if (m_pFrame)
			return true;

		int Width		=	m_pattern->GetBox().GetWidth();
		int Height		=	m_pattern->GetBox().GetHeight();

		BYTE* pData = new BYTE[4 * Width * Height];
		memset(pData, 0, 4 * Width * Height);

		m_pFrame = new Aggplus::CImage();
		m_pFrame->Create(pData, Width, Height, 4 * Width);

		return (NULL != m_pFrame);
	}
	bool PatternImage::InitPatternFrame()
	{
		if (m_pPatternFrame)
			return true;

		int Width		=	m_pattern->GetBound().GetWidth();
		int Height		=	m_pattern->GetBound().GetHeight();

		BYTE* pData = new BYTE[4 * Width * Height];
		memset(pData, 0, 4 * Width * Height);

		m_pPatternFrame = new Aggplus::CImage();
		m_pPatternFrame->Create(pData, Width, Height, 4 * Width);

		return (NULL != m_pPatternFrame);
	}
	bool PatternImage::InitRender(Aggplus::CImage* frame, int frameWidth, int frameHeight)
	{
		if (m_render)
			return true;

		CBgraFrame oFrame;
		oFrame.put_Data(frame->GetData());
		oFrame.put_Width((int)frame->GetWidth());
		oFrame.put_Height((int)frame->GetHeight());
		oFrame.put_Stride((int)frame->GetStride());

		m_render = NSGraphics::Create();
		m_render->CreateFromBgraFrame(&oFrame);

		oFrame.put_Data(NULL);

		return (NULL != m_render);
	}
	bool PatternImage::Render()
	{
		Point factor = GetNormalizeFactor();
		m_pattern->NormalizeEx(factor.X, factor.Y);

		Painter painter;
		painter.SetWorkingDirectory(m_basePainter->GetWorkingDirectory());
		painter.Draw(m_pattern, m_render, m_basePainter->GetUs(), 0, 0, 100, 100); // TODO:

		RELEASEINTERFACE(m_render);
		if (InitRender(m_pPatternFrame, m_pattern->GetBound().GetWidth(), m_pattern->GetBound().GetHeight()))
		{
			Point dpi;
			
			m_render->get_DpiX(&dpi.X);
			m_render->get_DpiY(&dpi.Y);

			Point sizes;

			m_render->get_Width(&sizes.X);
			m_render->get_Height(&sizes.Y);

			if ((fabs(dpi.X) < 0.000001) || (fabs(dpi.Y) < 0.000001))
			{
				dpi.X = m_basePainter->GetUs().GetWidth() / sizes.X * 25.4;
				dpi.Y = m_basePainter->GetUs().GetHeight() / sizes.Y * 25.4;

				if ((fabs(dpi.X) < 0.000001) || (fabs(dpi.Y) < 0.000001))
				{
					return false;
				}
			}

			Point mul			=	Point(25.4 / dpi.X, 25.4 / dpi.Y);

			int Width			=	m_pattern->GetBound().GetWidth();
			int Height			=	m_pattern->GetBound().GetHeight();

			if (L"none" != m_pattern->PreserveAspectRatio())			//	TODO: http://www.w3.org/TR/SVG/coords.html#PreserveAspectRatioAttribute
			{
				if (m_pattern->PreserveAspectRatio().empty())
				{
					double dfX	=	(double)m_pattern->GetBox().GetWidth() / (double)m_pattern->GetBound().GetWidth();
					double dfY	=	(double)m_pattern->GetBox().GetHeight() / (double)m_pattern->GetBound().GetHeight();

					double dF	=	dfX / dfY - 1.0;
					if (!(fabs(dF) < 0.000001))
					{
						dfY		/=	std::min(dfY, dfX);
						dfX		/=	std::min(dfY, dfX);

						Width	/=	dfY;
						Height	/=	dfX;
					}
				}
			}

			if (0 == (int)m_pattern->GetBound().GetX() && 0 == (int)m_pattern->GetBound().GetY())
			{
				m_render->DrawImage(m_pFrame, 0, 0, Width * mul.X, Height * mul.Y);
			}
			else
			{
				// сдвинутый тайл

				Point move	=	Point (m_pattern->GetBound().GetX(), m_pattern->GetBound().GetY());

				if (m_pattern->GetBound().GetX() > m_pattern->GetBound().GetWidth())
					move.X	=	(double) ( (int)m_pattern->GetBound().GetX() % (int)m_pattern->GetBound().GetWidth());

				if (m_pattern->GetBound().GetY() > m_pattern->GetBound().GetHeight())
					move.Y	=	(double) ( (int)m_pattern->GetBound().GetY() % (int)m_pattern->GetBound().GetHeight());

				Point t		=	Point (move.X * mul.X, move.Y * mul.Y);

				m_render->DrawImage(m_pFrame, t.X,					t.Y,					Width * mul.X, Height * mul.Y);
				m_render->DrawImage(m_pFrame, t.X - Width * mul.X,	t.Y - Height * mul.Y,	Width * mul.X, Height * mul.Y);
				m_render->DrawImage(m_pFrame, t.X,					t.Y - Height * mul.Y,	Width * mul.X, Height * mul.Y);
				m_render->DrawImage(m_pFrame, t.X - Width * mul.X,	t.Y,					Width * mul.X, Height * mul.Y);
			}

			m_sLivePath = NSDirectory::GetTempPath() + L"/" + m_pattern->nodeId() + L".png";
			SaveImage(m_pPatternFrame, m_sLivePath);
		}

		// освободим память

		RELEASEINTERFACE(m_render);
		RELEASEOBJECT(m_pFrame);
		RELEASEOBJECT(m_pPatternFrame);

		return true;
	}
	bool PatternImage::SaveImage(Aggplus::CImage* pFrame, const std::wstring& file)
	{
		pFrame->SaveFile(file, _CXIMAGE_FORMAT_PNG);
		return true;
	}
	Point PatternImage::GetNormalizeFactor()
	{
		if (m_render)
		{
			Point dpi;
			
			m_render->get_DpiX(&dpi.X);
			m_render->get_DpiY(&dpi.Y);

			Point sizes;

			m_render->get_Width(&sizes.X);
			m_render->get_Height(&sizes.Y);

			if ((fabs(dpi.X) < 0.000001) || (fabs(dpi.Y) < 0.000001))
			{
				dpi.X		=	m_basePainter->GetUs().GetWidth() / sizes.X * 25.4;
				dpi.Y		=	m_basePainter->GetUs().GetHeight() / sizes.Y * 25.4;
			}

			return Point(25.4 / dpi.X, 25.4 / dpi.Y);
		}

		return Point(1.0, 1.0);
	}
}
