#include "Close.h"

namespace PPTX
{
	namespace Logic
	{

		Close::Close()
		{
		}

		Close::~Close()
		{
		}

		Close::Close(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Close& Close::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Close::fromXML(const XML::XNode& node)
		{
//			const XML::XElement element(node);
		}

		const XML::XNode Close::toXML() const
		{
			 return XML::XElement(ns.a + "close");
		}

		//std::string Close::GetODString()const
		void Close::GetODString(std::list<std::string>& strList)const
		{
			//return "<close/>";
			strList.push_back("<close/>");
		}
	} // namespace Logic
} // namespace PPTX