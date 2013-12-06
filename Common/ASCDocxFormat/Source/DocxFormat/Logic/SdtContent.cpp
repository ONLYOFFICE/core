
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SdtContent.h"

namespace OOX
{
	namespace Logic
	{
		SdtContent::SdtContent()
		{
		}

		SdtContent::~SdtContent()
		{
		}	

		SdtContent::SdtContent(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SdtContent& SdtContent::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SdtContent::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "p", "tbl");
		}

		const XML::XNode SdtContent::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX