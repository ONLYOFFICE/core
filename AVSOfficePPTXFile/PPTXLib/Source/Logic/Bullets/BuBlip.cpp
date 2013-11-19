#include "BuBlip.h"

namespace PPTX
{
	namespace Logic
	{

		BuBlip::BuBlip()
		{
		}


		BuBlip::~BuBlip()
		{
		}


		BuBlip::BuBlip(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuBlip& BuBlip::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuBlip::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			blip = element.element("blip");
		}


		const XML::XNode BuBlip::toXML() const
		{
			return XML::XElement(ns.a + "buBlip",
				XML::Write(blip)
			);
		}

		void BuBlip::FillParentPointersForChilds()
		{
			blip->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX