#pragma once
#ifndef ODT_CONTENT_TEXT_ITEM_INCLUDE_H_
#define ODT_CONTENT_TEXT_ITEM_INCLUDE_H_

#include "./../WritingElement.h"


namespace Odt
{
	namespace Content
	{
		class TextItem : public WritingElement
		{
		public:
			virtual const bool hasNote() const = 0;
			virtual const bool hasList() const = 0;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TEXT_ITEM_INCLUDE_H_