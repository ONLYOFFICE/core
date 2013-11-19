#include "Body.h"

namespace Odp
{
	namespace Content
	{
		Body::Body()
		{
		}

		Body::~Body()
		{
		}

		Body::Body(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Body& Body::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Body::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(Slides, element.element("presentation"), "page");
		}

		const XML::XNode Body::toXML() const
		{
			return 
				XML::XElement(ns.office + "body", 
					XML::XElement(ns.office + "presentation",
						XML::Write(Slides)
					)
				);
		}
	} // namespace Content
} // namespace Odp