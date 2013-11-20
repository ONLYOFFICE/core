#pragma once
#ifndef OOX_LOGIC_FIELD_SIMPLE_INCLUDE_H_
#define OOX_LOGIC_FIELD_SIMPLE_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "Run.h"
#include "property.h"
#include <string>


namespace OOX
{
	namespace Logic
	{
		class FldSimple : public ParagraphItemBase
		{
		public:
			FldSimple();
			virtual ~FldSimple();
			explicit FldSimple(const XML::XNode& node);
			const FldSimple& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			property<Run>					Run;
			property<std::string> Instr;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FIELD_SIMPLE_INCLUDE_H_