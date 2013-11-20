
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "LnStyleLst.h"


namespace OOX
{
	namespace Theme
	{

		LnStyleLst::LnStyleLst()
		{
		}


		LnStyleLst::~LnStyleLst()
		{
		}
	

		LnStyleLst::LnStyleLst(const XML::XNode& node)
		{
			fromXML(node);
		}


		const LnStyleLst& LnStyleLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void LnStyleLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_lns, element);
		}


		const XML::XNode LnStyleLst::toXML() const
		{
			return 
				XML::XElement(ns.a + "lnStyleLst", 
					XML::Write(m_lns)
				);
		}

	} // namespace Theme
} // namespace OOX