#include "SchemeClr.h"
#include "./../../Slide.h"
#include "./../../SlideLayout.h"
#include "./../../SlideMaster.h"
#include "./../../Theme.h"
#include "./../../TableStyles.h"
#include "./../../Presentation.h"

namespace PPTX
{
	namespace Logic
	{
		SchemeClr::SchemeClr()
		{
		}

		SchemeClr::~SchemeClr()
		{
		}
	
		SchemeClr::SchemeClr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SchemeClr& SchemeClr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SchemeClr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			val = element.attribute("val").value();
			Modifiers->clear();
			XML::Fill(Modifiers, element);
		}

		const XML::XNode SchemeClr::toXML() const
		{
			return XML::XElement(ns.a + "schemeClr",
					XML::XAttribute("val", val) +
					XML::Write(Modifiers)
				);
		}

		DWORD SchemeClr::GetRGBA(DWORD RGBA)
		{
			DWORD A = RGBA & 0xFF;
			A = A << 24;
			FillRGBFromVal((RGBA >> 8) + A);
			return ColorBase::GetRGBA(RGBA);
		}

		DWORD SchemeClr::GetARGB(DWORD ARGB)
		{
			FillRGBFromVal(ARGB);
			return ColorBase::GetARGB(ARGB);
		}

		DWORD SchemeClr::GetBGRA(DWORD BGRA)
		{
			DWORD ARGB = (BGRA & 0xFF) << 24;
			ARGB += (BGRA & 0xFF00) << 8;
			ARGB += (BGRA & 0xFF0000) >> 8;
			ARGB += (BGRA & 0xFF000000) >> 24;
			FillRGBFromVal(ARGB);
			return ColorBase::GetBGRA(BGRA);
		}

		DWORD SchemeClr::GetABGR(DWORD ABGR)
		{
			DWORD ARGB = ABGR & 0xFF00FF00;
			ARGB += (ABGR & 0xFF) << 16;
			ARGB += (ABGR & 0xFF0000) >> 16;
			FillRGBFromVal(ARGB);
			return ColorBase::GetABGR(ABGR);
		}

		void SchemeClr::FillRGBFromVal(DWORD rgb)
		{
			DWORD RGB = 0;
			if(val.get() == "phClr")
				RGB = rgb;
			else
			if(parentFileIs<Slide>())
				RGB = parentFileAs<Slide>().GetARGBFromMap(val.get());
			else if(parentFileIs<SlideLayout>())
				RGB = parentFileAs<SlideLayout>().GetARGBFromMap(val.get());
			else if(parentFileIs<SlideMaster>())
				RGB = parentFileAs<SlideMaster>().GetARGBFromMap(val.get());
			else if(parentFileIs<TableStyles>())
				RGB = parentFileAs<TableStyles>().GetARGBFromMap(val.get());
			else if(parentFileIs<Theme>())
				RGB = parentFileAs<Theme>().GetARGBFromMap(val.get());
			else if(parentFileIs<Presentation>())
				RGB = parentFileAs<Presentation>().GetARGBFromMap(val.get());
			//{
			//	if(val.get() == "phClr")
			//		RGB = rgb;
			//	else RGB = parentFileAs<Theme>().GetARGBFromScheme(val.get());
			//}

			blue	= static_cast<unsigned char>(RGB & 0xFF);
			green	= static_cast<unsigned char>((RGB & 0xFF00)>>8);
			red		= static_cast<unsigned char>((RGB & 0xFF0000)>>16);
			alpha	= static_cast<unsigned char>((RGB & 0xFF000000)>>24);
		}
	} // namespace Logic
} // namespace PPTX