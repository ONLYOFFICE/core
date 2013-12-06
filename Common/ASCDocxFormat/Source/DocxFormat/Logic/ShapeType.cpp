
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ShapeType.h"

namespace OOX
{
	namespace Logic
	{			
		ShapeType::ShapeType()
		{

		}

		ShapeType::~ShapeType()
		{

		}

		ShapeType::ShapeType(const XML::XNode& node)
		{
			fromXML(node);
		}

		const ShapeType& ShapeType::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void ShapeType::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			Id				=	element.attribute("id").value();
			coordsize		=	element.attribute("coordsize").value();
			Spt				=	element.attribute("spt").value();
			path			=	element.attribute("path").value();
			Filled			=	element.attribute("filled").value();
			Stroked			=	element.attribute("stroked").value();	
			Stroke			=	element.element("stroke");
			formulas		=	element.element("formulas");
			Lock			=	element.element("lock");
			PathElement		=	element.element("path");	
			Handles			=	element.element("handles");
			TextPath		=	element.element("textpath");
			Complex			=	element.element("complex");
			adj				=	element.attribute("adj").value();
			Preferrelative	=	element.attribute("preferrelative").value();
		}

		const XML::XNode ShapeType::toXML() const
		{		
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX
