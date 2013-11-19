#pragma once
#ifndef OOX_LOGIC_SPACING_INCLUDE_H_
#define OOX_LOGIC_SPACING_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable_property.h"
#include "../Limit/LineRule.h"
#include "./../Unit.h"

namespace OOX
{
	namespace Logic
	{
		class Spacing : public WritingElement
		{
		public:
			Spacing();
			virtual ~Spacing();			
			explicit Spacing(const XML::XNode& node);
			const Spacing& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool isSimple() const;

		public:
			nullable_property<UniversalUnit>				After;
			nullable_property<UniversalUnit>				Before;
			nullable_property<UniversalUnit>				Line;
			
			nullable_property<int> AfterLines;
			nullable_property<int> BeforeLines;

			nullable_property<std::string, Limit::LineRule>	LineRule;

			nullable_property<bool>													BeforeAutospacing;
			nullable_property<bool>													AfterAutospacing;

		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SPACING_INCLUDE_H