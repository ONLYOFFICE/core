/// \file   sax_win32_xmllite.h
/// \brief  Реализация интерфейса SAX через библиотеку XmlLite

#ifndef _CPDOCCORE_XML_SAX_WIN32_XMLLITE_H_
#define _CPDOCCORE_XML_SAX_WIN32_XMLLITE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <cpdoccore/xml/sax.h>
#include <cpdoccore/CPSharedPtr.h>

namespace cpdoccore {
namespace xml {

sax_ptr create_sax_win32_xmllite(const wchar_t * FileName);
sax_ptr create_sax_win32_xmllite( shared_ptr< std::istream >::Type istreamPtr);
sax_ptr create_sax_win32_xmllite( std::istream & istreamVal );

} 
}

#endif // #ifndef _CPDOCCORE_XML_SAX_WIN32_XMLLITE_H_
