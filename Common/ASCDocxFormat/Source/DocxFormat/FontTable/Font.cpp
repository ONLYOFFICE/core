
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../FontTable.h"


namespace OOX
{

	FontTable::Font::Font()
	{
	}


	FontTable::Font::~Font()
	{
	}


	FontTable::Font::Font(const XML::XNode& node)
	{
		fromXML(node);
	}


	const FontTable::Font& FontTable::Font::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}


	void FontTable::Font::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		Name	=	element.attribute("name").value();
		Panose1 =	element.element("panose1").attribute("val").value();
		Charset =	element.element("charset").attribute("val").value();
		Family	=	element.element("family").attribute("val").value();
		Pitch	=	element.element("pitch").attribute("val").value();
		Usb0	=	element.element("sig").attribute("usb0").value();
		Usb1	=	element.element("sig").attribute("usb1").value();
		Usb2	=	element.element("sig").attribute("usb2").value();
		Usb3	=	element.element("sig").attribute("usb3").value();
		Csb0	=	element.element("sig").attribute("csb0").value();
		Csb1	=	element.element("sig").attribute("csb1").value();
	}


	const XML::XNode FontTable::Font::toXML() const
	{
		return
			XML::XElement(ns.w + "font",
				XML::XAttribute(ns.w + "name", Name) +
				XML::XElement(ns.w + "panose1", XML::XAttribute(ns.w + "val", Panose1)) + 
				XML::XElement(ns.w + "charset", XML::XAttribute(ns.w + "val", Charset)) +
				XML::XElement(ns.w + "family", XML::XAttribute(ns.w + "val", Family)) +
				XML::XElement(ns.w + "pitch", XML::XAttribute(ns.w + "val", Pitch)) + 
				WriteIf(XML::XElement(ns.w + "sig", 
					XML::XAttribute(ns.w + "usb0", Usb0)+ 
					XML::XAttribute(ns.w + "usb1", Usb1) + 
					XML::XAttribute(ns.w + "usb2", Usb2) +
					XML::XAttribute(ns.w + "usb3", Usb3) + 
					XML::XAttribute(ns.w + "csb0", Csb0) + 
					XML::XAttribute(ns.w + "csb1", Csb1)),
					((Usb0.is_init()) || (Usb1.is_init()) || (Usb2.is_init()) || (Usb3.is_init()) || (Csb0.is_init()) || (Csb1.is_init()))
				)
			);
	}
	
} // namespace OOX