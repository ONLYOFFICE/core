#include "Par.h"

namespace PPTX
{
	namespace Logic
	{

		Par::Par()
		{
		}

		Par::~Par()
		{
		}

		Par::Par(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Par& Par::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Par::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cTn = element.element("cTn");
			FillParentPointersForChilds();
		}

		const XML::XNode Par::toXML() const
		{
			return XML::XElement(ns.p + "par",
				XML::Write(cTn)
			);
		}

		void Par::FillParentPointersForChilds()
		{
			cTn->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX