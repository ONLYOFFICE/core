
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ShapeDefault.h"


namespace OOX
{
	namespace Settings
	{

		ShapeDefault::ShapeDefault()
		{
		}


		ShapeDefault::~ShapeDefault()
		{
		}


		ShapeDefault::ShapeDefault(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ShapeDefault& ShapeDefault::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ShapeDefault::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_ext			=	element.attribute("ext").value();
			m_spidmax =	element.attribute("spidmax").value().ToString();
		}


		const XML::XNode ShapeDefault::toXML() const
		{
			return 
				XML::XElement(ns.o + "shapedefaults",
					XML::XAttribute(ns.v + "ext", m_ext) +
					XML::XAttribute("spidmax", m_spidmax)
				);
		}

	} // namespace Settings
} // namespace OOX