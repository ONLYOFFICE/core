#pragma once
#ifndef PPTX_PRESENTATION_EMBEDDEDFONTDATAID_INCLUDE_H_
#define PPTX_PRESENTATION_EMBEDDEDFONTDATAID_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class EmbeddedFontDataId : public WrapperWritingElement
		{
		public:
			EmbeddedFontDataId();
			virtual ~EmbeddedFontDataId();
			explicit EmbeddedFontDataId(const XML::XNode& node);
			const EmbeddedFontDataId& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> rid;
		//private:
		public:
			property<std::string> m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_EMBEDDEDFONTDATAID_INCLUDE_H_