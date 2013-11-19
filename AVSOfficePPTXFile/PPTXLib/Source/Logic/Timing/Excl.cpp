#include "Excl.h"

namespace PPTX
{
	namespace Logic
	{

		Excl::Excl()
		{
		}

		Excl::~Excl()
		{
		}

		Excl::Excl(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Excl& Excl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Excl::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cTn = element.element("cTn");
			FillParentPointersForChilds();
		}

		const XML::XNode Excl::toXML() const
		{
			return XML::XElement(ns.p + "excl",
					XML::Write(cTn)
				);
		}

		void Excl::FillParentPointersForChilds()
		{
			cTn->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX