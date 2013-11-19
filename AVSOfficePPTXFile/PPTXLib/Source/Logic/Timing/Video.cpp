#include "Video.h"

namespace PPTX
{
	namespace Logic
	{

		Video::Video()
		{
		}

		Video::~Video()
		{
		}

		Video::Video(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Video& Video::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Video::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			fullScrn = element.attribute("fullScrn").value();
			cMediaNode = element.element("cMediaNode");

			FillParentPointersForChilds();
		}

		const XML::XNode Video::toXML() const
		{
			return XML::XElement(ns.p + "video",
				XML::XAttribute("fullScrn", fullScrn) +
				XML::Write(cMediaNode)
				);
		}

		void Video::FillParentPointersForChilds()
		{
			cMediaNode->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX