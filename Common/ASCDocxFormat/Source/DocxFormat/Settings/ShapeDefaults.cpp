
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ShapeDefaults.h"


namespace OOX
{
	namespace Settings
	{

		ShapeDefaults::ShapeDefaults()
		{
		}


		ShapeDefaults::~ShapeDefaults()
		{
		}


		ShapeDefaults::ShapeDefaults(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ShapeDefaults& ShapeDefaults::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ShapeDefaults::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_shapeDefault = element.element("shapedefaults");
			m_shapeLayout	=	element.element("shapelayout");
		}


		const XML::XNode ShapeDefaults::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Settings
} // namespace OOX