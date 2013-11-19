#include "Blip.h"
#include "./../../Slide.h"
#include "./../../SlideMaster.h"
#include "./../../SlideLayout.h"
#include "./../../Theme.h"

namespace PPTX
{
	namespace Logic
	{
		Blip::Blip()
		{
		}

		Blip::~Blip()
		{
		}

		Blip::Blip(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Blip& Blip::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Blip::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_namespace = element.XName.get().Ns;

			embed = element.attribute("embed").value();
			link = element.attribute("link").value();
			cstate = element.attribute("cstate").value();

			Effects->clear();
			XML::Fill(Effects, element);

			FillParentPointersForChilds();
		}

		const XML::XNode Blip::toXML() const
		{
			 return XML::XElement(m_namespace + "blip",
					XML::XNamespace(ns.r) +
					XML::XAttribute(ns.r + "embed", embed) +
					XML::XAttribute(ns.r + "link", link) +
					XML::XAttribute("cstate", cstate) +
					XML::Write(Effects)
				);
		}

		void Blip::FillParentPointersForChilds()
		{
			for(std::list<UniEffect>::iterator i = Effects->begin(); i != Effects->end(); i++)
				i->SetParentPointer(*this);
		}

		std::wstring Blip::GetFullPicName()const
		{
			if(embed.is_init())
			{
				if(parentFileIs<Slide>())
					return parentFileAs<Slide>().GetMediaFullPathNameFromRId(embed.get());
				else if(parentFileIs<SlideLayout>())
					return parentFileAs<SlideLayout>().GetMediaFullPathNameFromRId(embed.get());
				else if(parentFileIs<SlideMaster>())
					return parentFileAs<SlideMaster>().GetMediaFullPathNameFromRId(embed.get());
				else if(parentFileIs<Theme>())
					return parentFileAs<Theme>().GetMediaFullPathNameFromRId(embed.get());
				return L"";
			}
			return L"";
		}
	} // namespace Logic
} // namespace PPTX