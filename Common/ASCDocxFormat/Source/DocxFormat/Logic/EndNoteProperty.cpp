
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EndNoteProperty.h"


namespace OOX
{
	namespace Logic
	{

		EndNoteProperty::EndNoteProperty()
		{
		}


		EndNoteProperty::~EndNoteProperty()
		{
		}


		EndNoteProperty::EndNoteProperty(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EndNoteProperty& EndNoteProperty::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EndNoteProperty::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if(element.element("numFmt").exist())
				NumFormat = element.element("numFmt");
			else 
				NumFormat->setLowerRoman();

			if(element.element("numStart").exist())
				NumStart =  element.element("numStart").attribute("val").value();
			else 
				NumStart = 1;
		}


		const XML::XNode EndNoteProperty::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX