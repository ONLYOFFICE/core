
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Separator.h"


namespace OOX
{
	namespace Logic
	{

		Separator::Separator()
		{
		}


		Separator::~Separator()
		{
		}
	

		Separator::Separator(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Separator& Separator::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Separator::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode Separator::toXML() const
		{
		return XML::XElement();
		}


		const std::string Separator::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX