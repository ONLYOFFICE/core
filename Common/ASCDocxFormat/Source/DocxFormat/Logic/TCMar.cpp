
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TCMar.h"

namespace OOX
{
	namespace Logic
	{
		TCMar::TCMar()
		{

		}

		TCMar::~TCMar()
		{

		}	

		TCMar::TCMar(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TCMar& TCMar::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TCMar::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			Top			=	element.element("top");
			Left		=	element.element("left");
			Bottom		=	element.element("bottom");
			Right		=	element.element("right");
		}

		const XML::XNode TCMar::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX