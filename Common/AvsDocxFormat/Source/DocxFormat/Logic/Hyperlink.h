#pragma once
#ifndef OOX_LOGIC_HYPERLINK_INCLUDE_H_
#define OOX_LOGIC_HYPERLINK_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "Run.h"
#include "property.h"
#include "nullable_property.h"
#include "./../RId.h"
#include <string>
#include <vector>


namespace OOX
{
	namespace Logic
	{
		class Hyperlink : public ParagraphItemBase
		{
		public:
			Hyperlink();
			virtual ~Hyperlink();
			explicit Hyperlink(const XML::XNode& node);
			const Hyperlink& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			nullable_property<RId>				 rId;
			property<std::vector<Run> >			 Runs;			
			nullable_property<std::string>		 Anchor;
			nullable_property<int>				 History;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_HYPERLINK_INCLUDE_H_