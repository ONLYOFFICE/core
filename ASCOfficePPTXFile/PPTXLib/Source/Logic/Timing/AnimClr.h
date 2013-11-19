#pragma once
#ifndef PPTX_LOGIC_ANIMCLR_INCLUDE_H_
#define PPTX_LOGIC_ANIMCLR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CBhvr.h"
#include "./../UniColor.h"
#include "./../../Limit/TLColorSpace.h"
#include "./../../Limit/TLColorDirection.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimClr : public WrapperWritingElement
		{
		public:
			AnimClr();
			virtual ~AnimClr();			
			explicit AnimClr(const XML::XNode& node);
			const AnimClr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CBhvr> cBhvr;

			nullable_property<int> byR;
			nullable_property<int> byG;
			nullable_property<int> byB;
			nullable_property<int> byH;
			nullable_property<int> byS;
			nullable_property<int> byL;
			UniColor from;
			UniColor to;
//Attributes
			nullable_property<std::string, Limit::TLColorSpace> clrSpc; //ST_TLAnimateColorSpace 
			nullable_property<std::string, Limit::TLColorDirection> dir; //ST_TLAnimateColorDirection 
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMCLR_INCLUDE_H