
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FootnotePr.h"


namespace OOX
{
	namespace Settings
	{

		FootnotePr::FootnotePr()
		{
		}


		FootnotePr::~FootnotePr()
		{
		}


		FootnotePr::FootnotePr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FootnotePr& FootnotePr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FootnotePr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_footnotes, element, "footnote");
		}


		const XML::XNode FootnotePr::toXML() const
		{
			return 
				XML::XElement(ns.m + "footnotePr",
					XML::Write(m_footnotes)					
				);
		}

	} // namespace Settings
} // namespace OOX