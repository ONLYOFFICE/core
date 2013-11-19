#include "Rot.h"

namespace PPTX
{
	namespace Logic
	{

		Rot::Rot()
		{
		}


		Rot::~Rot()
		{
		}


		Rot::Rot(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Rot& Rot::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Rot::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			lat = element.attribute("lat").value();
			lon = element.attribute("lon").value();
			rev = element.attribute("rev").value();
		}


		const XML::XNode Rot::toXML() const
		{
			return XML::XElement(ns.a + "rot",
					XML::XAttribute("lat", lat) +
					XML::XAttribute("lon", lon) +
					XML::XAttribute("rev", rev)
				);
		}

	} // namespace Logic
} // namespace PPTX