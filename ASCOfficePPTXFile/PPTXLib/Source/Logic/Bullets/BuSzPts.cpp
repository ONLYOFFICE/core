#include "BuSzPts.h"

namespace PPTX
{
	namespace Logic
	{

		BuSzPts::BuSzPts()
		{
		}


		BuSzPts::~BuSzPts()
		{
		}


		BuSzPts::BuSzPts(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuSzPts& BuSzPts::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuSzPts::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			val = element.attribute("val").value();
		}


		const XML::XNode BuSzPts::toXML() const
		{
			return XML::XElement(ns.a + "buSzPts",
					XML::XAttribute("val", val)
				);
		}

	} // namespace Logic
} // namespace PPTX