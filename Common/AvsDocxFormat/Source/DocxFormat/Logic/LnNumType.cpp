
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "LnNumType.h"

namespace OOX
{
	namespace Logic
	{
		LnNumType::LnNumType()
		{

		}

		LnNumType::~LnNumType()
		{

		}

		LnNumType::LnNumType(const XML::XNode& node)
		{
			fromXML(node);
		}

		const LnNumType& LnNumType::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void LnNumType::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			countBy		=	element.attribute("countBy").value();
			start		=	element.attribute("start").value();
			//distance	=	element.attribute("distance").value();
			restart		=	element.attribute("restart").value();
		}

		const XML::XNode LnNumType::toXML() const
		{
			return XML::XElement(ns.w + "lnNumType",
				XML::XAttribute(ns.w + "countBy", countBy) +
				XML::XAttribute(ns.w + "start", start) +
				//XML::XAttribute(ns.w + "distance", distance) +
				XML::XAttribute(ns.w + "restart", restart) );
		}
	}
}