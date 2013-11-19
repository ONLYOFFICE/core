#pragma once
#ifndef PPTX_LOGIC_ARCTO_INCLUDE_H_
#define PPTX_LOGIC_ARCTO_INCLUDE_H_

#include "PathBase.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class ArcTo : public PathBase
		{
		public:
			ArcTo();
			virtual ~ArcTo();
			explicit ArcTo(const XML::XNode& node);
			const ArcTo& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> wR;
			property<std::string> hR;
			property<std::string> stAng;
			property<std::string> swAng;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			//std::string GetODString()const;
			void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ARCTO_INCLUDE_H_