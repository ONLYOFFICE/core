
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ShapeLayout.h"


namespace OOX
{
	namespace Settings
	{

		ShapeLayout::ShapeLayout()
		{
		}


		ShapeLayout::~ShapeLayout()
		{
		}


		ShapeLayout::ShapeLayout(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ShapeLayout& ShapeLayout::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ShapeLayout::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_ext		=	element.attribute("ext").value();
			m_idMap		=	element.element("idmap");
			m_rules		=	element.element("rules");
		}


		const XML::XNode ShapeLayout::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Settings
} // namespace OOX