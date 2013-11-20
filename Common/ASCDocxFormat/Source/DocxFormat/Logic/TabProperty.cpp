
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TabProperty.h"


namespace OOX
{
	namespace Logic
	{

		TabProperty::TabProperty()
		{
		}


		TabProperty::~TabProperty()
		{
		}
	

		TabProperty::TabProperty(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TabProperty& TabProperty::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TabProperty::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Val		 = element.attribute("val").value();
			Leader = element.attribute("leader").value();
			//Pos		 = element.attribute("pos").value().ToString();
			if (element.attribute("pos").exist())
			{
				int pos = element.attribute("pos").value();
				UniversalUnit value(pos, UniversalUnit::Dx);
				Pos = value;
			}
		}


		const XML::XNode TabProperty::toXML() const
		{
			return 
				XML::XElement(ns.w + "tab",
					XML::XAttribute(ns.w + "val", Val) +
					XML::XAttribute(ns.w + "leader", Leader) +
					XML::XAttribute(ns.w + "pos", Pos)
				);
		}
	} // namespace Logic
} // namespace OOX