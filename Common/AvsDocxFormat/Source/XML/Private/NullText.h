#pragma once
#ifndef XML_PRIVATE_NULL_TEXT_INCLUDE_H_
#define XML_PRIVATE_NULL_TEXT_INCLUDE_H_

#include "Text.h"


namespace XML
{
	namespace Private
	{
		class NullText : public Text
		{
		};

		static NullText nullText;

	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NULL_TEXT_INCLUDE_H_