#include "ClrChange.h"

namespace PPTX
{
	namespace Logic
	{
		ClrChange::ClrChange()
		{
		}

		ClrChange::~ClrChange()
		{
		}

		ClrChange::ClrChange(const XML::XNode& node)
		{
			fromXML(node);
		}

		const ClrChange& ClrChange::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void ClrChange::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			ClrFrom.GetColorFrom(element.element("clrFrom"));
			ClrTo.GetColorFrom(element.element("clrTo"));
			useA = element.attribute("useA").value();
			FillParentPointersForChilds();
		}

		const XML::XNode ClrChange::toXML() const
		{
			return XML::XElement(ns.a + "clrChange",
					XML::XElement(ns.a + "clrFrom", XML::Write(ClrFrom)) +
					XML::XElement(ns.a + "clrTo", XML::Write(ClrTo)) +
					XML::XAttribute("useA", useA)
				);
		}

		void ClrChange::FillParentPointersForChilds()
		{
			ClrFrom.SetParentPointer(*this);
			ClrTo.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX