#pragma once
#ifndef PPTX_PRESENTATION_NOTESSZ_INCLUDE_H_
#define PPTX_PRESENTATION_NOTESSZ_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class NotesSz : public WrapperWritingElement
		{
		public:
			NotesSz();
			virtual ~NotesSz();
			explicit NotesSz(const XML::XNode& node);
			const NotesSz& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int, setter::only_positive<int> > cx;
			property<int, setter::only_positive<int> > cy;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_NOTESSZ_INCLUDE_H_