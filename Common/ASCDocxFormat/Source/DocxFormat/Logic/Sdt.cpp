
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Sdt.h"


namespace OOX
{
	namespace Logic
	{

		Sdt::Sdt()
		{
		}


		Sdt::~Sdt()
		{
		}
	

		Sdt::Sdt(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Sdt& Sdt::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Sdt::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Property		= element.element("sdtPr");
			EndProperty	= element.element("sdtEndPr");
			Content			= element.element("sdtContent");		
		}


		const XML::XNode Sdt::toXML() const
		{
		return XML::XElement();
		}		
	} // namespace Logic
} // namespace OOX