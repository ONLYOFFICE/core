#pragma once
#ifndef XML_INCLUDE_H_
#define XML_INCLUDE_H_

#include "XAttribute.h"
#include "XNamespace.h"
#include "XText.h"
#include "XContainer.h"
#include "XElement.h"
#include "XNode.h"
#include "XDocument.h"
#include "Private/XList.h"
#include "Extension/Write.h"
#include "Extension/WriteIf.h"
#include "Extension/Fill.h"
#include "Extension/FillFromAttribute.h"
#include "XObject.h"
#include "XComment.h"


namespace XML
{
	typedef XML::Private::XAttributeContainer::iterator			attribute_iterator;
	typedef XML::Private::XAttributeContainer::const_iterator	const_attribute_iterator;
	typedef XML::Private::XNamespaceContainer::iterator			namespace_iterator;
	typedef XML::Private::XNamespaceContainer::const_iterator	const_namespace_iterator;
	typedef XML::Private::XNodeContainer::iterator				node_iterator;
	typedef XML::Private::XNodeContainer::const_iterator		const_node_iterator;
	typedef XML::Private::XElementContainer::iterator			element_iterator;
	typedef XML::Private::XElementContainer::const_iterator		const_element_iterator;
	typedef XML::Private::XTextContainer::iterator				text_iterator;
	typedef XML::Private::XTextContainer::const_iterator		const_text_iterator;
} // namespace XML

#endif // XML_INCLUDE_H_