#pragma once
#ifndef OOX_LOGIC_PICT_INCLUDE_H_
#define OOX_LOGIC_PICT_INCLUDE_H_

#include "RunItemBase.h"
#include "Shape.h"
#include "ShapeType.h"
#include "Rect.h"
#include "Oval.h"
#include "Line.h"
#include "Roundrect.h"
#include "OleObject.h"
#include "Group.h"
#include "./../Limit/PictName.h"

namespace OOX
{
	namespace Logic
	{
		class Pict : public RunItemBase
		{
		public:
			Pict();
			virtual ~Pict();
			explicit Pict(const XML::XNode& node);
			
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			const std::string toTxt() const;

		public:
			const bool hasPictures() const;
			const bool isOle() const;

		public:

			property<std::string, Limit::PictName>	name;

			nullable_property<Shape>				shape;
			nullable_property<ShapeType>			shapetype;
			nullable_property<Rect>					rect;
			nullable_property<Oval>					oval;
			nullable_property<OOX::Logic::Line>		line;
			nullable_property<Roundrect>			roundrect;
			nullable_property<Group>				group;
			
			nullable_property<std::string>			DxaOrig;
			nullable_property<std::string>			DyaOrig;
			nullable_property<OleObject>			OleObject;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PICT_INCLUDE_H_