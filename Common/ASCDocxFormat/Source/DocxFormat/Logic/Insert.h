#pragma once
#ifndef OOX_LOGIC_INSERT_INCLUDE_H_
#define OOX_LOGIC_INSERT_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "Run.h"
#include "property.h"
#include "nullable_property.h"
#include <vector>


namespace OOX
{
	namespace Logic
	{
		class Insert : public ParagraphItemBase
		{
		public:
			Insert();
			virtual ~Insert();
			explicit Insert(const XML::XNode& node);
			const Insert& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			nullable_property<int>               Id;
			nullable_property<std::string>       Author;
			nullable_property<std::string>       Date;
			property<std::vector<Run> > Runs;			
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_INSERT_INCLUDE_H_