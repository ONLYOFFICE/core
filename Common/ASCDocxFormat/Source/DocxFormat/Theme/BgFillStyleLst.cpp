
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BgFillStyleLst.h"


namespace OOX
{
	namespace Theme
	{

		BgFillStyleLst::BgFillStyleLst()
		{
		}


		BgFillStyleLst::~BgFillStyleLst()
		{
		}


		BgFillStyleLst::BgFillStyleLst(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BgFillStyleLst& BgFillStyleLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BgFillStyleLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_solidFill = element.element("solidFill");
			XML::Fill(m_gradFills, element, "gradFill");
		}

	
		const XML::XNode BgFillStyleLst::toXML() const
		{
			return 
				XML::XElement(ns.a + "bgFillStyleLst",
					XML::Write(m_solidFill)	 + 
					XML::Write(m_gradFills)
				);
		}

	} // namespace Theme
} // namespace OOX