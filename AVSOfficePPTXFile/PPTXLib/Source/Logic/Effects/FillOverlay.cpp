#include "FillOverlay.h"

namespace PPTX
{
	namespace Logic
	{
		FillOverlay::FillOverlay()
		{
		}

		FillOverlay::~FillOverlay()
		{
		}

		FillOverlay::FillOverlay(const XML::XNode& node)
		{
			fromXML(node);
		}

		const FillOverlay& FillOverlay::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void FillOverlay::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Fill.GetFillFrom(element);
			blend = element.attribute("blend").value();

			FillParentPointersForChilds();
		}

		const XML::XNode FillOverlay::toXML() const
		{
			return XML::XElement(ns.a + "fillOverlay",
					XML::XAttribute("blend", blend) +
					XML::Write(Fill)
				);
		}

		void FillOverlay::FillParentPointersForChilds()
		{
			Fill.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX