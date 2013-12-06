
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
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX