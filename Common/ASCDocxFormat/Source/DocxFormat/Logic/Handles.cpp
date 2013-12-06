
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Handles.h"


namespace OOX
{
	namespace Logic
	{

		Handles::Handles()
		{
		}


		Handles::~Handles()
		{
		}


		Handles::Handles(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Handles& Handles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Handles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Fill(Hs, element, "h");		
		}


		const XML::XNode Handles::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX