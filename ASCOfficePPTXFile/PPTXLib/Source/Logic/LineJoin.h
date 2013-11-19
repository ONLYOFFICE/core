#pragma once
#ifndef PPTX_LOGIC_LINEJOIN_INCLUDE_H_
#define PPTX_LOGIC_LINEJOIN_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{
		enum eJoin {JoinEmpty, JoinRound, JoinBevel, JoinMiter};
	
		class LineJoin : public WrapperWritingElement
		{
		public:
			LineJoin();
			virtual ~LineJoin();
			explicit LineJoin(const XML::XNode& node);
			const LineJoin& operator =(const XML::XNode& node);
			void GetJoinFrom(const XML::XElement& element);
			virtual bool is_init()const{return (type==JoinEmpty);};

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<eJoin> type;
			nullable_property<int, setter::only_positive<int> > lim;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LINEJOIN_INCLUDE_H_