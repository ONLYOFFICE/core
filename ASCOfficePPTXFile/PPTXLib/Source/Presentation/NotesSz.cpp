#include "NotesSz.h"

namespace PPTX
{
	namespace nsPresentation
	{
		NotesSz::NotesSz()
		{
		}

		NotesSz::~NotesSz()
		{
		}

		NotesSz::NotesSz(const XML::XNode& node)
		{
			fromXML(node);
		}

		const NotesSz& NotesSz::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void NotesSz::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cx = element.attribute("cx").value();
			cy = element.attribute("cy").value();
		}

		const XML::XNode NotesSz::toXML() const
		{
			return	XML::XElement(ns.p + "notesSz",
				XML::XAttribute("cx", cx) +
				XML::XAttribute("cy", cy)
			);
		}
	} // namespace nsPresentation
} // namespace PPTX