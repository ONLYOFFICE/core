#include "RelOff.h"

namespace PPTX
{
	namespace Logic
	{

		RelOff::RelOff()
		{
		}

		RelOff::~RelOff()
		{
		}

		RelOff::RelOff(const XML::XNode& node)
		{
			fromXML(node);
		}

		const RelOff& RelOff::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void RelOff::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			tx = element.attribute("tx").value();
			ty = element.attribute("ty").value();
		}

		const XML::XNode RelOff::toXML() const
		{
			return XML::XElement(ns.a + "relOff",
					XML::XAttribute("tx", tx) +
					XML::XAttribute("ty", ty)
			);
		}
	} // namespace Logic
} // namespace PPTX