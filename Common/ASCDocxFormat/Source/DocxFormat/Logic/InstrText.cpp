
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "InstrText.h"


namespace OOX
{
	namespace Logic
	{

		InstrText::InstrText()
		{
		}


		InstrText::~InstrText()
		{
		}
	

		InstrText::InstrText(const XML::XNode& node)
		{
			fromXML(node);
		}


		const InstrText& InstrText::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void InstrText::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Text = element.text();
		}


		const XML::XNode InstrText::toXML() const
		{
		return XML::XElement();
		}


		const std::string InstrText::toTxt() const
		{					
			return Text;
		}

	} // namespace Logic
} // namespace OOX