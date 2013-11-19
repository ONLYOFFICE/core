#pragma once
#ifndef PPTX_LOGIC_AHPOLAR_INCLUDE_H_
#define PPTX_LOGIC_AHPOLAR_INCLUDE_H_

#include "Ah.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class AhPolar : public Ah
		{
		public:
			AhPolar();
			virtual ~AhPolar();
			explicit AhPolar(const XML::XNode& node);
			const AhPolar& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int> x;
			property<int> y;

			nullable_property<std::string> gdRefAng;
			nullable_property<std::string> gdRefR;
			nullable_property<std::string> maxAng;
			nullable_property<std::string> maxR;
			nullable_property<std::string> minAng;
			nullable_property<std::string> minR;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			//virtual std::string GetODString()const;
			virtual void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AHPOLAR_INCLUDE_H_