
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ClrSchemeMapping.h"


namespace OOX
{
	namespace Settings
	{

		ClrSchemeMapping::ClrSchemeMapping()
		{
		}


		ClrSchemeMapping::~ClrSchemeMapping()
		{
		}


		ClrSchemeMapping::ClrSchemeMapping(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ClrSchemeMapping& ClrSchemeMapping::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ClrSchemeMapping::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_bg1 = element.attribute("bg1").value();
			m_t1	=	element.attribute("t1").value();
			m_bg2 =	element.attribute("bg2").value();
			m_t2	=	element.attribute("t2").value();
			m_accent1	=	element.attribute("accent1").value();
			m_accent2	=	element.attribute("accent2").value();
			m_accent3	=	element.attribute("accent3").value();
			m_accent4	=	element.attribute("accent4").value();
			m_accent5	=	element.attribute("accent5").value();
			m_accent6	=	element.attribute("accent6").value();
			m_hyperlink	=	element.attribute("hyperlink").value();
			m_followedHyperlink	=	element.attribute("followedHyperlink").value();

		}


		const XML::XNode ClrSchemeMapping::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Settings
} // namespace OOX