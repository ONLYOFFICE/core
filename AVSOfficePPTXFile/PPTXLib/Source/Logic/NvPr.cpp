#include "NvPr.h"

namespace PPTX
{
	namespace Logic
	{

		NvPr::NvPr()
		{
		}

		NvPr::~NvPr()
		{
		}

		NvPr::NvPr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const NvPr& NvPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void NvPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			isPhoto = element.attribute("isPhoto").value();
			userDrawn = element.attribute("userDrawn").value();

			ph = element.element("ph");
			media.GetMediaFrom(element);
		}

		const XML::XNode NvPr::toXML() const
		{
			return XML::XElement(ns.p + "nvPr",
				XML::XAttribute("isPhoto", isPhoto) +
				XML::XAttribute("userDrawn", userDrawn) +
				XML::Write(ph) +
				XML::Write(media)
			);
		}

		void NvPr::FillParentPointersForChilds()
		{
			if(ph.is_init())
				ph->SetParentPointer(*this);
			if(media.is_init())
				media.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX