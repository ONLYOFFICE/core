#pragma once
#ifndef OOX_LOGIC_EXTRUSION_INCLUDE_H_
#define OOX_LOGIC_EXTRUSION_INCLUDE_H_

#include "./../WritingElement.h"
#include "./../Limit/TrueFalse.h"
#include <string>
#include "nullable_property.h"


namespace OOX
{
	namespace Logic
	{
		class Extrusion : public WritingElement
		{
		public:
			Extrusion();
			virtual ~Extrusion();			
			explicit Extrusion(const XML::XNode& node);
			const Extrusion& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			
			nullable_property<std::string, Limit::TrueFalse>	On;
			nullable_property<std::string>						Ext;
			nullable_property<std::string>						BackDepth;
			nullable_property<std::string>						ViewPoint;	
			nullable_property<std::string>						ViewPointOrigin;
			nullable_property<std::string>						SkewAngle;	
			nullable_property<std::string>						LightPosition;
			nullable_property<std::string>						LightPosition2;	
			nullable_property<std::string>						Type;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_EXTRUSION_INCLUDE_H_