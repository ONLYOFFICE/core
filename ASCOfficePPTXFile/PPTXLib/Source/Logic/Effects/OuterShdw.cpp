#include "OuterShdw.h"

namespace PPTX
{
	namespace Logic
	{

		OuterShdw::OuterShdw()
		{
		}


		OuterShdw::~OuterShdw()
		{
		}


		OuterShdw::OuterShdw(const XML::XNode& node)
		{
			fromXML(node);
		}


		const OuterShdw& OuterShdw::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void OuterShdw::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Color.GetColorFrom(element);
			algn = element.attribute("algn").value();
			blurRad = element.attribute("blurRad").value();
			dir = element.attribute("dir").value();
			dist = element.attribute("dist").value();
			kx = element.attribute("kx").value();
			ky = element.attribute("ky").value();
			rotWithShape = element.attribute("rotWithShape").value();
			sx = element.attribute("sx").value();
			sy = element.attribute("sy").value();

			FillParentPointersForChilds();
		}


		const XML::XNode OuterShdw::toXML() const
		{
			return XML::XElement(ns.a + "outerShdw",
					XML::XAttribute("blurRad", blurRad) +
					XML::XAttribute("dist", dist) +
					XML::XAttribute("dir", dir) +
					XML::XAttribute("sx", sx) +
					XML::XAttribute("sy", sy) +
					XML::XAttribute("kx", kx) +
					XML::XAttribute("ky", ky) +
					XML::XAttribute("algn", algn) +
					XML::XAttribute("rotWithShape", rotWithShape) +
					XML::Write(Color)
				);
		}

		void OuterShdw::FillParentPointersForChilds()
		{
			Color.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX