#pragma once
#ifndef PPTX_LOGIC_TIMING_INCLUDE_H_
#define PPTX_LOGIC_TIMING_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class Timing : public WrapperWritingElement
		{
		public:
			Timing();
			virtual ~Timing();
			explicit Timing(const XmlUtils::CXmlNode& node);
			const Timing& operator =(const XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(const XmlUtils::CXmlNode& node);
			virtual CString toXML() const;
		public:
			//tnLst (Time Node List)  §19.5.87 
			//bldLst (Build List)  §19.5.14 
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TIMING_INCLUDE_H_