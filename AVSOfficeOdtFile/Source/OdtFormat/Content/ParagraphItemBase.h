#ifndef ODT_CONTENT_PARAGRAPH_ITEM_BASE_INCLUDE_H_
#define ODT_CONTENT_PARAGRAPH_ITEM_BASE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>


namespace Odt
{
	namespace Content
	{
		class ParagraphItemBase : public WritingElement
		{
		public:
			virtual const std::string toTxt() const = 0;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_PARAGRAPH_ITEM_BASE_INCLUDE_H_