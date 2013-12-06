
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ClrScheme.h"


namespace OOX
{
	namespace Theme
	{

		ClrScheme::ClrScheme()
		{
		}


		ClrScheme::~ClrScheme()
		{
		}
	

		ClrScheme::ClrScheme(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ClrScheme& ClrScheme::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ClrScheme::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			name = element.attribute("name").value();
			XML::Fill(clrSchemeItems, element);
		}


		const XML::XNode ClrScheme::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX