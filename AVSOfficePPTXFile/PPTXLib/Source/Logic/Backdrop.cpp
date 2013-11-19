#include "Backdrop.h"

namespace PPTX
{
	namespace Logic
	{

		Backdrop::Backdrop()
		{
		}


		Backdrop::~Backdrop()
		{
		}


		Backdrop::Backdrop(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Backdrop& Backdrop::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Backdrop::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			anchorX = element.element("anchor").attribute("x").value();
			anchorY = element.element("anchor").attribute("y").value();
			anchorZ = element.element("anchor").attribute("z").value();

			normX = element.element("norm").attribute("dx").value();
			normY = element.element("norm").attribute("dy").value();
			normZ = element.element("norm").attribute("dz").value();

			upX = element.element("up").attribute("dx").value();
			upY = element.element("up").attribute("dy").value();
			upZ = element.element("up").attribute("dz").value();
		}


		const XML::XNode Backdrop::toXML() const
		{
			return XML::XElement(ns.a + "backdrop",
					XML::XElement(ns.a + "anchor",
						XML::XAttribute("x", anchorX) +
						XML::XAttribute("y", anchorY) +
						XML::XAttribute("z", anchorZ)
					) +
					XML::XElement(ns.a + "norm",
						XML::XAttribute("dx", normX) +
						XML::XAttribute("dy", normY) +
						XML::XAttribute("dz", normZ)
					) +
					XML::XElement(ns.a + "up",
						XML::XAttribute("dx", upX) +
						XML::XAttribute("dy", upY) +
						XML::XAttribute("dz", upZ)
					)
				);
		}

	} // namespace Logic
} // namespace PPTX