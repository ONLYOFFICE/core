
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ExtraClrSchemeLst.h"


namespace OOX
{
	namespace Theme
	{

		ExtraClrSchemeLst::ExtraClrSchemeLst()
		{
		}


		ExtraClrSchemeLst::~ExtraClrSchemeLst()
		{
		}


		ExtraClrSchemeLst::ExtraClrSchemeLst(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ExtraClrSchemeLst& ExtraClrSchemeLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ExtraClrSchemeLst::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode ExtraClrSchemeLst::toXML() const
		{
			return 
				XML::XElement(ns.a + "extraClrSchemeLst");
		}

	} // namespace Theme
} // namespace OOX