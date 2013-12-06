
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BevelT.h"


namespace OOX
{
	namespace Theme
	{

		BevelT::BevelT()
		{
		}


		BevelT::~BevelT()
		{
		}
	

		BevelT::BevelT(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BevelT& BevelT::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BevelT::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_w = element.attribute("w").value();
			m_h = element.attribute("h").value();
		}


		const XML::XNode BevelT::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX