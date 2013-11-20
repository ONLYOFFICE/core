
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "NullRun.h"


namespace OOX
{
	namespace Logic
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


		const NullRun& NullRun::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NullRun::fromXML(const XML::XNode& node)
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

	} // namespace Logic
} // namespace OOX