#pragma once
#ifndef PPTX_LOGIC_AHXY_INCLUDE_H_
#define PPTX_LOGIC_AHXY_INCLUDE_H_

#include "Ah.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class AhXY : public Ah
		{
		public:
			AhXY();
			virtual ~AhXY();
			explicit AhXY(const XML::XNode& node);
			const AhXY& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int> x;
			property<int> y;

			nullable_property<std::string> gdRefX;
			nullable_property<std::string> gdRefY;
			nullable_property<std::string> maxX;
			nullable_property<std::string> maxY;
			nullable_property<std::string> minX;
			nullable_property<std::string> minY;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			//virtual std::string GetODString()const;
			virtual void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AHXY_INCLUDE_H_