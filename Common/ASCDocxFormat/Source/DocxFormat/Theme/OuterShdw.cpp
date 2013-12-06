
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "OuterShdw.h"


namespace OOX
{
	namespace Theme
	{

		OuterShdw::OuterShdw()
		{
		}


		OuterShdw::~OuterShdw()
		{
		}
	

		OuterShdw::OuterShdw(const XML::XNode& node)
		{
			fromXML(node);
		}


		const OuterShdw& OuterShdw::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void OuterShdw::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_blurRad	=	element.attribute("blurRad").value();
			m_dist		=	element.attribute("dist").value();
			m_dir			=	element.attribute("dir").value();
			m_rotWithShare	=	element.attribute("rotWithShape").value();
			m_srgbClr	=	element.element("srgbClr");

		}


		const XML::XNode OuterShdw::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX