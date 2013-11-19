#pragma once
#ifndef PPTX_LOGIC_QUADBEZTO_INCLUDE_H_
#define PPTX_LOGIC_QUADBEZTO_INCLUDE_H_

#include "PathBase.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class QuadBezTo : public PathBase
		{
		public:
			QuadBezTo();
			virtual ~QuadBezTo();
			explicit QuadBezTo(const XML::XNode& node);
			const QuadBezTo& operator =(const XML::XNode& node);
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int> x1;
			property<int> y1;
			property<int> x2;
			property<int> y2;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			//std::string GetODString()const;
			virtual void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_QUADBEZTO_INCLUDE_H_