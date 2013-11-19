#include "AttrNameLst.h"

namespace PPTX
{
	namespace Logic
	{

		AttrNameLst::AttrNameLst()
		{
		}

		AttrNameLst::~AttrNameLst()
		{
		}

		AttrNameLst::AttrNameLst(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AttrNameLst& AttrNameLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AttrNameLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			XML::Fill(list, element, "attrName");
			FillParentPointersForChilds();
		}

		const XML::XNode AttrNameLst::toXML() const
		{
			return XML::XElement(ns.p + "attrNameLst",
					XML::Write(list)
				);
		}
	} // namespace Logic
} // namespace PPTX