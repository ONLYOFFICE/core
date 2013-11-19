
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "GsLst.h"


namespace OOX
{
	namespace Theme
	{

		GsLst::GsLst()
		{
		}


		GsLst::~GsLst()
		{
		}
	

		GsLst::GsLst(const XML::XNode& node)
		{
			fromXML(node);
		}


		const GsLst& GsLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void GsLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_gss, element);
		}


		const XML::XNode GsLst::toXML() const
		{
			return 
				XML::XElement(ns.a + "gsLst", 
					XML::Write(m_gss)
				);
		}

	} // namespace Theme
} // namespace OOX