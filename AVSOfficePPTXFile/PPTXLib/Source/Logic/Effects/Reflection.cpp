#include "Reflection.h"

namespace PPTX
{
	namespace Logic
	{
		Reflection::Reflection()
		{
		}

		Reflection::~Reflection()
		{
		}

		Reflection::Reflection(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Reflection& Reflection::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Reflection::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			algn = element.attribute("algn").value();
			blurRad = element.attribute("blurRad").value();
			dir = element.attribute("dir").value();
			dist = element.attribute("dist").value();
			kx = element.attribute("kx").value();
			ky = element.attribute("ky").value();
			rotWithShape = element.attribute("rotWithShape").value();
			sx = element.attribute("sx").value();
			sy = element.attribute("sy").value();
			stA = element.attribute("stA").value();
			endA = element.attribute("endA").value();
			stPos = element.attribute("stPos").value();
			endPos = element.attribute("endPos").value();
			fadeDir = element.attribute("fadeDir").value();
		}

		const XML::XNode Reflection::toXML() const
		{
			return XML::XElement(ns.a + "reflection",
					XML::XAttribute("blurRad", blurRad) +
					XML::XAttribute("stA", stA) +
					XML::XAttribute("stPos", stPos) +
					XML::XAttribute("endA", endA) +
					XML::XAttribute("endPos", endPos) +
					XML::XAttribute("dist", dist) +
					XML::XAttribute("dir", dir) +
					XML::XAttribute("fadeDir", fadeDir) +
					XML::XAttribute("sx", sx) +
					XML::XAttribute("sy", sy) +
					XML::XAttribute("kx", kx) +
					XML::XAttribute("ky", ky) +
					XML::XAttribute("algn", algn) +
					XML::XAttribute("rotWithShape", rotWithShape)
				);
		}
	} // namespace Logic
} // namespace PPTX