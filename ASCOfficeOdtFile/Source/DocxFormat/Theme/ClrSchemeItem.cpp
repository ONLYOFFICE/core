
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ClrSchemeItem.h"
#include "SysClr.h"
#include "SrgbClr.h"
#include "./../FormatError.h"


namespace OOX
{
	namespace Theme
	{

		ClrSchemeItem::ClrSchemeItem()
		{
		}


		ClrSchemeItem::~ClrSchemeItem()
		{
		}
	

		ClrSchemeItem::ClrSchemeItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ClrSchemeItem& ClrSchemeItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ClrSchemeItem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			name = element.XName->Name;

			if (element.element("sysClr").exist())
				item->reset(new SysClr(element.element("sysClr")));
			else if (element.element("srgbClr").exist())
				item->reset(new SrgbClr(element.element("srgbClr")));
			else
				throw FormatError("unknow element " + *name);
		}


		const XML::XNode ClrSchemeItem::toXML() const
		{
			return XML::XElement(ns.a + *name, XML::Write(*item));
		}

	} // namespace Theme
} // namespace OOX