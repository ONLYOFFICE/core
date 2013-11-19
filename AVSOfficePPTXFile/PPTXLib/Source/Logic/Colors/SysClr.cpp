#include "SysClr.h"
#include "Parse.h"
#include <windows.h>

namespace PPTX
{
	namespace Logic
	{

		SysClr::SysClr()
		{
		}


		SysClr::~SysClr()
		{
		}
	

		SysClr::SysClr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SysClr& SysClr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SysClr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			val = element.attribute("val").value();

			Modifiers->clear();
			XML::Fill(Modifiers, element);
		}


		const XML::XNode SysClr::toXML() const
		{
			char lastClr[10]="";
			sprintf_s(&lastClr[0],10,"%.02X%.02X%.02X", red, green, blue);

			return XML::XElement(ns.a + "sysClr",
				XML::XAttribute("val", val) +
				XML::XAttribute("lastClr", std::string(lastClr)) +
				XML::Write(Modifiers)
			);
		}

		DWORD SysClr::GetRGBA(DWORD RGBA)
		{
			FillRGBFromVal();
			return ColorBase::GetRGBA(RGBA);
		}

		DWORD SysClr::GetARGB(DWORD ARGB)
		{
			FillRGBFromVal();
			return ColorBase::GetARGB(ARGB);
		}

		DWORD SysClr::GetBGRA(DWORD BGRA)
		{
			FillRGBFromVal();
			return ColorBase::GetBGRA(BGRA);
		}

		DWORD SysClr::GetABGR(DWORD ABGR)
		{
			FillRGBFromVal();
			return ColorBase::GetABGR(ABGR);
		}

		void SysClr::FillRGBFromVal()
		{
			DWORD RGB = 0;
			if(val.get() != "")
			{
				switch(val.get()[0])
				{
				case '3':
					if(val.get() == "3dDkShadow") { RGB = ::GetSysColor(COLOR_3DDKSHADOW); break;}
					if(val.get() == "3dLight") { RGB = ::GetSysColor(COLOR_3DLIGHT); break;}
					break;
				case 'a':
					if(val.get() == "activeBorder") { RGB = ::GetSysColor(COLOR_ACTIVEBORDER); break;}
					if(val.get() == "activeCaption") { RGB = ::GetSysColor(COLOR_ACTIVECAPTION); break;}
					if(val.get() == "appWorkspace") { RGB = ::GetSysColor(COLOR_APPWORKSPACE); break;}
					break;
				case 'b':
					if(val.get() == "background") { RGB = ::GetSysColor(COLOR_BACKGROUND); break;}
					if(val.get() == "btnFace") { RGB = ::GetSysColor(COLOR_BTNFACE); break;}
					if(val.get() == "btnHighlight") { RGB = ::GetSysColor(COLOR_BTNHIGHLIGHT); break;}
					if(val.get() == "btnShadow") { RGB = ::GetSysColor(COLOR_BTNSHADOW); break;}
					if(val.get() == "btnText") { RGB = ::GetSysColor(COLOR_BTNTEXT); break;}
					break;
				case 'c':
					if(val.get() == "captionText") { RGB = ::GetSysColor(COLOR_CAPTIONTEXT); break;}
					break;
				case 'g':
					if(val.get() == "gradientActiveCaption") { RGB = ::GetSysColor(COLOR_GRADIENTACTIVECAPTION); break;}
					if(val.get() == "gradientInactiveCaption") { RGB = ::GetSysColor(COLOR_GRADIENTINACTIVECAPTION); break;}
					if(val.get() == "grayText") { RGB = ::GetSysColor(COLOR_GRAYTEXT); break;}
					break;
				case 'h':
					if(val.get() == "highlight") { RGB = ::GetSysColor(COLOR_HIGHLIGHT); break;}
					if(val.get() == "highlightText") { RGB = ::GetSysColor(COLOR_HIGHLIGHTTEXT); break;}
					if(val.get() == "hotLight") { RGB = ::GetSysColor(COLOR_HOTLIGHT); break;}
					break;
				case 'i':
					if(val.get() == "inactiveBorder") { RGB = ::GetSysColor(COLOR_INACTIVEBORDER); break;}
					if(val.get() == "inactiveCaption") { RGB = ::GetSysColor(COLOR_INACTIVECAPTION); break;}
					if(val.get() == "inactiveCaptionText") { RGB = ::GetSysColor(COLOR_INACTIVECAPTIONTEXT); break;}
					if(val.get() == "infoBk") { RGB = ::GetSysColor(COLOR_INFOBK); break;}
					if(val.get() == "infoText") { RGB = ::GetSysColor(COLOR_INFOTEXT); break;}
					break;
				case 'm':
					if(val.get() == "menu") { RGB = ::GetSysColor(COLOR_MENU); break;}
					if(val.get() == "menuBar") { RGB = ::GetSysColor(COLOR_MENUBAR); break;}
					if(val.get() == "menuHighlight") { RGB = ::GetSysColor(COLOR_MENUHILIGHT); break;}
					if(val.get() == "menuText") { RGB = ::GetSysColor(COLOR_MENUTEXT); break;}
					break;
				case 's':
					if(val.get() == "scrollBar") { RGB = ::GetSysColor(COLOR_SCROLLBAR); break;}
					break;
				case 'w':
					if(val.get() == "window") { RGB = ::GetSysColor(COLOR_WINDOW); break;}
					if(val.get() == "windowFrame") { RGB = ::GetSysColor(COLOR_WINDOWFRAME); break;}
					if(val.get() == "windowText") { RGB = ::GetSysColor(COLOR_WINDOWTEXT); break;}
					break;
				}
			}
			blue	= static_cast<unsigned char>(RGB & 0xFF);
			green	= static_cast<unsigned char>((RGB & 0xFF00)>>8);
			red		= static_cast<unsigned char>((RGB & 0xFF0000)>>16);
		}

	} // namespace Logic
} // namespace PPTX