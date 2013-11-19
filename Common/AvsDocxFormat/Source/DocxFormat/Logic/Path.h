#pragma once
#ifndef OOX_LOGIC_PATH_INCLUDE_H_
#define OOX_LOGIC_PATH_INCLUDE_H_

#include "./../WritingElement.h"
#include "./../Limit/TrueFalse.h"
#include "./../Limit/ConnectType.h"
#include <string>
#include "property.h"


namespace OOX
{
	namespace Logic
	{
		class Path : public WritingElement
		{
		public:
			Path();
			virtual ~Path();			
			explicit Path(const XML::XNode& node);
			const Path& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			
			nullable_property<std::string, Limit::TrueFalse>	ArrowOk;
			nullable_property<std::string, Limit::TrueFalse>	ExtrusionOk;
			nullable_property<std::string, Limit::TrueFalse>	GradientShapeOk;
			nullable_property<std::string, Limit::TrueFalse>	TextPathOk;
			nullable_property<std::string, Limit::ConnectType>	ConnectType;
			nullable_property<std::string>						ConnectLocs;
			nullable_property<std::string>						TextBoxRect;
			nullable_property<std::string>						ConnectAngles;	
			nullable_property<std::string>						Limo;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PATH_INCLUDE_H_