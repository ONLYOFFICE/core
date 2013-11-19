#include "PrstShdw.h"

namespace PPTX
{
	namespace Logic
	{

		PrstShdw::PrstShdw()
		{
		}

		PrstShdw::~PrstShdw()
		{
		}

		PrstShdw::PrstShdw(const XML::XNode& node)
		{
			fromXML(node);
		}

		const PrstShdw& PrstShdw::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void PrstShdw::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Color.GetColorFrom(element);
			prst = element.attribute("prst").value();
			dir = element.attribute("dir").value();
			dist = element.attribute("dist").value();

			FillParentPointersForChilds();
		}

		const XML::XNode PrstShdw::toXML() const
		{
			return XML::XElement(ns.a + "prstShdw",
					XML::XAttribute("prst", prst) +
					XML::XAttribute("dist", dist) +
					XML::XAttribute("dir", dir) +
					XML::Write(Color)
				);
		}

		void PrstShdw::FillParentPointersForChilds()
		{
			Color.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX