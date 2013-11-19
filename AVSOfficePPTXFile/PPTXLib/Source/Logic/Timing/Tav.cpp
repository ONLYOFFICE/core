#include "Tav.h"

namespace PPTX
{
	namespace Logic
	{

		Tav::Tav()
		{
		}

		Tav::~Tav()
		{
		}

		Tav::Tav(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Tav& Tav::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Tav::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			tm = element.attribute("tm").value();
			fmla = element.attribute("fmla").value();
			val = element.element("val");
			FillParentPointersForChilds();
		}

		const XML::XNode Tav::toXML() const
		{
			return XML::XElement(ns.p + "tav",
				XML::XAttribute("tm", tm) +
				XML::XAttribute("fmla", fmla) +
				XML::Write(val)
				);
		}

		void Tav::FillParentPointersForChilds()
		{
			if(val.is_init())
				val->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX