
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ObjectDefaults.h"


namespace OOX
{
	namespace Theme
	{

		ObjectDefaults::ObjectDefaults()
		{
		}


		ObjectDefaults::~ObjectDefaults()
		{
		}
	

		ObjectDefaults::ObjectDefaults(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ObjectDefaults& ObjectDefaults::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ObjectDefaults::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode ObjectDefaults::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX