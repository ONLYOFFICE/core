#pragma once
#ifndef PPTX_LOGIC_ANIMMOTION_INCLUDE_H_
#define PPTX_LOGIC_ANIMMOTION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CBhvr.h"
#include "./../../Limit/TLOrigin.h"
#include "./../../Limit/TLPathEditMode.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimMotion : public WrapperWritingElement
		{
		public:
			AnimMotion();
			virtual ~AnimMotion();			
			explicit AnimMotion(const XML::XNode& node);
			const AnimMotion& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CBhvr> cBhvr;

			nullable_property<int> byX;
			nullable_property<int> byY;
			nullable_property<int> fromX;
			nullable_property<int> fromY;
			nullable_property<int> toX;
			nullable_property<int> toY;
			nullable_property<int> rCtrX;
			nullable_property<int> rCtrY;
//Attributes
			nullable_property<std::string, Limit::TLOrigin> origin; //ST_TLAnimateMotionBehaviorOrigin
			nullable_property<std::string> path; //M = move to, L = line to, C = curve to, Z=close loop, E=end 
										//UPPERCASE = absolute coords, lowercase = relative coords 
										//Thus total allowed set = {M,L,C,Z,E,m,l,c,z,e) 
										//W3C XML Schema string 
			nullable_property<std::string, Limit::TLPathEditMode> pathEditMode; //ST_TLAnimateMotionPathEditMode
			nullable_property<std::string> ptsTypes; //A = Auto, F = Corner, T = Straight, S = Smooth 
											//UPPERCASE = Straight Line follows point, lowercase = curve follows point. 
											//Thus, the total allowed set = {A,F,T,S,a,f,t,s} 
											//W3C XML Schema string 
			nullable_property<int> rAng;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMMOTION_INCLUDE_H