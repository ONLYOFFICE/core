#include "LineJoin.h"

namespace PPTX
{
	namespace Logic
	{

		LineJoin::LineJoin()
		{
			type = JoinEmpty;
		}


		LineJoin::~LineJoin()
		{
		}


		LineJoin::LineJoin(const XML::XNode& node)
		{
			fromXML(node);
		}


		const LineJoin& LineJoin::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void LineJoin::GetJoinFrom(const XML::XElement& element)
		{
			type = JoinEmpty;

			if(element.element("round").exist())
				type = JoinRound;
			else if(element.element("bevel").exist())
				type = JoinBevel;
			else if(element.element("miter").exist())
			{
				type = JoinMiter;
				lim = element.element("miter").attribute("lim").value();
			}
		}

		void LineJoin::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			type = JoinEmpty;

			if(element->XName == "round")
				type = JoinRound;
			else if(element->XName == "bevel")
				type = JoinBevel;
			else if(element->XName == "miter")
			{
				type = JoinMiter;
				lim = element.attribute("lim").value();
			}
		}


		const XML::XNode LineJoin::toXML() const
		{
			XML::XNode node;
			if(type == JoinRound)
				return XML::XElement(ns.a + "round");
			else if(type == JoinBevel)
				return XML::XElement(ns.a + "bevel");
			else if(type == JoinMiter)
				return XML::XElement(ns.a + "miter", XML::XAttribute("lim", lim));
			return node;
		}

	} // namespace Logic
} // namespace PPTX