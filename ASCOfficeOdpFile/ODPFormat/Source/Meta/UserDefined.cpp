#include "UserDefined.h"

namespace Odp
{
	namespace Meta
	{
		UserDefined::UserDefined()
		{
		}

		UserDefined::~UserDefined()
		{
		}

		UserDefined::UserDefined(const XML::XNode& node)
		{
			fromXML(node);
		}

		const UserDefined& UserDefined::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void UserDefined::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			name = element.attribute("name").value();
		}

		const XML::XNode UserDefined::toXML() const
		{
			return
				XML::XElement(ns.meta + "user-defined",
					XML::XAttribute(ns.meta + "name", name)
				);
		}

	} // namespace Meta
} // namespace Odt