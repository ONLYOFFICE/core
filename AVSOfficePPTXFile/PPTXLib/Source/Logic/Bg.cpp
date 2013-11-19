#include "Bg.h"
#include "../Slide.h"
#include "../SlideMaster.h"
#include "../SlideLayout.h"

namespace PPTX
{
	namespace Logic
	{

		Bg::Bg()
		{
		}


		Bg::~Bg()
		{
		}


		Bg::Bg(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Bg& Bg::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Bg::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			bwMode = element.attribute("bwMode").value();
			//if(element.element("bgPr").exist())
			//	background.reset(new BgPr(element.element("bgPr")));
			//else if(element.element("bgRef").exist())
			//	background.reset(new StyleRef(element.element("bgRef")));
			//else background.reset();
			bgPr = element.element("bgPr");
			bgRef = element.element("bgRef");

			FillParentPointersForChilds();
		}


		const XML::XNode Bg::toXML() const
		{
			return XML::XElement(ns.p + "bg",
					XML::XAttribute("bwMode", bwMode) +
					//XML::Write(background)
					XML::Write(bgPr) +
					XML::Write(bgRef)
				);
		}

		void Bg::FillParentPointersForChilds()
		{
			//if(background != 0)
			//	background->SetParentPointer(*this);
			if(bgPr.is_init())
				bgPr->SetParentPointer(*this);
			if(bgRef.is_init())
				bgRef->SetParentPointer(*this);
		}

		void Bg::GetBackground(Logic::BgPr& bg, DWORD& ARGB)const
		{
			if(bgPr.is_init())
				bg = bgPr.get();
			else// if(cSld->bg->bgRef.is_init())
			{
				ARGB = bgRef->Color.GetARGB();
				if(parentFileIs<Slide>())
					parentFileAs<Slide>().Theme->themeElements->fmtScheme->GetFillStyle(bgRef->idx.get_value_or(0), bg.Fill);
				else if(parentFileIs<SlideLayout>())
					parentFileAs<SlideLayout>().Theme->themeElements->fmtScheme->GetFillStyle(bgRef->idx.get_value_or(0), bg.Fill);
				else if(parentFileIs<SlideMaster>())
					parentFileAs<SlideMaster>().Theme->themeElements->fmtScheme->GetFillStyle(bgRef->idx.get_value_or(0), bg.Fill);
			}
		}

	} // namespace Logic
} // namespace PPTX