
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SdtProperty.h"


namespace OOX
{
	namespace Logic
	{

		SdtProperty::SdtProperty()
		{
		}


		SdtProperty::~SdtProperty()
		{
		}
	

		SdtProperty::SdtProperty(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SdtProperty& SdtProperty::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SdtProperty::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Id				 = element.element("id").attribute("val").value();
			DocPartObj = element.element("docPartObj");
		}


		const XML::XNode SdtProperty::toXML() const
		{
			return 
				XML::XElement(ns.w + "sdtPr",
					XML::Write(ns.w + "id", ns.w + "val", Id) + 
					XML::Write(DocPartObj)
				);
		}

	} // namespace Logic
} // namespace OOX