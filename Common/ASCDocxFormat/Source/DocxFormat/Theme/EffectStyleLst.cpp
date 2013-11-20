
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EffectStyleLst.h"


namespace OOX
{
	namespace Theme
	{

		EffectStyleLst::EffectStyleLst()
		{
		}


		EffectStyleLst::~EffectStyleLst()
		{
		}
	

		EffectStyleLst::EffectStyleLst(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EffectStyleLst& EffectStyleLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}
	

		void EffectStyleLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_effectStyles, element);
		}


		const XML::XNode EffectStyleLst::toXML() const
		{
			return 
				XML::XElement(ns.a + "effectStyleLst", 
					XML::Write(m_effectStyles)
				);
		}

	} // namespace Theme
} // namespace OOX