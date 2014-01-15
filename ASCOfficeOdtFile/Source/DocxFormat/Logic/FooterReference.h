#ifndef OOX_LOGIC_FOOTER_REFERENCE_INCLUDE_H_
#define OOX_LOGIC_FOOTER_REFERENCE_INCLUDE_H_

#include "./../WritingElement.h"
#include "./../RId.h"
#include "property.h"
#include "./../Limit/TypeFootHead.h"


namespace OOX
{
	namespace Logic
	{
		class FooterReference : public WritingElement
		{
		public:
			FooterReference();
			virtual ~FooterReference();
			explicit FooterReference(const XML::XNode& node);
			const FooterReference& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			property<RId>																rId;
			property<std::string, Limit::TypeFootHead>	Type;	
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FOOTER_REFERENCE_INCLUDE_H_