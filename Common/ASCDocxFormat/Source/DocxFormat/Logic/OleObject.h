#pragma once
#ifndef OOX_LOGIC_OLE_OBJECT_INCLUDE_H_
#define OOX_LOGIC_OLE_OBJECT_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <string>
#include "FieldCodes.h"
#include "nullable.h"
#include "property.h"
#include "./../RId.h"


namespace OOX
{
	namespace Logic
	{
		class OleObject : public WritingElement
		{
		public:
			OleObject();
			virtual ~OleObject();			
			OleObject(const XML::XNode& node);
			const OleObject& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>		Type;
			property<std::string>		ProgId;			
			property<std::string>		ShapeId;		
			property<std::string>		DrawAspect;
			property<std::string>		ObjectId;		
			property<OOX::RId>			rId;
			property<std::string>		TargetId;
			nullable_property<FieldCodes>  FieldCodes;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_OLE_OBJECT_INCLUDE_H_