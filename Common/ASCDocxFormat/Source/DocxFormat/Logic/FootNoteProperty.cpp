
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FootNoteProperty.h"


namespace OOX
{
	namespace Logic
	{

		FootNoteProperty::FootNoteProperty()
		{
		}


		FootNoteProperty::~FootNoteProperty()
		{
		}


		FootNoteProperty::FootNoteProperty(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FootNoteProperty& FootNoteProperty::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FootNoteProperty::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			if(element.element("numFmt").exist())
				NumFormat = element.element("numFmt");
			else 
				NumFormat->setDecimal();

			if(element.element("numStart").exist())
				NumStart =  element.element("numStart").attribute("val").value();
			else 
				NumStart = 1;
		}


		const XML::XNode FootNoteProperty::toXML() const
		{
			return 
				XML::XElement(ns.w + "footnotePr",
					XML::Write(NumFormat) +
					XML::Write(ns.w + "numStart", ns.w + "val", NumStart)
				);
		}

	} // namespace Logic
} // namespace OOX