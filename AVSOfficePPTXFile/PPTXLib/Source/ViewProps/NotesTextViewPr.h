#pragma once
#ifndef PPTX_VIEWPROPS_NOTES_TEXT_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_NOTES_TEXT_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class NotesTextViewPr : public WrapperWritingElement
		{
		public:
			NotesTextViewPr();
			virtual ~NotesTextViewPr();
			explicit NotesTextViewPr(const XML::XNode& node);
			const NotesTextViewPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<nsViewProps::CViewPr> CViewPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_NOTES_TEXT_VIEW_PROPERTIES_INCLUDE_H_