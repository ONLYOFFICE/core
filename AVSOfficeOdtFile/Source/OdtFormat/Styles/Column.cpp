
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Column.h"


namespace Odt
{
	namespace Styles
	{

		Column::Column()
		{
		}


		Column::~Column()
		{
		}


		Column::Column(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Column& Column::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Column::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			RelWidth		= element.attribute("rel-width").value().ToString();
			StartIndent = element.attribute("start-indent").value().ToString();
			EndIndent   = element.attribute("end-indent").value().ToString();
		}


		const XML::XNode Column::toXML() const
		{
			return 
				XML::XElement(ns.style + "column",
					XML::XAttribute(ns.style + "rel-width", RelWidth) + 
					XML::XAttribute(ns.fo + "start-indent", StartIndent) + 
					XML::XAttribute(ns.fo + "end-indent", EndIndent)
				);
		}

	} // namespace Styles
} // namespace Odt