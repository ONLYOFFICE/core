#pragma once
#ifndef OOX_LOGIC_TEXT_PATH_INCLUDE_H_
#define OOX_LOGIC_TEXT_PATH_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/TrueFalse.h"


namespace OOX
{
	namespace Logic
	{
		class TextPath : public WritingElement
		{
		public:
			TextPath();
			virtual ~TextPath();			
			explicit TextPath(const XML::XNode& node);
			const TextPath& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:			
			nullable_property<std::string, Limit::TrueFalse>	FitShape;
			nullable_property<std::string>						Style; 
			nullable_property<std::string, Limit::TrueFalse>	On;
			nullable_property<std::string, Limit::TrueFalse>	Trim;
			nullable_property<std::string, Limit::TrueFalse>	FitPath;
			nullable_property<std::string>						String;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TEXT_PATH_INCLUDE_H_