
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SdtEndProperty.h"


namespace OOX
{
	namespace Logic
	{

		SdtEndProperty::SdtEndProperty()
		{
		}


		SdtEndProperty::~SdtEndProperty()
		{
		}
	

		SdtEndProperty::SdtEndProperty(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SdtEndProperty& SdtEndProperty::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SdtEndProperty::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			RunProperty		= element.element("rPr");
			Id						= element.element("id").attribute("val").value();
			Temporary		  = element.attribute("temporary").exist();
			ShowingPlcHdr = element.attribute("showingPlcHdr").exist();;
		}


		const XML::XNode SdtEndProperty::toXML() const
		{
			return 
				XML::XElement(ns.w + "sdtEndPr",
					XML::Write(RunProperty) +
					XML::Write(ns.w + "id", ns.w + "val", Id) +
					XML::WriteIf(XML::XElement(ns.w + "temporary"), Temporary) +
					XML::WriteIf(XML::XElement(ns.w + "showingPlcHdr"), ShowingPlcHdr)
				);
		}

	} // namespace Logic
} // namespace OOX