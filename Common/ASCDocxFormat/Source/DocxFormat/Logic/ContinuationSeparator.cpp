
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ContinuationSeparator.h"


namespace OOX
{
	namespace Logic
	{

		ContinuationSeparator::ContinuationSeparator()
		{
		}


		ContinuationSeparator::~ContinuationSeparator()
		{
		}
	

		ContinuationSeparator::ContinuationSeparator(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ContinuationSeparator& ContinuationSeparator::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ContinuationSeparator::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode ContinuationSeparator::toXML() const
		{
			return XML::XElement(ns.w + "continuationSeparator");
		}


		const std::string ContinuationSeparator::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX