
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Lin.h"


namespace OOX
{
	namespace Theme
	{

		Lin::Lin()
		{
		}


		Lin::~Lin()
		{
		}


		Lin::Lin(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Lin& Lin::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Lin::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_ang			= element.attribute("ang").value();
		  m_scaled	= element.attribute("scaled").value();
		}


		const XML::XNode Lin::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX