#include "Set.h"

namespace PPTX
{
	namespace Logic
	{

		Set::Set()
		{
		}

		Set::~Set()
		{
		}

		Set::Set(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Set& Set::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Set::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cBhvr = element.element("cBhvr");
			to = element.element("to");

			FillParentPointersForChilds();
		}

		const XML::XNode Set::toXML() const
		{
			return XML::XElement(ns.p + "set",
				XML::Write(cBhvr) +
				XML::Write(to)
				);
		}

		void Set::FillParentPointersForChilds()
		{
			cBhvr->SetParentPointer(*this);
			if(to.is_init())
				to->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX