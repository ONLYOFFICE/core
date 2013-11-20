
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SysClr.h"


namespace OOX
{
	namespace Theme
	{

		SysClr::SysClr()
		{
		}


		SysClr::~SysClr()
		{
		}
	

		SysClr::SysClr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SysClr& SysClr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SysClr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_value		=	element.attribute("val").value();
			m_lastClr	=	element.attribute("lastClr").value();
		}


		const XML::XNode SysClr::toXML() const
		{
			return 
				XML::XElement(ns.a + "sysClr", 
					XML::XAttribute("val", m_value) + 
					XML::XAttribute("lastClr", m_lastClr)
				);
		}

	} // namespace Theme
} // namespace OOX