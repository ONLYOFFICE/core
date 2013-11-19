#pragma once
#ifndef PPTX_VIEWPROPS_NOTES_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_NOTES_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CSldViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class NotesViewPr : public WrapperWritingElement
		{
		public:
			NotesViewPr();
			virtual ~NotesViewPr();
			explicit NotesViewPr(const XML::XNode& node);
			const NotesViewPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<nsViewProps::CSldViewPr> CSldViewPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_NOTES_VIEW_PROPERTIES_INCLUDE_H_