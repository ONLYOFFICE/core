#include "CondLst.h"

namespace PPTX
{
	namespace Logic
	{

		CondLst::CondLst()
		{
		}

		CondLst::~CondLst()
		{
		}

		CondLst::CondLst(const XML::XNode& node)
		{
			fromXML(node);
		}

		const CondLst& CondLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CondLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			name = element.XName->Name;

			XML::Fill(list, element, "cond");
			FillParentPointersForChilds();
		}

		const XML::XNode CondLst::toXML() const
		{
			return XML::XElement(ns.p + name.get(),
					XML::Write(list)
				);
		}

		void CondLst::FillParentPointersForChilds()
		{
			for(std::list<Cond>::iterator i = list->begin(); i != list->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX