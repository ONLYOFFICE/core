#include "LineEnd.h"

namespace PPTX
{
	namespace Logic
	{

		LineEnd::LineEnd()
		{
		}


		LineEnd::~LineEnd()
		{
		}


		LineEnd::LineEnd(const XML::XNode& node)
		{
			fromXML(node);
		}


		const LineEnd& LineEnd::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void LineEnd::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;
			
			type = element.attribute("type").value();
			w = element.attribute("w").value();
			len = element.attribute("len").value();
		}


		const XML::XNode LineEnd::toXML() const
		{
			return XML::XElement(m_namespace + m_name.get(),
					XML::XAttribute("type", type) +
					XML::XAttribute("w", w) +
					XML::XAttribute("len", len)
				);
		}

		void LineEnd::Merge(nullable_property<LineEnd>& line)const
		{
			if(!line.is_init())
				line = LineEnd();

			if(type.is_init())
				line->type = *type;
			if(w.is_init())
				line->w = *w;
			if(len.is_init())
				line->len = *len;
		}
	} // namespace Logic
} // namespace PPTX