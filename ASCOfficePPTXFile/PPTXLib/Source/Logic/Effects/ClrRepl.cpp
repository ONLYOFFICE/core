#include "ClrRepl.h"

namespace PPTX
{
	namespace Logic
	{
		ClrRepl::ClrRepl()
		{
		}

		ClrRepl::~ClrRepl()
		{
		}

		ClrRepl::ClrRepl(const XML::XNode& node)
		{
			fromXML(node);
		}

		const ClrRepl& ClrRepl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void ClrRepl::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Color.GetColorFrom(element);
			FillParentPointersForChilds();
		}

		const XML::XNode ClrRepl::toXML() const
		{
			return XML::XElement(ns.a + "clrRepl",
					XML::Write(Color)
				);
		}

		void ClrRepl::FillParentPointersForChilds()
		{
			Color.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX