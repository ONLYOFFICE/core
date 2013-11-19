#include "HeadingVariant.h"


namespace PPTX
{
	namespace Logic
	{

		HeadingVariant::HeadingVariant()
		{
		}


		HeadingVariant::~HeadingVariant()
		{
		}


		HeadingVariant::HeadingVariant(const XML::XNode& node)
		{
			fromXML(node);
		}


		const HeadingVariant& HeadingVariant::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void HeadingVariant::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			if(element.element("i4").exist()){
				m_type = "i4";
				m_iContent = element.element("i4").text();
			}
			else{
				m_type = "lpstr";
				m_strContent = element.element("lpstr").text();
			}
		}


		const XML::XNode HeadingVariant::toXML() const
		{
			if(m_type.get()=="i4")
				return	XML::XElement(ns.vt + "variant",
							XML::XElement(ns.vt + m_type.get(),XML::XText(m_iContent))
						);
			else
				return	XML::XElement(ns.vt + "variant",
							XML::XElement(ns.vt + m_type.get(),XML::XText(m_strContent))
						);
		}

	} // namespace Logic
} // namespace PPTX