#pragma once
#ifndef PPTX_LOGIC_MOVETO_INCLUDE_H_
#define PPTX_LOGIC_MOVETO_INCLUDE_H_

#include "PathBase.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class MoveTo : public PathBase
		{
		public:
			MoveTo();
			virtual ~MoveTo();
			explicit MoveTo(const XML::XNode& node);
			const MoveTo& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int> x;
			property<int> y;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			//std::string GetODString()const;
			void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_MOVETO_INCLUDE_H_