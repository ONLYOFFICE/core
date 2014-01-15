#pragma once
#ifndef OOX_LOGIC_LINE_INCLUDE_H_
#define OOX_LOGIC_LINE_INCLUDE_H_

#include "Wrap.h"
#include "Shadow.h"
#include "TextBox.h"
#include "LineStyle.h"
#include "FillStyle.h"
#include "ShapeStyle.h"

namespace OOX
{
	namespace Logic
	{
		class Line : public WritingElement
		{
		public:
			Line();
			virtual ~Line();			
			explicit Line(const XML::XNode& node);
			const Line& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>				Id;		
			property<ShapeStyle>				style;

			nullable_property<Wrap>				Wrap;
			nullable_property<NullElement>		anchorlock;

			nullable_property<FillStyle>		fillstyle;
			nullable_property<LineStyle>		linestyle;
			nullable_property<Shadow>			shadow;

			nullable_property<std::string>		from;
			nullable_property<std::string>		to;

			nullable_property<TextBox>			textbox;		// none
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_LINE_INCLUDE_H_