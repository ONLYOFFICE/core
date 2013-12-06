
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "H.h"


namespace OOX
{
	namespace Logic
	{

		H::H()
		{
		}


		H::~H()
		{
		}


		H::H(const XML::XNode& node)
		{
			fromXML(node);
		}


		const H& H::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void H::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Position = element.attribute("position").value();
			XRange	 = element.attribute("xrange").value();
			YRange	 = element.attribute("yrange").value();
		}


		const XML::XNode H::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX