#include "BldLst.h"

namespace PPTX
{
	namespace Logic
	{

		BldLst::BldLst()
		{
		}

		BldLst::~BldLst()
		{
		}

		BldLst::BldLst(const XML::XNode& node)
		{
			fromXML(node);
		}

		const BldLst& BldLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BldLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			XML::Fill(list, element);
			FillParentPointersForChilds();
		}

		const XML::XNode BldLst::toXML() const
		{
			return XML::XElement(ns.p + "bldLst",
					XML::Write(list)
				);
		}

		void BldLst::FillParentPointersForChilds()
		{
			for(std::list<BuildNodeBase>::iterator i = list->begin(); i != list->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX