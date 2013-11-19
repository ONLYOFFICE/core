#pragma once
#ifndef PPTX_LOGIC_FONTCOLLECTION_INCLUDE_H_
#define PPTX_LOGIC_FONTCOLLECTION_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "TextFont.h"
#include "SupplementalFont.h"
#include <list>

namespace PPTX
{
	namespace Logic
	{

		class FontCollection : public WrapperWritingElement
		{
		public:
			FontCollection();
			virtual ~FontCollection();
			explicit FontCollection(const XML::XNode& node);
			const FontCollection& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<TextFont> latin;
			property<TextFont> ea;
			property<TextFont> cs;
			property<std::list<SupplementalFont> > Fonts;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FONTCOLLECTION_INCLUDE_H_