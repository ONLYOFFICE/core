#pragma once
#ifndef OOX_LOGIC_LOCK_INCLUDE_H_
#define OOX_LOGIC_LOCK_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "./../Limit/TrueFalse.h"
#include "./../Limit/Ext.h"


namespace OOX
{
	namespace Logic
	{
		class Lock : public WritingElement
		{
		public:
			Lock();
			virtual ~Lock();			
			explicit Lock(const XML::XNode& node);
			const Lock& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::Ext>					Ext;
			nullable_property<std::string, Limit::TrueFalse>	Aspectratio;
			nullable_property<std::string, Limit::TrueFalse>	Text;
			nullable_property<std::string, Limit::TrueFalse>	ShapeType;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_LOCK_INCLUDE_H_