
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ParagraphBorder.h"


namespace OOX
{
	namespace Logic
	{

		ParagraphBorder::ParagraphBorder()
		{
		}


		ParagraphBorder::~ParagraphBorder()
		{
		}
	

		ParagraphBorder::ParagraphBorder(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ParagraphBorder& ParagraphBorder::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ParagraphBorder::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Top			=	element.element("top");
			Bottom		=	element.element("bottom");
			Left		=	element.element("left");
			Right		=	element.element("right");			
		}


		const XML::XNode ParagraphBorder::toXML() const
		{
			return XML::XElement(ns.w + "pBdr",
						XML::Write(Top) +
						XML::Write(Bottom) +
						XML::Write(Left) +
						XML::Write(Right)
					);
		}

	} // namespace Logic
} // namespace OOX