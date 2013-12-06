
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Lang.h"


namespace OOX
{
	namespace Logic
	{

		Lang::Lang()
		{
		}


		Lang::~Lang()
		{
		}
	

		Lang::Lang(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Lang& Lang::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Lang::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Value			=	element.attribute("val").value();
			EastAsia	=	element.attribute("eastAsia").value();
			Bidi			=	element.attribute("bidi").value();
		}


		const XML::XNode Lang::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX