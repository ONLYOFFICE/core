
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Ln.h"


namespace OOX
{
	namespace Theme
	{

		Ln::Ln()
		{
		}
	

		Ln::~Ln()
		{
		}


		Ln::Ln(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Ln& Ln::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Ln::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_w			=	element.attribute("w").value();
			m_cap		=	element.attribute("cap").value();
			m_cmpd	=	element.attribute("cmpd").value();
			m_algn	=	element.attribute("algn").value();
			m_solidFill	=	element.element("solidFill");
			m_prstDash	=	element.element("prstDash").attribute("val").value();
		}


		const XML::XNode Ln::toXML() const
		{
			return 
				XML::XElement(ns.a + "ln", 
					XML::XAttribute("w", m_w) +
					XML::XAttribute("cap", m_cap) +
					XML::XAttribute("cmpd", m_cmpd) +
					XML::XAttribute("algn", m_algn) +
					XML::Write(m_solidFill) +
					XML::Write(ns.a + "prstDash", "val", m_prstDash)
				);
		}

	} // namespace Theme
} // namespace OOX