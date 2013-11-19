#include "BuSzPct.h"

namespace PPTX
{
	namespace Logic
	{

		BuSzPct::BuSzPct()
		{
		}


		BuSzPct::~BuSzPct()
		{
		}


		BuSzPct::BuSzPct(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuSzPct& BuSzPct::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuSzPct::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			val = element.attribute("val").value();
		}


		const XML::XNode BuSzPct::toXML() const
		{
			return XML::XElement(ns.a + "buSzPct",
					XML::XAttribute("val", val)
				);
		}

	} // namespace Logic
} // namespace PPTX