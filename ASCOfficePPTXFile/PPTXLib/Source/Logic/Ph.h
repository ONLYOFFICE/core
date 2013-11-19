#ifndef PPTX_LOGIC_PH_INCLUDE_H_
#define PPTX_LOGIC_PH_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "./../Limit/Orient.h"
#include "./../Limit/PlaceholderSize.h"
#include "./../Limit/PlaceholderType.h"

namespace PPTX
{
	namespace Logic
	{
		class Ph : public WrapperWritingElement
		{
		public:
			Ph();
			virtual ~Ph();			
			explicit Ph(const XML::XNode& node);
			const Ph& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<bool> hasCustomPrompt;
			nullable_property<std::string> idx;
			nullable_property<std::string, Limit::Orient> orient;
			nullable_property<std::string, Limit::PlaceholderSize> sz;
			nullable_property<std::string, Limit::PlaceholderType> type;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PH_INCLUDE_H