
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EndnotePr.h"


namespace OOX
{
	namespace Settings
	{

		EndnotePr::EndnotePr()
		{
		}


		EndnotePr::~EndnotePr()
		{
		}


		EndnotePr::EndnotePr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EndnotePr& EndnotePr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void EndnotePr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_endnotes, element, "endnote");
		}


		const XML::XNode EndnotePr::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Settings
} // namespace OOX