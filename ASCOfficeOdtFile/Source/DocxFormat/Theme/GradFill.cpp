
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "GradFill.h"


namespace OOX
{
	namespace Theme
	{

		GradFill::GradFill()
		{
		}


		GradFill::~GradFill()
		{
		}
	

		GradFill::GradFill(const XML::XNode& node)
		{
			fromXML(node);
		}


		const GradFill& GradFill::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void GradFill::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_rotWithShape	=	element.attribute("rotWithShape").value();
			m_gsLst	=	element.element("gsLst");
			m_lin		=	element.element("lin");
			m_path	=	element.element("path");
		}


		const XML::XNode GradFill::toXML() const
		{
			return 
				XML::XElement(ns.a + "gradFill", 
					XML::XAttribute("rotWithShape", m_rotWithShape) + 
					XML::Write(m_gsLst) +
					XML::Write(m_lin) +
					XML::Write(m_path)
				);
		}

	} // namespace Theme
} // namespace OOX