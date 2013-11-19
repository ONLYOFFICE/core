#pragma once
#ifndef ODT_WRITING_ELEMENT_INCLUDE_H_
#define ODT_WRITING_ELEMENT_INCLUDE_H_

#include "NamespaceOwn.h"
#include "XML.h"


namespace Odt
{
	class WritingElement : public NamespaceOwn, public XML::XObject
	{
	public:
		WritingElement() {};
		virtual ~WritingElement() = 0 {};
	};
} // namespace Odt

#endif // ODT_WRITING_ELEMENT_INCLUDE_H_