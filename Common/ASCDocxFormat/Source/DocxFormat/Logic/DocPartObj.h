#pragma once
#ifndef OOX_LOGIC_DOC_PART_OBJ_INCLUDE_H_
#define OOX_LOGIC_DOC_PART_OBJ_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"


namespace OOX
{
	namespace Logic
	{
		class DocPartObj : public WritingElement
		{
		public:
			DocPartObj();
			virtual ~DocPartObj();
			explicit DocPartObj(const XML::XNode& node);
			const DocPartObj& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string>	Gallery;
			property<bool>									Unique;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DOC_PART_OBJ_INCLUDE_H_