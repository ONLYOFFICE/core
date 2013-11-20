
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FillStyleLst.h"


namespace OOX
{
	namespace Theme
	{

		FillStyleLst::FillStyleLst()
		{
		}


		FillStyleLst::~FillStyleLst()
		{
		}
	

		FillStyleLst::FillStyleLst(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FillStyleLst& FillStyleLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FillStyleLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_solidFill = element.element("solidFill");
			XML::Fill(m_gradFills, element, "gradFill");
		}


		const XML::XNode FillStyleLst::toXML() const
		{
			return 
				XML::XElement(ns.a + "fillStyleLst",
					XML::Write(m_solidFill) +
					XML::Write(m_gradFills)
				);
		}

	} // namespace Theme
} // namespace OOX