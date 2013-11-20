#pragma once
#ifndef OOX_LOGIC_IND_INCLUDE_H_
#define OOX_LOGIC_IND_INCLUDE_H_

#include "./../WritingElement.h"
#include "nullable_property.h"
#include "./../Unit.h"


namespace OOX
{
	namespace Logic
	{
		class Ind : public WritingElement
		{
		public:
			Ind();
			virtual ~Ind();
			explicit Ind(const XML::XNode& node);
			const Ind& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool isSimple() const;

		public:
			nullable_property<UniversalUnit>		Left;
			nullable_property<UniversalUnit>		Right;
			nullable_property<UniversalUnit>		Hanging;
			nullable_property<UniversalUnit>		FirstLine;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_IND_INCLUDE_H_