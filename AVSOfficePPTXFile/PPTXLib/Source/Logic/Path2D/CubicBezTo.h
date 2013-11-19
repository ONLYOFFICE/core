#pragma once
#ifndef PPTX_LOGIC_CUBICBEZTO_INCLUDE_H_
#define PPTX_LOGIC_CUBICBEZTO_INCLUDE_H_

#include "PathBase.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class CubicBezTo : public PathBase
		{
		public:
			CubicBezTo();
			virtual ~CubicBezTo();
			explicit CubicBezTo(const XML::XNode& node);
			const CubicBezTo& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int> x1;
			property<int> y1;
			property<int> x2;
			property<int> y2;
			property<int> x3;
			property<int> y3;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			//std::string GetODString()const;
			void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CUBICBEZTO_INCLUDE_H_