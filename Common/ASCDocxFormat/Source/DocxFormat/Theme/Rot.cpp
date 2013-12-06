
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Rot.h"


namespace OOX
{
	namespace Theme
	{

		Rot::Rot()
		{
		}


		Rot::~Rot()
		{
		}
	

		Rot::Rot(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Rot& Rot::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Rot::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_lat	=	element.attribute("lat").value();
			m_lon	=	element.attribute("lon").value();
			m_rev	=	element.attribute("rev").value();
		}


		const XML::XNode Rot::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX