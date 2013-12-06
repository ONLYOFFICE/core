
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DelText.h"


namespace OOX
{
	namespace Logic
	{

		DelText::DelText()
		{
		}


		DelText::~DelText()
		{
		}
	

		DelText::DelText(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DelText& DelText::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DelText::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Text = element.text();
		}


		const XML::XNode DelText::toXML() const
		{
		return XML::XElement();
		}


		const std::string DelText::toTxt() const
		{					
			return Text;
		}

	} // namespace Logic
} // namespace OOX