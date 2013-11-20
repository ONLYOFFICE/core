
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Columns.h"


namespace OOX
{
	namespace Logic
	{

		Columns::Columns()
		{
		}


		Columns::~Columns()
		{
		}
	

		Columns::Columns(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Columns& Columns::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Columns::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Num				 = element.attribute("num").value();
			if(element.attribute("space").exist())
			{
				int space = element.attribute("space").value().ToInt();
				Space = UniversalUnit(space, UniversalUnit::Dx);
			}
			EqualWidth = element.attribute("equalWidth").value();
			Sep				 = element.attribute("sep").value();
			Fill(Items, element, "col");
		}


		const XML::XNode Columns::toXML() const
		{
			return 
				XML::XElement(ns.w + "cols", 
					XML::XAttribute(ns.w + "num", Num) +
					XML::XAttribute(ns.w + "sep", Sep) +
					XML::XAttribute(ns.w + "space", Space) +
					XML::XAttribute(ns.w + "equalWidth", EqualWidth) +					
					XML::Write(Items)
				);
		}

	} // namespace Logic
} // namespace OOX