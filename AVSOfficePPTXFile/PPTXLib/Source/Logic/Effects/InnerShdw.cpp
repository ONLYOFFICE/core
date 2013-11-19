#include "InnerShdw.h"

namespace PPTX
{
	namespace Logic
	{

		InnerShdw::InnerShdw()
		{
		}

		InnerShdw::~InnerShdw()
		{
		}

		InnerShdw::InnerShdw(const XML::XNode& node)
		{
			fromXML(node);
		}

		const InnerShdw& InnerShdw::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void InnerShdw::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Color.GetColorFrom(element);
			blurRad = element.attribute("blurRad").value();
			dir = element.attribute("dir").value();
			dist = element.attribute("dist").value();

			FillParentPointersForChilds();
		}

		const XML::XNode InnerShdw::toXML() const
		{
			return XML::XElement(ns.a + "innerShdw",
					XML::XAttribute("blurRad", blurRad) +
					XML::XAttribute("dist", dist) +
					XML::XAttribute("dir", dir) +
					XML::Write(Color)
				);
		}

		void InnerShdw::FillParentPointersForChilds()
		{
			Color.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX