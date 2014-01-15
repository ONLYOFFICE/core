#pragma once
#ifndef OOX_LOGIC_GROUP_INCLUDE_H_
#define OOX_LOGIC_GROUP_INCLUDE_H_

#include <string>

#include "./../WritingElement.h"
#include "ShapeStyle.h"
#include "property.h"
#include "nullable_property.h"
#include "IItemable.h"

namespace OOX
{
	namespace Logic
	{
		class Group;
		class GroupItem : public WritingElement, public IItemable<WritingElement>
		{
		public:
			GroupItem();
			GroupItem(const Oval& elem);
			GroupItem(const Rect& elem);
			GroupItem(const Roundrect& elem);
			GroupItem(const ShapeType& elem);
			GroupItem(const Shape& elem);
			GroupItem(const Line& elem);
			GroupItem(const Group& elem);

			virtual ~GroupItem();
			explicit GroupItem(const XML::XNode& node);
			const GroupItem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		};

		class Group : public WritingElement
		{
		public:
			Group();
			virtual ~Group();
			explicit Group(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>				id;
			property<std::string>				spid;
			property<ShapeStyle>				style;
			property<std::string>				coordorigin;
			property<std::string>				coordsize;

			property<std::vector<GroupItem>>	items;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_GROUP_INCLUDE_H_