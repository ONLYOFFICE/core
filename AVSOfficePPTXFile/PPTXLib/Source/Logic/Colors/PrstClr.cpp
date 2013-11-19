#include "PrstClr.h"

namespace PPTX
{
	namespace Logic
	{

		PrstClr::PrstClr()
		{
		}


		PrstClr::~PrstClr()
		{
		}
	

		PrstClr::PrstClr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const PrstClr& PrstClr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void PrstClr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			val = element.attribute("val").value();
			Modifiers->clear();
			XML::Fill(Modifiers, element);
		}


		const XML::XNode PrstClr::toXML() const
		{
			return XML::XElement(ns.a + "prstClr",
					XML::XAttribute("val", val) +
					XML::Write(Modifiers)
				);
		}

		DWORD PrstClr::GetRGBA(DWORD RGBA)
		{
			FillRGBFromVal();
			return ColorBase::GetRGBA(RGBA);
		}

		DWORD PrstClr::GetARGB(DWORD ARGB)
		{
			FillRGBFromVal();
			return ColorBase::GetARGB(ARGB);
		}

		DWORD PrstClr::GetBGRA(DWORD BGRA)
		{
			FillRGBFromVal();
			return ColorBase::GetBGRA(BGRA);
		}

		DWORD PrstClr::GetABGR(DWORD ABGR)
		{
			FillRGBFromVal();
			return ColorBase::GetABGR(ABGR);
		}

		void PrstClr::FillRGBFromVal()
		{
			DWORD RGB = 0;
			if(val.get() != "")
			{
				switch(val.get()[0])
				{
				case 'a':
					if(val.get() == "aliceBlue")		{RGB = 0xF0F8FF; break;} // (Alice Blue Preset Color)		Specifies a color with RGB value (240,248,255) 
					if(val.get() == "antiqueWhite")		{RGB = 0xFAEBD7; break;} // (Antique White Preset Color)	Specifies a color with RGB value (250,235,215) 
					if(val.get() == "aqua")				{RGB = 0x00FFFF; break;} // (Aqua Preset Color)				Specifies a color with RGB value (0,255,255) 
					if(val.get() == "aquamarine")		{RGB = 0x7FFFD4; break;} // (Aquamarine Preset Color)		Specifies a color with RGB value (127,255,212) 
					if(val.get() == "azure")			{RGB = 0xF0FFFF; break;} // (Azure Preset Color)			Specifies a color with RGB value (240,255,255) 
					break;
				case 'b':
					if(val.get() == "beige")			{RGB = 0xF5F5DC; break;} // (Beige Preset Color)			Specifies a color with RGB value (245,245,220) 
					if(val.get() == "bisque")			{RGB = 0xFFE4C4; break;} // (Bisque Preset Color)			Specifies a color with RGB value (255,228,196) 
					if(val.get() == "black")			{RGB = 0x000000; break;} // (Black Preset Color)			Specifies a color with RGB value (0,0,0) 
					if(val.get() == "blanchedAlmond")	{RGB = 0xFFEBCD; break;} // (Blanched Almond Preset Color)  Specifies a color with RGB value (255,235,205) 
					if(val.get() == "blue")				{RGB = 0x0000FF; break;} // (Blue Preset Color)				Specifies a color with RGB value (0,0,255) 
					if(val.get() == "blueViolet")		{RGB = 0x8A2BE2; break;} // (Blue Violet Preset Color)		Specifies a color with RGB value (138,43,226) 
					if(val.get() == "brown")			{RGB = 0xA52A2A; break;} // (Brown Preset Color)			Specifies a color with RGB value (165,42,42) 
					if(val.get() == "burlyWood")		{RGB = 0xDEB887; break;} // (Burly Wood Preset Color)		Specifies a color with RGB value (222,184,135) 
					break;
				case 'c':
					if(val.get() == "cadetBlue")		{RGB = 0x5F9EA0; break;} // (Cadet Blue Preset Color)		Specifies a color with RGB value (95,158,160) 
					if(val.get() == "chartreuse")		{RGB = 0x7FFF00; break;} // (Chartreuse Preset Color)		Specifies a color with RGB value (127,255,0) 
					if(val.get() == "chocolate")		{RGB = 0xD2691E; break;} // (Chocolate Preset Color)		Specifies a color with RGB value (210,105,30) 
					if(val.get() == "coral")			{RGB = 0xFF7F50; break;} // (Coral Preset Color)			Specifies a color with RGB value (255,127,80) 
					if(val.get() == "cornflowerBlue")	{RGB = 0x6495ED; break;} // (Cornflower Blue Preset Color)  Specifies a color with RGB value (100,149,237) 
					if(val.get() == "cornsilk")			{RGB = 0xFFF8DC; break;} // (Cornsilk Preset Color)			Specifies a color with RGB value (255,248,220) 
					if(val.get() == "crimson")			{RGB = 0xDC143C; break;} // (Crimson Preset Color)			Specifies a color with RGB value (220,20,60) 
					if(val.get() == "cyan")				{RGB = 0x00FFFF; break;} // (Cyan Preset Color)				Specifies a color with RGB value (0,255,255) 
					break;
				case 'd':
					if(val.get() == "darkBlue")			{RGB = 0x00008B; break;} // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139) 
					if(val.get() == "darkCyan")			{RGB = 0x008B8B; break;} // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139) 
					if(val.get() == "darkGoldenrod")	{RGB = 0xB8860B; break;} // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11) 
					if(val.get() == "darkGray")			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169) 
					if(val.get() == "darkGreen")		{RGB = 0x006400; break;} // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0) 
					if(val.get() == "darkGrey")			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169) 
					if(val.get() == "darkKhaki")		{RGB = 0xBDB76B; break;} // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107) 
					if(val.get() == "darkMagenta")		{RGB = 0x8B008B; break;} // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139) 
					if(val.get() == "darkOliveGreen")	{RGB = 0x556B2F; break;} // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47) 
					if(val.get() == "darkOrange")		{RGB = 0xFF8C00; break;} // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0) 
					if(val.get() == "darkOrchid")		{RGB = 0x9932CC; break;} // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204) 
					if(val.get() == "darkRed")			{RGB = 0x8B0000; break;} // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0) 
					if(val.get() == "darkSalmon")		{RGB = 0xE9967A; break;} // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122) 
					if(val.get() == "darkSeaGreen")		{RGB = 0x8FBC8F; break;} // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,143) 
					if(val.get() == "darkSlateBlue")	{RGB = 0x483D8B; break;} // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139) 
					if(val.get() == "darkSlateGray")	{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79) 
					if(val.get() == "darkSlateGrey")	{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79) 
					if(val.get() == "darkTurquoise")	{RGB = 0x00CED1; break;} // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209) 
					if(val.get() == "darkViolet")		{RGB = 0x9400D3; break;} // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211) 
					if(val.get() == "deepPink")			{RGB = 0xFF1493; break;} // (Deep Pink Preset Color)		Specifies a color with RGB value (255,20,147) 
					if(val.get() == "deepSkyBlue")		{RGB = 0x00BFFF; break;} // (Deep Sky Blue Preset Color)	Specifies a color with RGB value (0,191,255) 
					if(val.get() == "dimGray")			{RGB = 0x696969; break;} // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105) 
					if(val.get() == "dimGrey")			{RGB = 0x696969; break;} // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105) 
					if(val.get() == "dkBlue")			{RGB = 0x00008B; break;} // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139) 
					if(val.get() == "dkCyan")			{RGB = 0x008B8B; break;} // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139) 
					if(val.get() == "dkGoldenrod")		{RGB = 0xB8860B; break;} // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11) 
					if(val.get() == "dkGray")			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169) 
					if(val.get() == "dkGreen")			{RGB = 0x006400; break;} // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0) 
					if(val.get() == "dkGrey")			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169) 
					if(val.get() == "dkKhaki")			{RGB = 0xBDB76B; break;} // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107) 
					if(val.get() == "dkMagenta")		{RGB = 0x8B008B; break;} // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139) 
					if(val.get() == "dkOliveGreen")		{RGB = 0x556B2F; break;} // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47) 
					if(val.get() == "dkOrange")			{RGB = 0xFF8C00; break;} // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0) 
					if(val.get() == "dkOrchid")			{RGB = 0x9932CC; break;} // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204) 
					if(val.get() == "dkRed")			{RGB = 0x8B0000; break;} // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0) 
					if(val.get() == "dkSalmon")			{RGB = 0xE9967A; break;} // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122) 
					if(val.get() == "dkSeaGreen")		{RGB = 0x8FBC8B; break;} // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,139) 
					if(val.get() == "dkSlateBlue")		{RGB = 0x483D8B; break;} // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139) 
					if(val.get() == "dkSlateGray")		{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79) 
					if(val.get() == "dkSlateGrey")		{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79) 
					if(val.get() == "dkTurquoise")		{RGB = 0x00CED1; break;} // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209) 
					if(val.get() == "dkViolet")			{RGB = 0x9400D3; break;} // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211) 
					if(val.get() == "dodgerBlue")		{RGB = 0x1E90FF; break;} // (Dodger Blue Preset Color)		Specifies a color with RGB value (30,144,255) 
					break;
				case 'f':
					if(val.get() == "firebrick")		{RGB = 0xB22222; break;} // (Firebrick Preset Color)		Specifies a color with RGB value (178,34,34) 
					if(val.get() == "floralWhite")		{RGB = 0xFFFAF0; break;} // (Floral White Preset Color)		Specifies a color with RGB value (255,250,240) 
					if(val.get() == "forestGreen")		{RGB = 0x228B22; break;} // (Forest Green Preset Color)		Specifies a color with RGB value (34,139,34) 
					if(val.get() == "fuchsia")			{RGB = 0xFF00FF; break;} // (Fuchsia Preset Color)			Specifies a color with RGB value (255,0,255) 
					break;
				case 'g':
					if(val.get() == "gainsboro")		{RGB = 0xDCDCDC; break;} // (Gainsboro Preset Color)		Specifies a color with RGB value (220,220,220) 
					if(val.get() == "ghostWhite")		{RGB = 0xF8F8FF; break;} // (Ghost White Preset Color)		Specifies a color with RGB value (248,248,255) 
					if(val.get() == "gold")				{RGB = 0xFFD700; break;} // (Gold Preset Color)				Specifies a color with RGB value (255,215,0) 
					if(val.get() == "goldenrod")		{RGB = 0xDAA520; break;} // (Goldenrod Preset Color)		Specifies a color with RGB value (218,165,32) 
					if(val.get() == "gray")				{RGB = 0x808080; break;} // (Gray Preset Color)				Specifies a color with RGB value (128,128,128) 
					if(val.get() == "green")			{RGB = 0x8000; break;} // (Green Preset Color)				Specifies a color with RGB value (0,128,0) 
					if(val.get() == "greenYellow")		{RGB = 0xADFF2F; break;} // (Green Yellow Preset Color)		Specifies a color with RGB value (173,255,47) 
					if(val.get() == "grey")				{RGB = 0x808080; break;} // (Gray Preset Color)				Specifies a color with RGB value (128,128,128) 
					break;
				case 'h':
					if(val.get() == "honeydew")			{RGB = 0xF0FFF0; break;} // (Honeydew Preset Color)			Specifies a color with RGB value (240,255,240) 
					if(val.get() == "hotPink")			{RGB = 0xFF69B4; break;} // (Hot Pink Preset Color)			Specifies a color with RGB value (255,105,180) 
					break;
				case 'i':
					if(val.get() == "indianRed")		{RGB = 0xCD5C5C; break;} // (Indian Red Preset Color)		Specifies a color with RGB value (205,92,92) 
					if(val.get() == "indigo")			{RGB = 0x4B0082; break;} // (Indigo Preset Color)			Specifies a color with RGB value (75,0,130) 
					if(val.get() == "ivory")			{RGB = 0xFFFFF0; break;} // (Ivory Preset Color)			Specifies a color with RGB value (255,255,240) 
					break;
				case 'k':
					if(val.get() == "khaki")			{RGB = 0xF0E68C; break;} // (Khaki Preset Color)			Specifies a color with RGB value (240,230,140) 
					break;
				case 'l':
					if(val.get() == "lavender")			{RGB = 0xE6E6FA; break;} // (Lavender Preset Color)			Specifies a color with RGB value (230,230,250) 
					if(val.get() == "lavenderBlush")	{RGB = 0xFFF0F5; break;} // (Lavender Blush Preset Color)	Specifies a color with RGB value (255,240,245) 
					if(val.get() == "lawnGreen")		{RGB = 0x7CFC00; break;} // (Lawn Green Preset Color)		Specifies a color with RGB value (124,252,0) 
					if(val.get() == "lemonChiffon")		{RGB = 0xFFFACD; break;} // (Lemon Chiffon Preset Color)	Specifies a color with RGB value (255,250,205) 
					if(val.get() == "lightBlue")		{RGB = 0xADD8E6; break;} // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230) 
					if(val.get() == "lightCoral")		{RGB = 0xF08080; break;} // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128) 
					if(val.get() == "lightCyan")		{RGB = 0xE0FFFF; break;} // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255) 
					if(val.get()=="lightGoldenrodYellow"){RGB = 0xFAFAD2;break;} // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,210) 
					if(val.get() == "lightGray")		{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211) 
					if(val.get() == "lightGreen")		{RGB = 0x90EE90; break;} // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144) 
					if(val.get() == "lightGrey")		{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211) 
					if(val.get() == "lightPink")		{RGB = 0xFFB6C1; break;} // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193) 
					if(val.get() == "lightSalmon")		{RGB = 0xFFA07A; break;} // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122) 
					if(val.get() == "lightSeaGreen")	{RGB = 0x20B2AA; break;} // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170) 
					if(val.get() == "lightSkyBlue")		{RGB = 0x87CEFA; break;} // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250) 
					if(val.get() == "lightSlateGray")	{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153) 
					if(val.get() == "lightSlateGrey")	{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153) 
					if(val.get() == "lightSteelBlue")	{RGB = 0xB0C4DE; break;} // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222) 
					if(val.get() == "lightYellow")		{RGB = 0xFFFFE0; break;} // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224) 
					if(val.get() == "lime")				{RGB = 0x00FF00; break;} // (Lime Preset Color)				Specifies a color with RGB value (0,255,0) 
					if(val.get() == "limeGreen")		{RGB = 0x32CD32; break;} // (Lime Green Preset Color)		Specifies a color with RGB value (50,205,50) 
					if(val.get() == "linen")			{RGB = 0xFAF0E6; break;} // (Linen Preset Color)			Specifies a color with RGB value (250,240,230) 
					if(val.get() == "ltBlue")			{RGB = 0xADD8E6; break;} // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230) 
					if(val.get() == "ltCoral")			{RGB = 0xF08080; break;} // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128) 
					if(val.get() == "ltCyan")			{RGB = 0xE0FFFF; break;} // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255) 
					if(val.get() == "ltGoldenrodYellow"){RGB = 0xFAFA78; break;} // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,120) 
					if(val.get() == "ltGray")			{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211) 
					if(val.get() == "ltGreen")			{RGB = 0x90EE90; break;} // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144) 
					if(val.get() == "ltGrey")			{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211) 
					if(val.get() == "ltPink")			{RGB = 0xFFB6C1; break;} // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193) 
					if(val.get() == "ltSalmon")			{RGB = 0xFFA07A; break;} // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122) 
					if(val.get() == "ltSeaGreen")		{RGB = 0x20B2AA; break;} // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170) 
					if(val.get() == "ltSkyBlue")		{RGB = 0x87CEFA; break;} // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250) 
					if(val.get() == "ltSlateGray")		{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153) 
					if(val.get() == "ltSlateGrey")		{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153) 
					if(val.get() == "ltSteelBlue")		{RGB = 0xB0C4DE; break;} // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222) 
					if(val.get() == "ltYellow")			{RGB = 0xFFFFE0; break;} // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224) 
					break;
				case 'm':
					if(val.get() == "magenta")			{RGB = 0xFF00FF; break;} // (Magenta Preset Color)			Specifies a color with RGB value (255,0,255) 
					if(val.get() == "maroon")			{RGB = 0x800000; break;} // (Maroon Preset Color)			Specifies a color with RGB value (128,0,0) 
					if(val.get() == "medAquamarine")	{RGB = 0x66CDAA; break;} // (Medium Aquamarine Preset Color)Specifies a color with RGB value (102,205,170) 
					if(val.get() == "medBlue")			{RGB = 0x0000CD; break;} // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205) 
					if(val.get() == "mediumAquamarine") {RGB = 0x66CDAA; break;} // (Medium Aquamarine Color)		Specifies a color with RGB value (102,205,170) 
					if(val.get() == "mediumBlue")		{RGB = 0x0000CD; break;} // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205) 
					if(val.get() == "mediumOrchid")		{RGB = 0xBA55D3; break;} // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211) 
					if(val.get() == "mediumPurple")		{RGB = 0x9370DB; break;} // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219) 
					if(val.get() == "mediumSeaGreen")	{RGB = 0x3CB371; break;} // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113) 
					if(val.get() == "mediumSlateBlue")	{RGB = 0x7B68EE; break;} // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238) 
					if(val.get() == "mediumSpringGreen"){RGB = 0x00FA9A; break;} // (Medium Spring Color)			Specifies a color with RGB value (0,250,154) 
					if(val.get() == "mediumTurquoise")	{RGB = 0x48D1CC; break;} // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204) 
					if(val.get() == "mediumVioletRed")	{RGB = 0xC71585; break;} // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133) 
					if(val.get() == "medOrchid")		{RGB = 0xBA55D3; break;} // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211) 
					if(val.get() == "medPurple")		{RGB = 0x9370DB; break;} // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219) 
					if(val.get() == "medSeaGreen")		{RGB = 0x3CB371; break;} // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113) 
					if(val.get() == "medSlateBlue")		{RGB = 0x7B68EE; break;} // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238) 
					if(val.get() == "medSpringGreen")	{RGB = 0x00FA9A; break;} // (Medium Spring Preset Color)	Specifies a color with RGB value (0,250,154) 
					if(val.get() == "medTurquoise")		{RGB = 0x48D1CC; break;} // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204) 
					if(val.get() == "medVioletRed")		{RGB = 0xC71585; break;} // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133) 
					if(val.get() == "midnightBlue")		{RGB = 0x191970; break;} // (Midnight Blue Preset Color)	Specifies a color with RGB value (25,25,112) 
					if(val.get() == "mintCream")		{RGB = 0xF5FFFA; break;} // (Mint Cream Preset Color)		Specifies a color with RGB value (245,255,250) 
					if(val.get() == "mistyRose")		{RGB = 0xFFE4FF; break;} // (Misty Rose Preset Color)		Specifies a color with RGB value (255,228,225) 
					if(val.get() == "moccasin")			{RGB = 0xFFE4B5; break;} // (Moccasin Preset Color)			Specifies a color with RGB value (255,228,181) 
					break;
				case 'n':
					if(val.get() == "navajoWhite")		{RGB = 0xFFDEAD; break;} // (Navajo White Preset Color)		Specifies a color with RGB value (255,222,173) 
					if(val.get() == "navy")				{RGB = 0x000080; break;} // (Navy Preset Color)				Specifies a color with RGB value (0,0,128) 
					break;
				case 'o':
					if(val.get() == "oldLace")			{RGB = 0xFDF5E6; break;} // (Old Lace Preset Color)			Specifies a color with RGB value (253,245,230) 
					if(val.get() == "olive")			{RGB = 0x808000; break;} // (Olive Preset Color)			Specifies a color with RGB value (128,128,0) 
					if(val.get() == "oliveDrab")		{RGB = 0x6B8E23; break;} // (Olive Drab Preset Color)		Specifies a color with RGB value (107,142,35) 
					if(val.get() == "orange")			{RGB = 0xFFA500; break;} // (Orange Preset Color)			Specifies a color with RGB value (255,165,0) 
					if(val.get() == "orangeRed")		{RGB = 0xFF4500; break;} // (Orange Red Preset Color)		Specifies a color with RGB value (255,69,0) 
					if(val.get() == "orchid")			{RGB = 0xDA70D6; break;} // (Orchid Preset Color)			Specifies a color with RGB value (218,112,214) 
					break;
				case 'p':
					if(val.get() == "paleGoldenrod")	{RGB = 0xEEE8AA; break;} // (Pale Goldenrod Preset Color)	Specifies a color with RGB value (238,232,170) 
					if(val.get() == "paleGreen")		{RGB = 0x98FB98; break;} // (Pale Green Preset Color)		Specifies a color with RGB value (152,251,152) 
					if(val.get() == "paleTurquoise")	{RGB = 0xAFEEEE; break;} // (Pale Turquoise Preset Color)	Specifies a color with RGB value (175,238,238) 
					if(val.get() == "paleVioletRed")	{RGB = 0xDB7093; break;} // (Pale Violet Red Preset Color)  Specifies a color with RGB value (219,112,147) 
					if(val.get() == "papayaWhip")		{RGB = 0xFFEFD5; break;} // (Papaya Whip Preset Color)		Specifies a color with RGB value (255,239,213) 
					if(val.get() == "peachPuff")		{RGB = 0xFFDAB9; break;} // (Peach Puff Preset Color)		Specifies a color with RGB value (255,218,185) 
					if(val.get() == "peru")				{RGB = 0xCD853F; break;} // (Peru Preset Color)				Specifies a color with RGB value (205,133,63) 
					if(val.get() == "pink")				{RGB = 0xFFC0CB; break;} // (Pink Preset Color)				Specifies a color with RGB value (255,192,203) 
					if(val.get() == "plum")				{RGB = 0xD3A0D3; break;} // (Plum Preset Color)				Specifies a color with RGB value (221,160,221) 
					if(val.get() == "powderBlue")		{RGB = 0xB0E0E6; break;} // (Powder Blue Preset Color)		Specifies a color with RGB value (176,224,230) 
					if(val.get() == "purple")			{RGB = 0x800080; break;} // (Purple Preset Color)			Specifies a color with RGB value (128,0,128) 
					break;
				case 'r':
					if(val.get() == "red")				{RGB = 0xFF0000; break;} // (Red Preset Color)				Specifies a color with RGB value (255,0,0) 
					if(val.get() == "rosyBrown")		{RGB = 0xBC8F8F; break;} // (Rosy Brown Preset Color)		Specifies a color with RGB value (188,143,143) 
					if(val.get() == "royalBlue")		{RGB = 0x4169E1; break;} // (Royal Blue Preset Color)		Specifies a color with RGB value (65,105,225) 
					break;
				case 's':
					if(val.get() == "saddleBrown")		{RGB = 0x8B4513; break;} // (Saddle Brown Preset Color)		Specifies a color with RGB value (139,69,19) 
					if(val.get() == "salmon")			{RGB = 0xFA8072; break;} // (Salmon Preset Color)			Specifies a color with RGB value (250,128,114) 
					if(val.get() == "sandyBrown")		{RGB = 0xF4A460; break;} // (Sandy Brown Preset Color)		Specifies a color with RGB value (244,164,96) 
					if(val.get() == "seaGreen")			{RGB = 0x2E8B57; break;} // (Sea Green Preset Color)		Specifies a color with RGB value (46,139,87) 
					if(val.get() == "seaShell")			{RGB = 0xFFF5EE; break;} // (Sea Shell Preset Color)		Specifies a color with RGB value (255,245,238) 
					if(val.get() == "sienna")			{RGB = 0xA0522D; break;} // (Sienna Preset Color)			Specifies a color with RGB value (160,82,45) 
					if(val.get() == "silver")			{RGB = 0xC0C0C0; break;} // (Silver Preset Color)			Specifies a color with RGB value (192,192,192) 
					if(val.get() == "skyBlue")			{RGB = 0x87CEEB; break;} // (Sky Blue Preset Color)			Specifies a color with RGB value (135,206,235) 
					if(val.get() == "slateBlue")		{RGB = 0x6A5AEB; break;} // (Slate Blue Preset Color)		Specifies a color with RGB value (106,90,205) 
					if(val.get() == "slateGray")		{RGB = 0x708090; break;} // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144) 
					if(val.get() == "slateGrey")		{RGB = 0x708090; break;} // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144) 
					if(val.get() == "snow")				{RGB = 0xFFFAFA; break;} // (Snow Preset Color)				Specifies a color with RGB value (255,250,250) 
					if(val.get() == "springGreen")		{RGB = 0x00FF7F; break;} // (Spring Green Preset Color)		Specifies a color with RGB value (0,255,127) 
					if(val.get() == "steelBlue")		{RGB = 0x4682B4; break;} // (Steel Blue Preset Color)		Specifies a color with RGB value (70,130,180) 
					break;
				case 't':
					if(val.get() == "tan")				{RGB = 0xD2B48C; break;} // (Tan Preset Color)				Specifies a color with RGB value (210,180,140) 
					if(val.get() == "teal")				{RGB = 0x008080; break;} // (Teal Preset Color)				Specifies a color with RGB value (0,128,128) 
					if(val.get() == "thistle")			{RGB = 0xD8BFD8; break;} // (Thistle Preset Color)			Specifies a color with RGB value (216,191,216) 
					if(val.get() == "tomato")			{RGB = 0xFF7347; break;} // (Tomato Preset Color)			Specifies a color with RGB value (255,99,71) 
					if(val.get() == "turquoise")		{RGB = 0x40E0D0; break;} // (Turquoise Preset Color)		Specifies a color with RGB value (64,224,208) 
					break;
				case 'v':
					if(val.get() == "violet")			{RGB = 0xEE82EE; break;} // (Violet Preset Color)			Specifies a color with RGB value (238,130,238) 
					break;
				case 'w':
					if(val.get() == "wheat")			{RGB = 0xF5DEB3; break;} // (Wheat Preset Color)			Specifies a color with RGB value (245,222,179) 
					if(val.get() == "white")			{RGB = 0xFFFFFF; break;} // (White Preset Color)			Specifies a color with RGB value (255,255,255) 
					if(val.get() == "whiteSmoke")		{RGB = 0xF5F5F5; break;} // (White Smoke Preset Color)		Specifies a color with RGB value (245,245,245) 
					break;
				case 'y':
					if(val.get() == "yellow")			{RGB = 0xFFFF00; break;} // (Yellow Preset Color)			Specifies a color with RGB value (255,255,0) 
					if(val.get() == "yellowGreen")		{RGB = 0x9ACD32; break;} // (Yellow Green Preset Color)		Specifies a color with RGB value (154,205,50) 
					break;
				}
			}

			blue	= static_cast<unsigned char>(RGB & 0xFF);
			green	= static_cast<unsigned char>((RGB & 0xFF00)>>8);
			red		= static_cast<unsigned char>((RGB & 0xFF0000)>>16);
		}
	} // namespace Logic
} // namespace PPTX