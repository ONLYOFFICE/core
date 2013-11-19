#include "NotesViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		NotesViewPr::NotesViewPr()
		{
		}

		NotesViewPr::~NotesViewPr()
		{
		}

		NotesViewPr::NotesViewPr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const NotesViewPr& NotesViewPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void NotesViewPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			CSldViewPr = element.element("cSldViewPr");

			FillParentPointersForChilds();
		}

		const XML::XNode NotesViewPr::toXML() const
		{
			return	XML::XElement(ns.p + "notesViewPr",
				XML::Write(CSldViewPr)
			);
		}

		void NotesViewPr::FillParentPointersForChilds()
		{
			CSldViewPr->SetParentPointer(*this);
		}
	} // namespace nsViewProps
} // namespace PPTX