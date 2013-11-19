#pragma once
#ifndef ODP_CONTENT_FRAME_INCLUDE_H_
#define ODP_CONTENT_FRAME_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "Utility.h"
#include "OleObject.h"
#include "Image.h"
#include "TextBox.h"
#include "OdtFormat/Limit/Class.h"
#include <string>
#include <windows.h>
#include "OdtFormat/Content/Table.h"

namespace Odp
{
	namespace Content
	{
		class Frame : public Odt::WritingElement
		{
		public:
			Frame();
			virtual ~Frame();
			explicit Frame(const XML::XNode& node);
			const Frame& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> draw_style_name;
			nullable_property<std::string> presentation_style_name;
			nullable_property<std::string> text_style_name;
			nullable_property<std::string, Odt::Limit::Class> Class;
			nullable_property<bool> placeholder;
			property<std::string> layer;
			property<UniversalUnit> height;
			property<UniversalUnit> width;
			nullable_property<UniversalUnit> x;
			nullable_property<UniversalUnit> y;
			nullable_property<std::string> id;

			nullable_property<OleObject> object;
			nullable_property<Image> image;
			nullable_property<TextBox> text_box;
			nullable_property<Odt::Content::Table> Table;
		public:
			void GetMmRect(RECT& rect)const;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_FRAME_INCLUDE_H_