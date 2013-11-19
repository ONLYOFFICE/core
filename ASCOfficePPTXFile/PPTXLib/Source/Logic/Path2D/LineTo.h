#pragma once
#ifndef PPTX_LOGIC_LINETO_INCLUDE_H_
#define PPTX_LOGIC_LINETO_INCLUDE_H_

#include "PathBase.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class LineTo : public PathBase
		{
		public:
			LineTo();
			virtual ~LineTo();
			explicit LineTo(const XML::XNode& node);
			const LineTo& operator =(const XML::XNode& node);

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

#endif // PPTX_LOGIC_LINETO_INCLUDE_H_