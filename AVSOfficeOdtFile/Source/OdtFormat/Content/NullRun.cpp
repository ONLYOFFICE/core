
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "NullRun.h"


namespace Odt
{
	namespace Content
	{

		NullRun::NullRun()
		{
		}

		
		NullRun::~NullRun()
		{
		}
		
		
		NullRun::NullRun(const XML::XNode& node)
		{
			fromXML(node);
		}

		
		NullRun::NullRun(const std::string& text)
		{
			fromTxt(text);
		}
		
		
		const NullRun& NullRun::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}
		
		
		const NullRun& NullRun::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void NullRun::fromXML(const XML::XNode& node)
		{
		}

		
		void NullRun::fromTxt(const std::string& text)
		{
		}

		
		const XML::XNode NullRun::toXML() const
		{
			return XML::XNode();
		}

		
		const std::string NullRun::toTxt() const
		{
			return "";
		}

	} // namespace Content
} // namespace Odt