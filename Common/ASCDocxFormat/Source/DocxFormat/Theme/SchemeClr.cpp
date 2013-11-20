
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SchemeClr.h"


namespace OOX
{
	namespace Theme
	{

		SchemeClr::SchemeClr()
		{
		}


		SchemeClr::~SchemeClr()
		{
		}
	

		SchemeClr::SchemeClr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SchemeClr& SchemeClr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SchemeClr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_value	=	element.attribute("val").value();
			m_tint	=	element.element("tint").attribute("val").value();
			m_shade	=	element.element("shade").attribute("val").value();
			m_satMod	=	element.element("satMod").attribute("val").value();
		}


		const XML::XNode SchemeClr::toXML() const
		{
			return
				XML::XElement(ns.a + "schemeClr",
					XML::XAttribute("val", m_value) +
					XML::Write(ns.a + "tint", "val", m_tint) +
					XML::Write(ns.a + "shade", "val", m_shade) +
					XML::Write(ns.a + "satMod", "val", m_satMod)
				);
		}

	} // namespace Theme
} // namespace OOX