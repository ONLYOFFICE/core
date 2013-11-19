#include "PrstDash.h"

namespace PPTX
{
	namespace Logic
	{

		PrstDash::PrstDash()
		{
		}


		PrstDash::~PrstDash()
		{
		}


		PrstDash::PrstDash(const XML::XNode& node)
		{
			fromXML(node);
		}


		const PrstDash& PrstDash::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void PrstDash::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			val = element.attribute("val").value();
		}


		const XML::XNode PrstDash::toXML() const
		{
			return XML::XElement(ns.a + "prstDash",
					XML::XAttribute("val", val)
				);
		}

	} // namespace Logic
} // namespace PPTX