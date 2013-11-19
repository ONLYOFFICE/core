#include "NotesTextViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		NotesTextViewPr::NotesTextViewPr()
		{
		}

		NotesTextViewPr::~NotesTextViewPr()
		{
		}

		NotesTextViewPr::NotesTextViewPr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const NotesTextViewPr& NotesTextViewPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void NotesTextViewPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			CViewPr = element.element("cViewPr");

			FillParentPointersForChilds();
		}

		const XML::XNode NotesTextViewPr::toXML() const
		{
			return	XML::XElement(ns.p + "notesTextViewPr",
				XML::Write(CViewPr)
			);
		}

		void NotesTextViewPr::FillParentPointersForChilds()
		{
			CViewPr->SetParentPointer(*this);
		}
	} // namespace nsViewProps
} // namespace PPTX