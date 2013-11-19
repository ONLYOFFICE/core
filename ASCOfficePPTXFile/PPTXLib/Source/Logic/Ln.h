#pragma once
#ifndef PPTX_LOGIC_LN_INCLUDE_H_
#define PPTX_LOGIC_LN_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/PenAlign.h"
#include "./../Limit/LineCap.h"
#include "./../Limit/CompoundLine.h"
#include "UniFill.h"
#include "PrstDash.h"
#include "LineEnd.h"
#include "LineJoin.h"

namespace PPTX
{
	namespace Logic
	{
		class Ln : public WrapperWritingElement
		{
		public:
			Ln();
			virtual ~Ln();
			explicit Ln(const XML::XNode& node);
			const Ln& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			//const Ln MergeWith(const Ln& line)const;
			void Merge(nullable_property<Ln>& line)const;
			void Merge(Ln& line)const;

		public:
			UniFill Fill;
			nullable_property<PrstDash> prstDash;
			//custDash (Custom Dash)  §20.1.8.21 
			LineJoin Join;
			nullable_property<LineEnd> headEnd;
			nullable_property<LineEnd> tailEnd;

			nullable_property<std::string, Limit::PenAlign> algn;
			nullable_property<std::string, Limit::LineCap> cap;
			nullable_property<std::string, Limit::CompoundLine> cmpd;
			nullable_property<int, setter::between<int, 0, 20116800> > w;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LN_INCLUDE_H_