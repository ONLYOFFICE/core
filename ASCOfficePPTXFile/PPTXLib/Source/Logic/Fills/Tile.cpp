#include "Tile.h"

namespace PPTX
{
	namespace Logic
	{

		Tile::Tile()
		{
		}


		Tile::~Tile()
		{
		}


		Tile::Tile(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Tile& Tile::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Tile::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			algn = element.attribute("algn").value();
			flip = element.attribute("flip").value();
			sx = element.attribute("sx").value();
			sy = element.attribute("sy").value();
			tx = element.attribute("tx").value();
			ty = element.attribute("ty").value();
		}


		const XML::XNode Tile::toXML() const
		{
			 return XML::XElement(ns.a + "tile",
					XML::XAttribute("algn", algn) +
					XML::XAttribute("flip", flip) +
					XML::XAttribute("sx", sx) +
					XML::XAttribute("sy", sy) +
					XML::XAttribute("tx", tx) +
					XML::XAttribute("ty", ty)
				);
		}

	} // namespace Logic
} // namespace PPTX