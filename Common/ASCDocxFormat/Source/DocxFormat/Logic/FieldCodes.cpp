
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FieldCodes.h"


namespace OOX
{
	namespace Logic
	{

		FieldCodes::FieldCodes()
		{
		}


		FieldCodes::~FieldCodes()
		{
		}
	

		FieldCodes::FieldCodes(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FieldCodes& FieldCodes::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FieldCodes::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Text = element.text();
		}


		const XML::XNode FieldCodes::toXML() const
		{
			return XML::XElement(ns.o + "FieldCodes", XML::XText(Text));
		}

	} // namespace Logic
} // namespace OOX