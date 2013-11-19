#pragma once
#ifndef OOX_LOGIC_DOC_PROPERTIES_INCLUDE_H_
#define OOX_LOGIC_DOC_PROPERTIES_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"


namespace OOX
{
	namespace Logic
	{
		class DocPr : public WritingElement
		{
		public:
			DocPr();
			virtual ~DocPr();			
			explicit DocPr(const XML::XNode& node);
			const DocPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int>										Id;
			property<std::string>						Name;
			nullable_property<std::string>	Descr;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DOC_PROPERTIES_INCLUDE_H_