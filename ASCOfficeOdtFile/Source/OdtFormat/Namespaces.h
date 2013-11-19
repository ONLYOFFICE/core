#pragma once
#ifndef ODT_NAMESPACES_INCLUDE_H_
#define ODT_NAMESPACES_INCLUDE_H_

#include "XML.h"
#include <boost/utility.hpp>


namespace Odt
{
	class Namespaces : private boost::noncopyable
	{
	public:
		Namespaces();

	public:
		const XML::XNamespace office;
		const XML::XNamespace config;
		const XML::XNamespace style;
		const XML::XNamespace text;
		const XML::XNamespace table;
		const XML::XNamespace draw;
		const XML::XNamespace fo;
		const XML::XNamespace xlink;
		const XML::XNamespace dc;
		const XML::XNamespace meta;
		const XML::XNamespace number;
		const XML::XNamespace svg;
		const XML::XNamespace chart;
		const XML::XNamespace dr3d;
		const XML::XNamespace math;
		const XML::XNamespace form;
		const XML::XNamespace script;
		const XML::XNamespace ooo;
		const XML::XNamespace ooow;
		const XML::XNamespace oooc;
		const XML::XNamespace dom;
		const XML::XNamespace xforms;
		const XML::XNamespace xsd;
		const XML::XNamespace xsi;
		const XML::XNamespace rpt;
		const XML::XNamespace of;
		const XML::XNamespace rdfa;
		const XML::XNamespace manifest;
		const XML::XNamespace presentation;
		const XML::XNamespace	smil;
		const	XML::XNamespace anim;
		const	XML::XNamespace field;
		const	XML::XNamespace formx;
};

} // namespace Odt

#endif // ODT_NAMESPACES_INCLUDE_H_